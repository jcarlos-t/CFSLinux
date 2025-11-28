#include <iostream>
#include <iomanip>
#include <limits>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif
#include "RB_tree.h"
#include "benchmarks/benchmarks_seconds.h"
#include "benchmarks/benchmarks_comparisons.h"
#include "benchmarks/benchmarks_space.h"
#include "benchmarks/csv_export.h"
#include "simulacion.cpp"

void print_separator() {
    std::cout << std::string(50, '=') << '\n';
}

void print_menu() {
    print_separator();
    std::cout << "        ÁRBOL ROJO-NEGRO - MENÚ INTERACTIVO\n";
    print_separator();
    std::cout << "  OPERACIONES BÁSICAS:" << std::endl;
    std::cout << "    1. Insertar elemento" << std::endl;
    std::cout << "    2. Buscar elemento" << std::endl;
    std::cout << "    3. Buscar predecesor" << std::endl;
    std::cout << "    4. Buscar sucesor" << std::endl;
    std::cout << "    5. Eliminar elemento" << std::endl;
    std::cout << "    6. Imprimir árbol" << std::endl;
    std::cout << "\n  BENCHMARKS:" << std::endl;
    std::cout << "    7. Benchmark - TIEMPO" << std::endl;
    std::cout << "    8. Benchmark - COMPARACIONES" << std::endl;
    std::cout << "    9. Benchmark - ESPACIO" << std::endl;
    std::cout << "\n  CFS Linux:" << std::endl;
    std::cout << "    10. Simulacion con precarga" << std::endl;
    std::cout << "    11. Simulacion personalizada" << std::endl;
    
    std::cout << "\n    0. Salir" << std::endl;
    print_separator();
    std::cout << "Seleccione una opción: ";
}

void print_benchmark_time_result(const char* name, const BenchmarkResult& result) {
    std::cout << "\n╔════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║  " << std::setw(54) << std::left << name << "║" << std::endl;
    std::cout << "╠════════════════════════════════════════════════════════╣" << std::endl;
    std::cout << "║  Tamaño de datos: " << std::setw(34) << std::right << result.data_size << std::setw(6) << "║" << std::endl;
    std::cout << "╠════════════════════════════════════════════════════════╣" << std::endl;
    std::cout << "║  Operación        │  Tiempo acumulado (segundos)       ║" << std::endl;
    std::cout << "╠═══════════════════╪════════════════════════════════════╣" << std::endl;
    std::cout << "║  INSERT           │  " << std::setw(32) << std::fixed << std::setprecision(6) << result.insert_time << std::setw(5) << "║" << std::endl;
    std::cout << "║  SEARCH           │  " << std::setw(32) << std::fixed << std::setprecision(6) << result.search_time << std::setw(5) << "║" << std::endl;
    std::cout << "║  DELETE           │  " << std::setw(32) << std::fixed << std::setprecision(6) << result.delete_time << std::setw(5) << "║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════╝" << std::endl;
}

void print_benchmark_comparison_result(const char* name, const BenchmarkComparisonResult& result) {
    std::cout << "\n╔════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║  " << std::setw(52) << std::left << name <<  "  ║" << std::endl;
    std::cout << "╠════════════════════════════════════════════════════════╣" << std::endl;
    std::cout << "║  Tamaño de datos: " << std::setw(34) << std::right << result.data_size << std::setw(6) <<  "║" << std::endl;
    std::cout << "╠════════════════════════════════════════════════════════╣" << std::endl;
    std::cout << "║  Operación        │  Número total de comparaciones     ║" << std::endl;
    std::cout << "╠═══════════════════╪════════════════════════════════════╣" << std::endl;
    std::cout << "║  INSERT           │  " << std::setw(32) << result.insert_comparisons << std::setw(5) << "║" << std::endl;
    std::cout << "║  SEARCH           │  " << std::setw(32) << result.search_comparisons << std::setw(5) << "║" << std::endl;
    std::cout << "║  DELETE           │  " << std::setw(32) << result.delete_comparisons << std::setw(5) << "║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════╝" << std::endl;
}

void print_benchmark_space_result(const char* name, const BenchmarkSpaceResult& result) {
    std::cout << "\n╔════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║  " << std::setw(52) << std::left << name <<  "  ║" << std::endl;
    std::cout << "╠════════════════════════════════════════════════════════╣" << std::endl;
    std::cout << "║  Tamaño de datos: " << std::setw(34) << std::right << result.data_size << std::setw(6) <<  "║" << std::endl;
    std::cout << "╠════════════════════════════════════════════════════════╣" << std::endl;
    std::cout << "║  Métrica                    │  Valor                   ║" << std::endl;
    std::cout << "╠═════════════════════════════╪══════════════════════════╣" << std::endl;
    std::cout << "║  Altura actual              │  " << std::setw(22) << result.actual_height << std::setw(3) << "║" << std::endl;
    std::cout << "║  Altura teórica máx.        │  " << std::setw(22) << std::fixed << std::setprecision(2) << result.theoretical_max_height << std::setw(3) << "║" << std::endl;
    std::cout << "║  Memoria (bytes)            │  " << std::setw(22) << result.memory_bytes << std::setw(3) << "║" << std::endl;
    std::cout << "║  Memoria (KB)               │  " << std::setw(22) << std::fixed << std::setprecision(2) << result.memory_kb << std::setw(3) << "║" << std::endl;
    std::cout << "║  Memoria (MB)               │  " << std::setw(22) << std::fixed << std::setprecision(4) << result.memory_mb << std::setw(3) << "║" << std::endl;
    std::cout << "╚═════════════════════════════╩══════════════════════════╝" << std::endl;
}

void run_all_time_benchmarks() {
    std::cout << "\n╔════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                                                        ║" << std::endl;
    std::cout << "║     BENCHMARK COMPLETO - TIEMPO (SEGUNDOS)             ║" << std::endl;
    std::cout << "║                                                        ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════╝" << std::endl;

    std::vector<BenchmarkResult> results;

    std::cout << "\n[1/4] Ejecutando benchmark con 10,000 elementos..." << std::endl;
    auto result1 = run_benchmark(10000);
    print_benchmark_time_result("BENCHMARK 10K", result1);
    results.push_back(result1);

    std::cout << "\n[2/4] Ejecutando benchmark con 50,000 elementos..." << std::endl;
    auto result2 = run_benchmark(50000);
    print_benchmark_time_result("BENCHMARK 50K", result2);
    results.push_back(result2);

    std::cout << "\n[3/4] Ejecutando benchmark con 100,000 elementos..." << std::endl;
    auto result3 = run_benchmark(100000);
    print_benchmark_time_result("BENCHMARK 100K", result3);
    results.push_back(result3);

    std::cout << "\n[4/4] Ejecutando benchmark con 200,000 elementos..." << std::endl;
    auto result4 = run_benchmark(200000);
    print_benchmark_time_result("BENCHMARK 200K", result4);
    results.push_back(result4);

    // Resumen comparativo
    std::cout << "\n\n╔════════════════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                    RESUMEN COMPARATIVO - TIEMPO                        ║" << std::endl;
    std::cout << "╠════════════╦═══════════════╦═══════════════╦═══════════════════════════╣" << std::endl;
    std::cout << "║   Tamaño   ║  INSERT (s)   ║  SEARCH (s)   ║  DELETE (s)               ║" << std::endl;
    std::cout << "╠════════════╬═══════════════╬═══════════════╬═══════════════════════════╣" << std::endl;
    std::cout << "║   10,000   ║  " << std::setw(11) << std::fixed << std::setprecision(6) << result1.insert_time
              << "  ║  " << std::setw(11) << result1.search_time
              << "  ║  " << std::setw(23) << result1.delete_time << "  ║" << std::endl;
    std::cout << "║   50,000   ║  " << std::setw(11) << result2.insert_time
              << "  ║  " << std::setw(11) << result2.search_time
              << "  ║  " << std::setw(23) << result2.delete_time << "  ║" << std::endl;
    std::cout << "║  100,000   ║  " << std::setw(11) << result3.insert_time
              << "  ║  " << std::setw(11) << result3.search_time
              << "  ║  " << std::setw(23) << result3.delete_time << "  ║" << std::endl;
    std::cout << "║  200,000   ║  " << std::setw(11) << result4.insert_time
              << "  ║  " << std::setw(11) << result4.search_time
              << "  ║  " << std::setw(23) << result4.delete_time << "  ║" << std::endl;
    std::cout << "╚════════════╩═══════════════╩═══════════════╩═══════════════════════════╝" << std::endl;
    
    // Exportar a CSV
    export_time_benchmark_to_csv(results);
}

void run_all_comparison_benchmarks() {
    std::cout << "\n╔════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                                                        ║" << std::endl;
    std::cout << "║        BENCHMARK COMPLETO - COMPARACIONES              ║" << std::endl;
    std::cout << "║                                                        ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════╝" << std::endl;

    std::vector<BenchmarkComparisonResult> results;

    std::cout << "\n[1/4] Ejecutando benchmark con 10,000 elementos..." << std::endl;
    auto result1 = run_benchmark_comparisons(10000);
    print_benchmark_comparison_result("BENCHMARK 10K", result1);
    results.push_back(result1);

    std::cout << "\n[2/4] Ejecutando benchmark con 50,000 elementos..." << std::endl;
    auto result2 = run_benchmark_comparisons(50000);
    print_benchmark_comparison_result("BENCHMARK 50K", result2);
    results.push_back(result2);

    std::cout << "\n[3/4] Ejecutando benchmark con 100,000 elementos..." << std::endl;
    auto result3 = run_benchmark_comparisons(100000);
    print_benchmark_comparison_result("BENCHMARK 100K", result3);
    results.push_back(result3);

    std::cout << "\n[4/4] Ejecutando benchmark con 200,000 elementos..." << std::endl;
    auto result4 = run_benchmark_comparisons(200000);
    print_benchmark_comparison_result("BENCHMARK 200K", result4);
    results.push_back(result4);

    // Resumen comparativo
    std::cout << "\n\n╔════════════════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                 RESUMEN COMPARATIVO - COMPARACIONES                    ║" << std::endl;
    std::cout << "╠════════════╦═══════════════╦═══════════════╦═══════════════════════════╣" << std::endl;
    std::cout << "║   Tamaño   ║    INSERT     ║    SEARCH     ║    DELETE                 ║" << std::endl;
    std::cout << "╠════════════╬═══════════════╬═══════════════╬═══════════════════════════╣" << std::endl;
    std::cout << "║   10,000   ║  " << std::setw(11) << result1.insert_comparisons
              << "  ║  " << std::setw(11) << result1.search_comparisons
              << "  ║  " << std::setw(23) << result1.delete_comparisons << "  ║" << std::endl;
    std::cout << "║   50,000   ║  " << std::setw(11) << result2.insert_comparisons
              << "  ║  " << std::setw(11) << result2.search_comparisons
              << "  ║  " << std::setw(23) << result2.delete_comparisons << "  ║" << std::endl;
    std::cout << "║  100,000   ║  " << std::setw(11) << result3.insert_comparisons
              << "  ║  " << std::setw(11) << result3.search_comparisons
              << "  ║  " << std::setw(23) << result3.delete_comparisons << "  ║" << std::endl;
    std::cout << "║  200,000   ║  " << std::setw(11) << result4.insert_comparisons
              << "  ║  " << std::setw(11) << result4.search_comparisons
              << "  ║  " << std::setw(23) << result4.delete_comparisons << "  ║" << std::endl;
    std::cout << "╚════════════╩═══════════════╩═══════════════╩═══════════════════════════╝" << std::endl;
    
    // Exportar a CSV
    export_comparison_benchmark_to_csv(results);
}

void run_all_space_benchmarks() {
    std::cout << "\n╔════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                                                        ║" << std::endl;
    std::cout << "║           BENCHMARK COMPLETO - ESPACIO                 ║" << std::endl;
    std::cout << "║                                                        ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════╝" << std::endl;

    std::vector<BenchmarkSpaceResult> results;

    std::cout << "\n[1/4] Ejecutando benchmark con 10,000 elementos..." << std::endl;
    auto result1 = run_benchmark_space(10000);
    print_benchmark_space_result("BENCHMARK 10K", result1);
    results.push_back(result1);

    std::cout << "\n[2/4] Ejecutando benchmark con 50,000 elementos..." << std::endl;
    auto result2 = run_benchmark_space(50000);
    print_benchmark_space_result("BENCHMARK 50K", result2);
    results.push_back(result2);

    std::cout << "\n[3/4] Ejecutando benchmark con 100,000 elementos..." << std::endl;
    auto result3 = run_benchmark_space(100000);
    print_benchmark_space_result("BENCHMARK 100K", result3);
    results.push_back(result3);

    std::cout << "\n[4/4] Ejecutando benchmark con 200,000 elementos..." << std::endl;
    auto result4 = run_benchmark_space(200000);
    print_benchmark_space_result("BENCHMARK 200K", result4);
    results.push_back(result4);

    // Resumen comparativo
    std::cout << "\n\n╔════════════════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                 RESUMEN COMPARATIVO - ESPACIO                          ║" << std::endl;
    std::cout << "╠════════════╦═══════════╦═══════════════╦════════════════════════════════╣" << std::endl;
    std::cout << "║   Tamaño   ║  Altura   ║  Alt. Teórica ║  Memoria (MB)                  ║" << std::endl;
    std::cout << "╠════════════╬═══════════╬═══════════════╬════════════════════════════════╣" << std::endl;
    std::cout << "║   10,000   ║  " << std::setw(7) << result1.actual_height
              << "  ║  " << std::setw(11) << std::fixed << std::setprecision(2) << result1.theoretical_max_height
              << "  ║  " << std::setw(28) << std::fixed << std::setprecision(4) << result1.memory_mb << "  ║" << std::endl;
    std::cout << "║   50,000   ║  " << std::setw(7) << result2.actual_height
              << "  ║  " << std::setw(11) << std::fixed << std::setprecision(2) << result2.theoretical_max_height
              << "  ║  " << std::setw(28) << std::fixed << std::setprecision(4) << result2.memory_mb << "  ║" << std::endl;
    std::cout << "║  100,000   ║  " << std::setw(7) << result3.actual_height
              << "  ║  " << std::setw(11) << std::fixed << std::setprecision(2) << result3.theoretical_max_height
              << "  ║  " << std::setw(28) << std::fixed << std::setprecision(4) << result3.memory_mb << "  ║" << std::endl;
    std::cout << "║  200,000   ║  " << std::setw(7) << result4.actual_height
              << "  ║  " << std::setw(11) << std::fixed << std::setprecision(2) << result4.theoretical_max_height
              << "  ║  " << std::setw(28) << std::fixed << std::setprecision(4) << result4.memory_mb << "  ║" << std::endl;
    std::cout << "╚════════════╩═══════════╩═══════════════╩════════════════════════════════╝" << std::endl;
    
    // Exportar a CSV
    export_space_benchmark_to_csv(results);
}

int main() {
    // Habilitar UTF-8 en consola de Windows
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    RB_tree<int> tree;
    int opcion;
    int valor;

    while (true) {
        print_menu();
        std::cin >> opcion;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "\nOpción inválida. Intente nuevamente.\n" << std::endl;
            continue;
        }

        switch (opcion) {
            case 1:
                std::cout << "Ingrese el valor a insertar: ";
                std::cin >> valor;
                tree.add_leaf(valor);
                std::cout << "Elemento " << valor << " insertado correctamente." << std::endl;
                break;

            case 2:
                std::cout << "\nIngrese el valor a buscar: ";
                std::cin >> valor;
                if (tree.find(valor)) {
                    std::cout << "El elemento " << valor << " SÍ existe en el árbol." << std::endl;
                } else {
                    std::cout << "El elemento " << valor << " NO existe en el árbol." << std::endl;
                }
                break;

            case 3:
                std::cout << "\nIngrese el valor para buscar su predecesor: ";
                std::cin >> valor;
                try {
                    int pred = tree.predecesor(valor);
                    std::cout << "El predecesor de " << valor << " es: " << pred << std::endl;
                } catch (const std::runtime_error& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                break;

            case 4:
                std::cout << "\nIngrese el valor para buscar su sucesor: ";
                std::cin >> valor;
                try {
                    int suc = tree.sucesor(valor);
                    std::cout << "El sucesor de " << valor << " es: " << suc << std::endl;
                } catch (const std::runtime_error& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                break;

            case 5:
                std::cout << "\nIngrese el valor a eliminar: ";
                std::cin >> valor;
                if (tree.delete_leaf(valor)) {
                    std::cout << "Elemento " << valor << " eliminado correctamente." << std::endl;
                } else {
                    std::cout << "El elemento " << valor << " no existe en el árbol." << std::endl;
                }
                break;

            case 6:
                std::cout << std::endl;
                tree.print_tree();
                break;

            case 7:
                run_all_time_benchmarks();
                break;

            case 8:
                run_all_comparison_benchmarks();
                break;

            case 9:
                run_all_space_benchmarks();
                break;

            case 10:
                run_cfs_simulation(true);
                break;

            case 11:
                run_cfs_simulation(false);
                break;

            case 0:
                std::cout << "\n¡Hasta luego!\n" << std::endl;
                return 0;

            default:
                std::cout << "\nOpción inválida. Intente nuevamente.\n" << std::endl;
        }

        std::cout << "\nPresione Enter para continuar...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::string _tmp;
        std::getline(std::cin, _tmp);
    }

    return 0;
}