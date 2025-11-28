#ifndef CFS_SCHEDULER
#define CFS_SCHEDULER

#include "RB_tree.h"
#include "Process.h"
#include <vector>
#include <cmath>
#include <algorithm>
/* 
Formulas:

- Tiempo del CPU destinado a ejecutar un proceso: 
time_slice(p) = scheduling_period * (peso(p)/peso_total);
scheduling_period -> tiempo que el CFS divide entre los procesos. Para este caso es fijo
                    pero en implementaciones reales es variable y depende de la latencia

- Peso de un proceso:
weight = 1024 * 1.25^(-nice)

- Vruntime de un proceso:
vruntime += (delta_exec × NICE_0_LOAD) / peso_del_proceso
delta_exec -> tiempo de ejecucion del proceso
NICE_0_LOAD -> constante que representa el peso de un proceso con nice = 0
*/

class CFS_Scheduler {
private:
    RB_tree<Process> rbtree; //rbtree de procesos
    Process* current_process;
    double global_time; // tiempo de total de la simulacion
    double min_vruntime; // menor vruntime
    double cpu_time; // Tiempo total de ejecucion del CPU
    double current_slice_remaining; // Tiempo de CPU que le queda al proceso en ejecucion en este slicing
    const double scheduling_period = 24.0;
    int process_count = 0; // Cantidad de procesos en el CFS
    double total_weight = 0.0; // Weight total acumulado de todos los procesos
    
    // Lista de weight factors para el nice value [-20, 19]
    const double nice_weights[40] = {
        88761, 71755, 56483, 46273, 36291, // -20 to -16
        29154, 23254, 18705, 14949, 11916, // -15 to -11
        9548, 7620, 6100, 4904, 3906,      // -10 to -6
        3121, 2501, 1991, 1586, 1277,      // -5 to -1
        1024, 820, 655, 526, 423,          // 0 to 4
        335, 272, 215, 172, 137,           // 5 to 9
        110, 87, 70, 56, 45,               // 10 to 14
        36, 29, 23, 18, 15                 // 15 to 19
    };

    double get_weight(int nice) {
        if (nice < -20) nice = -20;
        if (nice > 19) nice = 19;
        return nice_weights[nice + 20];
    }

    // Calcula el time slice
    double calculate_time_slice(Process* process) {
        if (process == nullptr || total_weight <= 0) return scheduling_period;
        
        // time_slice(p) = scheduling_period * (peso(p) / peso_total)
        double process_weight = get_weight(process->nice);
        return scheduling_period * (process_weight / total_weight);
    }

public:
    CFS_Scheduler() : current_process(nullptr), global_time(0), min_vruntime(0), cpu_time(0), current_slice_remaining(0) {}

    void add_process(int pid, int nice, double burst_time, sf::Color color) {
        Process p(pid, nice, burst_time, global_time, color);
        p.vruntime = min_vruntime;
        rbtree.add_leaf(p);
        process_count++;
        total_weight += get_weight(nice); // Actualizar peso total
    }

    void tick(double time_step) {
        global_time += time_step;

        // si hay un proceso activo
        if (current_process) {
            cpu_time += time_step;
            current_process->exec_time += time_step;
            current_process->remaining_time = current_process->burst_time - current_process->exec_time;
            current_slice_remaining -= time_step;
            
            // Actualizacion de vruntime
            // vruntime += (exec_time * 1024) / weight
            double weight = get_weight(current_process->nice);
            double delta_vruntime = (time_step * 1024.0) / weight;
            current_process->vruntime += delta_vruntime;

            // Verificacion de finalizacion del proceso
            if (current_process->exec_time >= current_process->burst_time) {
                std::cout << "Process " << current_process->pid << " finished." << std::endl;
                total_weight -= get_weight(current_process->nice);
                delete current_process;
                current_process = nullptr;
                current_slice_remaining = 0;
                process_count--;
            } else if (current_slice_remaining <= 0) {
                // No ha termina y aun le queda burst_time
                rbtree.add_leaf(*current_process);
                delete current_process;
                current_process = nullptr;
                current_slice_remaining = 0;
            } else {
                // preemption si hay un vruntime menor -> reeemplazar con el proceso de menor vruntime
                Node<Process>* min_node = rbtree.get_min_node();
                if (min_node && min_node->key.vruntime < current_process->vruntime) {
                    rbtree.add_leaf(*current_process);
                    delete current_process;
                    current_process = nullptr;
                    current_slice_remaining = 0;
                }
            }
        }

        // Seleccionar el siguiente proceso
        if (!current_process) {
            Node<Process>* min_node = rbtree.get_min_node();
            if (min_node) {
                current_process = new Process(min_node->key);
                rbtree.delete_leaf(min_node->key);
                if (current_process->vruntime > min_vruntime) {
                    min_vruntime = current_process->vruntime;
                }
                // Nuevo time slice para el proceso
                current_slice_remaining = calculate_time_slice(current_process);
            }
        }
    }

    Process* get_current_process() const {
        return current_process;
    }

    Node<Process>* get_tree_root() const {
        return rbtree.get_root();
    }
    
    double get_global_time() const {
        return global_time;
    }

    double get_cpu_time() const {
        return cpu_time;
    }

    double get_current_slice_remaining() const {
        return current_slice_remaining;
    }
};

#endif // CFS_SCHEDULER
