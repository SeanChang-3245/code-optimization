#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <chrono>
#include <immintrin.h>
#include <omp.h>

using real_t = float;

void write_matrix(const std::string &filename, const real_t *data, size_t count, bool append=false) {
    std::ofstream ofs;
    if (append)
        ofs.open(filename, std::ios::binary | std::ios::app);
    else
        ofs.open(filename, std::ios::binary);

    if (!ofs) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        std::exit(1);
    }
    ofs.write(reinterpret_cast<const char*>(data), count * sizeof(real_t));
}

inline void matmul(real_t *A, real_t *B, real_t *C, int M, int K, int N) {

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < K; ++k) {
                C[i * N + j] += A[i * K + k] * B[k * N + j];
            }
        }
    }
}

inline void matmul_avx2(real_t *A, real_t *B, real_t *C, int M, int K, int N) {
    
    for (int i = 0; i < M; ++i) {
        for (int k = 0; k < K; ++k) {
            __m256 a_vec = _mm256_set1_ps(A[i * K + k]);
            int j = 0;
            // avx2
            for (; j <= N - 8; j += 8) {
                __m256 b_vec = _mm256_loadu_ps(&B[k * N + j]);
                __m256 c_vec = _mm256_loadu_ps(&C[i * N + j]);
                _mm256_storeu_ps(&C[i * N + j], _mm256_add_ps(_mm256_mul_ps(a_vec, b_vec), c_vec));
            }

            // remainder part
            for (; j < N; ++j) {
                C[i * N + j] += A[i * K + k] * B[k * N + j];
            }
        }
    }   
}

inline void matmul_blocked(real_t *A, real_t *B, real_t *C, int M, int K, int N) {
    // [TODO]: implement blocked matrix multiplication

}

inline void matmul_avx2_blocked(real_t *A, real_t *B, real_t *C, int M, int K, int N) {
    // [TODO]: implement blocked matrix multiplication with vectorization

}

inline void matmul_optimized(real_t *A, real_t *B, real_t *C, int M, int K, int N) {
    // [TODO] (optional): implement other optimization 
    
}




int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <M> <K> <N>" << std::endl;
        return 1;
    }

    int M = std::stoi(argv[1]);
    int K = std::stoi(argv[2]);
    int N = std::stoi(argv[3]);

    real_t *A = (real_t *)(_mm_malloc(M * K * sizeof(real_t), 32));
    real_t *B = (real_t *)(_mm_malloc(K * N * sizeof(real_t), 32));
    real_t *C = (real_t *)(_mm_malloc(M * N * sizeof(real_t), 32));

    auto run_test = [&](std::string name, auto func) {
        memset(C, 0, M * N * sizeof(real_t));
        auto start = std::chrono::high_resolution_clock::now();
        func(A, B, C, M, K, N);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "[" << name << "]: " << duration.count() << "ms\n";
        write_matrix(name + ".bin", C,  static_cast<size_t>(M) * N, false);
    };


    if (!A || !B || !C) {
        std::cerr << "Aligned allocation failed" << std::endl;
        return 1;
    }

    // Read data from files: A then B from input.bin
    std::ifstream ifs("input.bin", std::ios::binary);
    if (!ifs) {
        std::cerr << "Cannot open input.bin" << std::endl;
        return 1;
    }
    ifs.read(reinterpret_cast<char*>(A), static_cast<size_t>(M) * K * sizeof(real_t));
    ifs.read(reinterpret_cast<char*>(B), static_cast<size_t>(K) * N * sizeof(real_t));
    ifs.close();  // Close the file after reading

    // Matrix multiplication    
    run_test("naive", matmul);
    run_test("avx", matmul_avx2);
    run_test("blocked", matmul_blocked);
    run_test("avx_blocked", matmul_avx2_blocked);
    // run_test("opt", matmul_optimized);

    // Free allocated memory
    _mm_free(A);
    _mm_free(B);
    _mm_free(C);

    return 0;
}
