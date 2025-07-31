#pragma once
#include "Data.h"
#include "ProcessingStage.h"
#include "OutputSink.h"
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <memory>
#include <vector>
#include <string>

class PipelineEngine {
public:
    void build(const nlohmann::json& pipelineJson);
    void run(const std::string& startNode, const Data& input);
    void exportToDot(const std::string& filename) const;

private:
    std::unordered_map<std::string, std::shared_ptr<ProcessingStage>> processors;
    std::unordered_map<std::string, std::shared_ptr<OutputSink>> sinks;
    std::unordered_map<std::string, std::vector<std::string>> graph;
};

