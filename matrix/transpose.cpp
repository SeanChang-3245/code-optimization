#include <stdio.h>
#include <chrono>
#include <algorithm>

constexpr int N = 8192;
constexpr int TILE_SIZES[] = { 16, 32, 48, 64, 96, 128 };

void naive_transpose(const int* A, int* B) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            B[j * N + i] = A[i * N + j];
        }
    }
}

void tiling_transpose(const int* A, int* B, const int tile_size) {
    // [TODO]: implement blocked matrix transpose

}

int main() {
    int* A = new int[N * N];
    int* B = new int[N * N];
    std::fill(A, A + (N * N), 1);
    std::fill(B, B + (N * N), 0);

    // Warm-up run (avoid cold cache effects)
    naive_transpose(A, B); 
    
    auto start = std::chrono::high_resolution_clock::now();
    naive_transpose(A, B);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dur = end - start;
    printf("Naive: %f s\n", dur.count());
    
    for (size_t i = 0; i < sizeof(TILE_SIZES) / sizeof(int); ++i) {
        start = std::chrono::high_resolution_clock::now();
        tiling_transpose(A, B, TILE_SIZES[i]);
        end = std::chrono::high_resolution_clock::now();
        dur = end - start;
        printf("Tiling (%d): %f s\n", TILE_SIZES[i], dur.count());
    }


    delete[] A;
    delete[] B;

    return 0;
}