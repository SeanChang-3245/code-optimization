#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>

using namespace std;
using namespace std::chrono;

constexpr int ITERATIONS = 1000000000;


void unroll(vector<int>& data) {
    #pragma GCC unroll 8
    for (int i = 0; i < ITERATIONS; ++i) {
        data[i] += data[i] * 2 + 1;
    }
}

void normal(vector<int>& data) {
    for (int i = 0; i < ITERATIONS; ++i) {
        data[i] += data[i] * 2 + 1;
    }
}

int main() {
    vector<int> data(ITERATIONS);
    auto run_test = [&](string name, auto func) {
        auto start = high_resolution_clock::now();
        func(data);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);
        cout << name << ": " << duration.count()<< " ms\n";
    };

    for (int i = 0; i < ITERATIONS; ++i) {
        data[i] = i;
    }
    run_test("[Normal]", normal);

    for (int i = 0; i < ITERATIONS; ++i) {
        data[i] = i;
    }
    run_test("[Unroll]", unroll);
    
    return 0;
}