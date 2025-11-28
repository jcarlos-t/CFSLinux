#ifndef BENCHMARKS_SECONDS_H
#define BENCHMARKS_SECONDS_H

#include "../RB_tree.h"
#include <chrono>
#include <random>
#include <vector>
#include <algorithm>

struct BenchmarkResult {
    double insert_time;
    double search_time;
    double delete_time;
    int data_size;
};

BenchmarkResult run_benchmark(int size) {
    const int NUM_REPETITIONS = 10;
    BenchmarkResult result;
    result.data_size = size;
    result.insert_time = 0.0;
    result.search_time = 0.0;
    result.delete_time = 0.0;
    for (int rep = 0; rep < NUM_REPETITIONS; rep++) {
        std::vector<int> data(size);

        for (int i = 0; i < size; i++) {
            data[i] = i + 1;
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(data.begin(), data.end(), gen);

        RB_tree<int> tree;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < size; i++) {
            tree.add_leaf(data[i]);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        result.insert_time += diff.count();

        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < size; i++) {
            tree.find(data[i]);
        }
        end = std::chrono::high_resolution_clock::now();
        diff = end - start;
        result.search_time += diff.count();

        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < size; i++) {
            tree.delete_leaf(data[i]);
        }
        end = std::chrono::high_resolution_clock::now();
        diff = end - start;
        result.delete_time += diff.count();
    }
    
    result.insert_time /= NUM_REPETITIONS;
    result.search_time /= NUM_REPETITIONS;
    result.delete_time /= NUM_REPETITIONS;

    return result;
}

#endif // BENCHMARKS_SECONDS_H