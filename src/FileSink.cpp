#include "../include/OutputSink.h"
#include "../include/Logger.h"
#include "../include/Metrics.h"
#include <fstream>
#include <filesystem>

class FileSink : public OutputSink {
    std::string path;
    std::ofstream file;

public:
    // Constructor opens the file in ./output/ directory
    FileSink(const std::string& p)
        : path("output/" + p), file("output/" + p, std::ios::out) {
        std::filesystem::create_directories("output");  // Ensure output dir exists
    }

    // Write values to the file, one per line
    void write(const Data& d) override {
        Logger::get()->info("[FileSink:{}] Writing {} samples", path, d.values.size());
        for (auto v : d.values) {
            file << v << "\n";
        }
        Metrics::output_count += d.values.size();
    }

    // Ensure clean file closure
    ~FileSink() {
        file.close();
    }
};

