#include "../include/Data.h"
#include "../include/ProcessingStage.h"
#include <fftw3.h>
#include <vector>
#include <complex>

class FFT : public ProcessingStage {
public:
    Data process(const Data& input) override {
        int N = input.values.size();
        std::vector<double> in(N);
        std::vector<std::complex<double>> out(N);

        for (int i = 0; i < N; ++i)
            in[i] = input.values[i];

        fftw_complex* fft_out = reinterpret_cast<fftw_complex*>(out.data());
        fftw_plan plan = fftw_plan_dft_r2c_1d(N, in.data(), fft_out, FFTW_ESTIMATE);
        fftw_execute(plan);
        fftw_destroy_plan(plan);

        std::vector<double> magnitudes;
        for (int i = 0; i < N / 2; ++i)
            magnitudes.push_back(std::abs(out[i]));

        return {magnitudes, input.timestamp};
    }
};

