#ifndef PROCESS
#define PROCESS

#include <SFML/Graphics.hpp>
#include <iostream>

struct Process {
    int pid;
    int nice; // [-20, 19]
    double vruntime;
    double exec_time; // Tiempo de ejecución en el cpu
    double burst_time; // Tiempo total requerido para el proceso
    double remaining_time; // Tiempo que le falta para terminar todo su proceso
    sf::Color color;

    Process(int p, int n, double b, double a, sf::Color c) 
        : pid(p), nice(n), vruntime(0), exec_time(0), burst_time(b), remaining_time(b), color(c) {}

    Process() : pid(-1), nice(0), vruntime(0), exec_time(0), burst_time(0), remaining_time(0), color(sf::Color::White) {}

    // Comparacion de vruntime para ordenar en RBT
    bool operator<(const Process& other) const {
        if (vruntime != other.vruntime) {
            return vruntime < other.vruntime;
        }
        return pid < other.pid; // En caso tenga mismo vruntime
    }

    bool operator>(const Process& other) const {
        if (vruntime != other.vruntime) {
            return vruntime > other.vruntime;
        }
        return pid > other.pid;
    }

    bool operator==(const Process& other) const {
        return pid == other.pid;
    }

    bool operator!=(const Process& other) const {
        return pid != other.pid;
    }
    
    // print
    friend std::ostream& operator<<(std::ostream& os, const Process& p) {
        os << "P" << p.pid << "(v:" << p.vruntime << ")";
        return os;
    }
};

#endif // PROCESS
