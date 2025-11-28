#ifndef CSV_EXPORT_H
#define CSV_EXPORT_H

#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>  // Para _mkdir en Windows
#else
#include <sys/types.h>
#endif
#include "benchmarks_seconds.h"
#include "benchmarks_comparisons.h"
#include "benchmarks_space.h"

inline bool create_directory_if_not_exists(const std::string& path) {
#ifdef _WIN32
    struct _stat info;
    if (_stat(path.c_str(), &info) != 0) {
        // El directorio no existe, intentar crearlo
        return _mkdir(path.c_str()) == 0;
    }
    return (info.st_mode & _S_IFDIR) != 0;
#else
    struct stat info;
    if (stat(path.c_str(), &info) != 0) {
        // El directorio no existe, intentar crearlo
        return mkdir(path.c_str(), 0755) == 0;
    }
    return S_ISDIR(info.st_mode);
#endif
}

inline std::string get_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y%m%d_%H%M%S");
    return ss.str();
}

inline void export_time_benchmark_to_csv(
    const std::vector<BenchmarkResult>& results,
    const std::string& output_dir = "benchmarks_out"
) {
    create_directory_if_not_exists(output_dir);
    
    std::string filename = output_dir + "/benchmark_time_" + get_timestamp() + ".csv";
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo crear el archivo " << filename << std::endl;
        return;
    }
    
    // Escribir encabezados
    file << "Data_Size,Insert_Time_Seconds,Search_Time_Seconds,Delete_Time_Seconds\n";
    
    // Escribir datos
    for (const auto& result : results) {
        file << result.data_size << ","
             << std::fixed << std::setprecision(6) << result.insert_time << ","
             << std::fixed << std::setprecision(6) << result.search_time << ","
             << std::fixed << std::setprecision(6) << result.delete_time << "\n";
    }
    
    file.close();
    std::cout << "\n✓ Resultados exportados a: " << filename << std::endl;
}

inline void export_comparison_benchmark_to_csv(
    const std::vector<BenchmarkComparisonResult>& results,
    const std::string& output_dir = "benchmarks_out"
) {
    create_directory_if_not_exists(output_dir);
    
    std::string filename = output_dir + "/benchmark_comparisons_" + get_timestamp() + ".csv";
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo crear el archivo " << filename << std::endl;
        return;
    }

    file << "Data_Size,Insert_Comparisons,Search_Comparisons,Delete_Comparisons\n";

    for (const auto& result : results) {
        file << result.data_size << ","
             << result.insert_comparisons << ","
             << result.search_comparisons << ","
             << result.delete_comparisons << "\n";
    }
    
    file.close();
    std::cout << "\n✓ Resultados exportados a: " << filename << std::endl;
}

inline void export_space_benchmark_to_csv(
    const std::vector<BenchmarkSpaceResult>& results,
    const std::string& output_dir = "benchmarks_out"
) {
    create_directory_if_not_exists(output_dir);
    
    std::string filename = output_dir + "/benchmark_space_" + get_timestamp() + ".csv";
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo crear el archivo " << filename << std::endl;
        return;
    }

    file << "Data_Size,Actual_Height,Theoretical_Max_Height,Memory_Bytes,Memory_KB,Memory_MB\n";

    for (const auto& result : results) {
        file << result.data_size << ","
             << result.actual_height << ","
             << std::fixed << std::setprecision(2) << result.theoretical_max_height << ","
             << result.memory_bytes << ","
             << std::fixed << std::setprecision(2) << result.memory_kb << ","
             << std::fixed << std::setprecision(4) << result.memory_mb << "\n";
    }
    
    file.close();
    std::cout << "\n✓ Resultados exportados a: " << filename << std::endl;
}

#endif // CSV_EXPORT_H
