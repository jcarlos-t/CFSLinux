#ifndef BENCHMARKS_SPACE_H
#define BENCHMARKS_SPACE_H

#include "RB_tree_benchmark.h"
#include <random>
#include <vector>
#include <algorithm>

struct BenchmarkSpaceResult {
    int data_size;
    int actual_height;
    double theoretical_max_height;
    size_t memory_bytes;
    double memory_kb;
    double memory_mb;
};

BenchmarkSpaceResult run_benchmark_space(int size) {
    const int NUM_REPETITIONS = 10;
    BenchmarkSpaceResult result;
    result.data_size = size;
    long long total_height = 0;
    double total_theoretical_height = 0.0;
    size_t total_memory = 0;

    for (int rep = 0; rep < NUM_REPETITIONS; rep++) {
        std::vector<int> data(size);

        for (int i = 0; i < size; i++) {
            data[i] = i + 1;
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(data.begin(), data.end(), gen);
        
        RB_tree_benchmark<int> tree;
        
        for (int i = 0; i < size; i++) {
            tree.add_leaf(data[i]);
        }
        total_height += tree.get_height();
        total_theoretical_height += tree.get_theoretical_max_height();
        total_memory += tree.get_memory_usage();
    }
    

    result.actual_height = static_cast<int>(total_height / NUM_REPETITIONS);
    result.theoretical_max_height = total_theoretical_height / NUM_REPETITIONS;
    result.memory_bytes = total_memory / NUM_REPETITIONS;
    result.memory_kb = result.memory_bytes / 1024.0;
    result.memory_mb = result.memory_kb / 1024.0;
    
    return result;
}

#endif // BENCHMARKS_SPACE_H