#include "../include/Data.h"
#include <cmath>
#include <chrono>

class SineWaveGenerator {
    double freq, amp;
    int rate;
    double time;

public:
    SineWaveGenerator(double f, double a, int r) : freq(f), amp(a), rate(r), time(0.0) {}

    Data generate() {
        std::vector<double> values;
        for (int i = 0; i < 100; ++i) {
            values.push_back(amp * std::sin(2 * M_PI * freq * time));
            time += 1.0 / rate;
        }
        return {values, static_cast<uint64_t>(
            std::chrono::system_clock::now().time_since_epoch().count())};
    }
};

