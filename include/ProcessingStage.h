#pragma once
#include "Data.h"

class ProcessingStage {
public:
    virtual Data process(const Data& d) = 0;
    virtual ~ProcessingStage() = default;
};

