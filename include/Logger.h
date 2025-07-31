#pragma once

// Importing spdlog headers for logging
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <memory>

// Logger class: provides a static logger instance to use across the project
class Logger {
public:
    // Returns the shared logger instance (singleton pattern)
    static std::shared_ptr<spdlog::logger> get() {
        // Saves logs to the pipeline.log file
        static auto logger = spdlog::basic_logger_mt("pipeline_logger", "output/pipeline.log");
        return logger;
    }

    // Initializes log format
    static void init() {
        // Sets log message as this format with date, level, and message
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [thread %t] [%^%l%$] %v");

        // Log everything (debug/info/warn/error)
        get()->set_level(spdlog::level::debug);
    }
};

