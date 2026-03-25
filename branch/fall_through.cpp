#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>

using namespace std;
using namespace std::chrono;

volatile int sink = 0; // volatile prevents compiler optimization
const int ITERATIONS = 100000000;

void processNormal() {
    sink += 1;
}

void handleError() {
    sink += 2;
}


void fallThrough(const vector<bool>& errors) {
    for (bool err : errors) {
        if (!err) {
            processNormal();
        } else {
            handleError();
        }
    }
}

void branch(const vector<bool>& errors) {
    for (bool err : errors) {
        if (err) {
            handleError();
        } else {
            processNormal();
        }
    }
}

void compilerReorder(const vector<bool>& errors) {
    for (bool err : errors) {
        if (err) [[unlikely]] {
            handleError();
        } else {
            processNormal();
        }
    }
}

int main() {
    vector<bool> error_data(ITERATIONS);
    mt19937 gen(42); 
    bernoulli_distribution less_error_dist(0.001);
    bernoulli_distribution even_error_dist(0.5);
    
    auto run_test = [&](string name, auto func) {
        sink = 0;
        auto start = high_resolution_clock::now();
        func(error_data);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);
        cout << name << ": " << duration.count() << " ms\n";
    };

    
    cout << "----- Error Rate = 0.01% -----\n";
    for (int i = 0; i < ITERATIONS; ++i) {
        error_data[i] = less_error_dist(gen);
    }
    fallThrough(error_data); // Cache Warm-up
    run_test("[Fall Through]", fallThrough);
    run_test("[Branch]", branch);
    run_test("[Compiler Reorder]", compilerReorder);
    

    cout << "\n----- Error Rate = 50% -----\n";
    for (int i = 0; i < ITERATIONS; ++i) {
        error_data[i] = even_error_dist(gen);
    }
    fallThrough(error_data); // Cache Warm-up
    run_test("[Fall Through]", fallThrough);
    run_test("[Branch]", branch);
    run_test("[Compiler Reorder]", compilerReorder);
    return 0;
}