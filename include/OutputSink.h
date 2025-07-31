#pragma once
#include "Data.h"

class OutputSink {
public:
    virtual void write(const Data& d) = 0;
    virtual ~OutputSink() = default;
};

