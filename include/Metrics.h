#pragma once
#include <atomic>
#include <fstream>
#include <nlohmann/json.hpp>

class Metrics {
public:
    inline static std::atomic<uint64_t> input_count{0};
    inline static std::atomic<uint64_t> processed_count{0};
    inline static std::atomic<uint64_t> output_count{0};

    static void report(const std::string& path = "output/metrics.json") {
        nlohmann::json j;
        j["input_count"] = input_count.load();
        j["processed_count"] = processed_count.load();
        j["output_count"] = output_count.load();

        std::ofstream out(path);
        out << j.dump(2);
    }
};

