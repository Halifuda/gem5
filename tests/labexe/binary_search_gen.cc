/*
 * Arch 2024 fall course code.
 * - This is a C++ code generating arrays for Binary Search algorithm.
 * - This code is generated by Github Copilot.
 * - This program will generate an array of random integers and write
 * them to the given file as a C-style array. It will also pick some
 * target values from the array and write them to the file.
 */

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cout << "Usage: " << argv[0] << " <file_name> <n> <m>\n";
        return 1;
    }

    std::ofstream file(argv[1]);
    if (!file.is_open()) {
        std::cout << "Error: Unable to open file " << argv[1] << "\n";
        return 1;
    }

    int n = std::atoi(argv[2]);
    int m = std::atoi(argv[3]);
    file << "#define N " << n << "\n";
    file << "#define M " << m << "\n";
    int *arr = new int[n];

    // Generate integers with random values, ranging from 0 to 1e9
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1e9);
    // Write the array to the file as a C-style array
    for (int i = 0; i < n; i++) {
        arr[i] = dis(gen);
    }
    std::sort(arr, arr + n);
    file << "int ARR[] = {";
    for (int i = 0; i < n; i++) {
        file << arr[i];
        if (i != n - 1) file << ", ";
    }
    file << "};\n";
    // Pick some target values from the array
    file << "int TARGETS[] = {";
    for (int i = 0; i < m; i++) {
        file << arr[dis(gen) % n];
        if (i != m - 1) file << ", ";
    }
    file << "};\n";
    file << "\n";
    file.close();

    return 0;
}
