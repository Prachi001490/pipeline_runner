#include <CLI/CLI.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <thread>
#include <atomic>
#include <filesystem>

#include "../include/Data.h"
#include "../include/BlockingQueue.h"
#include "../include/PipelineEngine.h"
#include "../include/Logger.h"
#include "../include/Metrics.h"
#include "SineWaveGenerator.cpp"

using json = nlohmann::json;

int main(int argc, char** argv) {
    // -------------------- SETUP OUTPUT DIRECTORY --------------------
    std::filesystem::create_directories("output");

    // -------------------- INITIALIZE LOGGER --------------------
    Logger::init();
    auto logger = Logger::get();

    // -------------------- PARSE CLI --------------------
    CLI::App app{"Pipeline Runner CLI"};
    std::string configPath = "../configs/pipeline.json";
    app.add_option("-c,--config", configPath, "Path to config file");
    CLI11_PARSE(app, argc, argv);

    logger->info("Starting pipeline with config: {}", configPath);

    // -------------------- LOAD CONFIG --------------------
    std::ifstream configFile(configPath);
    if (!configFile) {
        logger->error("Cannot open config file: {}", configPath);
        return 1;
    }

    json config;
    try {
        configFile >> config;
    } catch (const std::exception& ex) {
        logger->critical("Failed to parse config: {}", ex.what());
        return 1;
    }

    // -------------------- EXTRACT CONFIG --------------------
    double freq = config["input"].value("frequency", 2.0);
    double amp  = config["input"].value("amplitude", 1.0);
    int    rate = config["input"].value("rate", 100);
    std::string startNode = config["start_node"];

    logger->info("Configured sine wave: freq={}, amp={}, rate={}", freq, amp, rate);
    logger->info("Pipeline will start from node: {}", startNode);

    // -------------------- INIT COMPONENTS --------------------
    SineWaveGenerator generator(freq, amp, rate);
    PipelineEngine engine;
    engine.build(config["pipeline"]);
    engine.exportToDot("pipeline_graph.dot");
    logger->info("Exported DAG to output/pipeline_graph.dot");

    BlockingQueue<Data> inputQueue;
    std::atomic<bool> running{true};

    // -------------------- THREAD 1: Input --------------------
    std::thread inputThread([&] {
        while (running) {
            Data d = generator.generate();
            inputQueue.push(d);
            Metrics::input_count++;
            logger->info("[Input Thread] Pushed {} samples at {}", d.values.size(), d.timestamp);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        logger->info("[Input Thread] Exiting");
    });

    // -------------------- THREAD 2: Processing --------------------
    std::thread processingThread([&] {
        while (running || !inputQueue.empty()) {
            if (!inputQueue.empty()) {
                Data d = inputQueue.pop();
                Metrics::processed_count++;
                logger->info("[Processing Thread] Processing {} samples", d.values.size());
                engine.run(startNode, d);
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
        logger->info("[Processing Thread] Exiting");
    });

    // -------------------- THREAD 3: Periodic Metrics Logger --------------------
    std::thread metricsThread([&] {
        while (running) {
            Metrics::report(); // write to output/metrics.json
            logger->info("[Metrics] input={}, processed={}, output={}",
                         Metrics::input_count.load(),
                         Metrics::processed_count.load(),
                         Metrics::output_count.load());
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        logger->info("[Metrics Thread] Exiting");
    });

    // -------------------- RUN FOR N SECONDS --------------------
    logger->info("Pipeline is running...");
    std::this_thread::sleep_for(std::chrono::seconds(10));
    running = false;

    // -------------------- JOIN THREADS --------------------
    inputThread.join();
    processingThread.join();
    metricsThread.join();

    logger->info("Pipeline terminated gracefully.");
    return 0;
}

