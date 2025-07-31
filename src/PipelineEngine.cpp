#include "../include/PipelineEngine.h"
#include "FFT.cpp"
#include "Filter.cpp"
#include "FileSink.cpp"
#include <fstream>
#include <iostream>

void PipelineEngine::build(const nlohmann::json& pipelineJson) {
    for (const auto& node : pipelineJson) {
        std::string name = node["name"];
        std::string type = node["type"];

        if (type == "FFT") {
            processors[name] = std::make_shared<FFT>();
        } else if (type == "Filter") {
            double minV = node.value("min", 0.0);
            double maxV = node.value("max", 1.0);
            processors[name] = std::make_shared<Filter>(minV, maxV);
        } else if (type == "FileSink") {
            std::string path = node.value("path", "output/output.txt");
            sinks[name] = std::make_shared<FileSink>(path);
        } else {
            throw std::runtime_error("Unknown stage type: " + type);
        }

        for (const auto& next : node.value("next", std::vector<std::string>{})) {
            graph[name].push_back(next);
        }
    }
}

void PipelineEngine::run(const std::string& node, const Data& input) {
    // Is it an OutputSink?
    if (sinks.count(node)) {
        sinks[node]->write(input);
        return;
    }

    // Is it a processor stage?
    if (processors.count(node)) {
        Data result = processors[node]->process(input);
        for (const auto& next : graph[node]) {
            run(next, result);
        }
        return;
    }

    throw std::runtime_error("Node not found in pipeline: " + node);
}

void PipelineEngine::exportToDot(const std::string& filename) const {
    std::ofstream out("output/" +filename);
    out << "digraph Pipeline {\n";
    for (const auto& [from, toList] : graph) {
        for (const auto& to : toList) {
            out << "  \"" << from << "\" -> \"" << to << "\";\n";
        }
    }
    out << "}\n";
    out.close();
}

