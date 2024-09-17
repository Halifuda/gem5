/*
 * Arch 2024 fall course code.
 * - This is an example C code of GEMM.
 * - This code is generated by Github Copilot.
 * - The matrices are directly included to avoid the reading process
 * for a quicker simulation.
 * - This program will simply multiply two matrices.
 * - This program is used for benchmarking the Branch Predictor and Cache
 * Prefetcher in gem5.
 */

#include "gemm.in"

void gemm(int n, int m, int k, double A[n][k], double B[m][k], double C[n][m]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            C[i][j] = 0;
            for (int l = 0; l < k; l++) {
                C[i][j] += A[i][l] * B[j][l];
            }
        }
    }
}

int main(int argc, char *argv[]) {
    gemm(N, M, K, A, B, C);
    return 0;
}
