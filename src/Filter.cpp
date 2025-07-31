#include "../include/Data.h"
#include "../include/ProcessingStage.h"
#include <algorithm>

class Filter : public ProcessingStage {
    double minVal, maxVal;

public:
    Filter(double minV, double maxV) : minVal(minV), maxVal(maxV) {}

    Data process(const Data& input) override {
        std::vector<double> filtered;
        std::copy_if(input.values.begin(), input.values.end(), std::back_inserter(filtered),
                     [&](double v) { return v >= minVal && v <= maxVal; });

        return {filtered, input.timestamp};
    }
};

