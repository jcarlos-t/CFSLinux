#ifndef BENCHMARKS_COMPARISONS_H
#define BENCHMARKS_COMPARISONS_H

#include "RB_tree_benchmark.h"
#include <random>
#include <vector>
#include <algorithm>

struct BenchmarkComparisonResult {
    long long insert_comparisons;
    long long search_comparisons;
    long long delete_comparisons;
    int data_size;
};

BenchmarkComparisonResult run_benchmark_comparisons(int size) {
    const int NUM_REPETITIONS = 10;
    BenchmarkComparisonResult result;
    result.data_size = size;
    result.insert_comparisons = 0;
    result.search_comparisons = 0;
    result.delete_comparisons = 0;
    for (int rep = 0; rep < NUM_REPETITIONS; rep++) {
        std::vector<int> data(size);
        for (int i = 0; i < size; i++) {
            data[i] = i + 1;
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(data.begin(), data.end(), gen);
        
        RB_tree_benchmark<int> tree;
        tree.reset_comparisons();
        for (int i = 0; i < size; i++) {
            tree.add_leaf(data[i]);
        }
        result.insert_comparisons += tree.get_comparisons();
        
        tree.reset_comparisons();
        for (int i = 0; i < size; i++) {
            tree.find(data[i]);
        }
        result.search_comparisons += tree.get_comparisons();
        
        tree.reset_comparisons();
        for (int i = 0; i < size; i++) {
            tree.delete_leaf(data[i]);
        }
        result.delete_comparisons += tree.get_comparisons();
    }
    
    result.insert_comparisons /= NUM_REPETITIONS;
    result.search_comparisons /= NUM_REPETITIONS;
    result.delete_comparisons /= NUM_REPETITIONS;
    
    return result;
}

#endif // BENCHMARKS_COMPARISONS_H