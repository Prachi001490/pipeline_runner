#include "../src/FFT.cpp"
#include <cassert>
#include <iostream>

int main() {
    Data d{{1, 2, 3}, 12345};
    FFT f;
    Data out = f.process(d);
    assert(out.values == d.values);
    std::cout << "FFT test passed.\n";
    return 0;
}

