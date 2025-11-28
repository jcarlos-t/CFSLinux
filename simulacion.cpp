#include <iostream>
#include "Visualizer.h"
#include <thread>
#include <chrono>
#include <optional>

void run_cfs_simulation(bool preconfigured) {
    sf::RenderWindow window(sf::VideoMode(1600, 900), "CFS Simulation");
    CFS_Scheduler scheduler;
    Visualizer visualizer(window);
    bool paused = false;
    float time_scale = 400.0f; // 1ms CFS = 100ms real
    
    // Inputs
    std::string input_pid_str = "";
    std::string input_nice_str = "";
    std::string input_burst_str = "";
    std::string input_time_scale_str = "";
    
    enum InputMode { NONE, PID_MODE, NICE_MODE, BURST_MODE, TIME_SCALE_MODE };
    InputMode current_mode = NONE;
    
    // Mouse tracking para el selector
    sf::Vector2f mouse_pos;
    bool mouse_pressed = false;
    bool mouse_clicked = false;

    if (preconfigured) {
        // Procesos para demo
        scheduler.add_process(101, 0, 50, sf::Color::Red);
        scheduler.add_process(102, 5, 30, sf::Color::Blue);
        scheduler.add_process(103, -5, 40, sf::Color::Green);
        scheduler.add_process(104, 10, 20, sf::Color::Yellow);
        scheduler.add_process(105, -10, 60, sf::Color::Magenta);
    } else {
        // Input de processo
        std::cout << "Enter number of processes: ";
        int n;
        std::cin >> n;
        for (int i = 0; i < n; ++i) {
            int pid, nice;
            double burst;
            std::cout << "Process " << i + 1 << " (PID Nice Burst): ";
            std::cin >> pid >> nice >> burst;
            sf::Color color(rand() % 255, rand() % 255, rand() % 255);
            scheduler.add_process(pid, nice, burst, color);
        }
    }

    while (window.isOpen()) {
        mouse_clicked = false;
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed) {
                // P -> control de pausa
                if (event.key.code == sf::Keyboard::P && current_mode == NONE) {
                    paused = !paused;
                }
                // Q -> salir de la ventana
                else if (event.key.code == sf::Keyboard::Q && current_mode == NONE) {
                    window.close();
                }
                // T -> Time Scale Input
                else if (event.key.code == sf::Keyboard::T) {
                    if (current_mode == TIME_SCALE_MODE) {
                        // Aplicar escala de tiempo
                        if (!input_time_scale_str.empty()) {
                            try {
                                time_scale = std::stof(input_time_scale_str);
                                if (time_scale < 1.0f) time_scale = 1.0f; // Minimum 1ms
                            } catch (...) {
                                // input invalido
                            }
                        }
                        input_time_scale_str = "";
                        current_mode = NONE;
                    } else if (current_mode == NONE) {
                        current_mode = TIME_SCALE_MODE;
                        input_time_scale_str = "";
                    }
                }
                // I -> insertar pid
                else if (event.key.code == sf::Keyboard::I) {
                    if (current_mode == PID_MODE) {
                        input_pid_str = "";
                        current_mode = NONE;
                    } else {
                        current_mode = PID_MODE;
                    }
                }
                // N -> insertar nice value
                else if (event.key.code == sf::Keyboard::N) {
                    if (current_mode == NICE_MODE) {
                        input_nice_str = "";
                        current_mode = NONE;
                    } else {
                        current_mode = NICE_MODE;
                    }
                }
                // B -> insertar burst_time
                else if (event.key.code == sf::Keyboard::B) {
                    if (current_mode == BURST_MODE) {
                        input_burst_str = "";
                        current_mode = NONE;
                    } else {
                        current_mode = BURST_MODE;
                    }
                }
                // Mapeo y formateo de inputs
                else if (current_mode != NONE) {
                    if (event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9) {
                        char digit = '0' + (event.key.code - sf::Keyboard::Num0);
                        if (current_mode == PID_MODE) input_pid_str += digit;
                        else if (current_mode == NICE_MODE) input_nice_str += digit;
                        else if (current_mode == BURST_MODE) input_burst_str += digit;
                        else if (current_mode == TIME_SCALE_MODE) input_time_scale_str += digit;
                    }
                    // Permitir - para nice value
                    else if (current_mode == NICE_MODE && event.key.code == sf::Keyboard::Hyphen && input_nice_str.empty()) {
                        input_nice_str = "-";
                    }
                    // Backspace para eliminar caracteres
                    else if (event.key.code == sf::Keyboard::Backspace) {
                        if (current_mode == PID_MODE && !input_pid_str.empty()) input_pid_str.pop_back();
                        else if (current_mode == NICE_MODE && !input_nice_str.empty()) input_nice_str.pop_back();
                        else if (current_mode == BURST_MODE && !input_burst_str.empty()) input_burst_str.pop_back();
                        else if (current_mode == TIME_SCALE_MODE && !input_time_scale_str.empty()) input_time_scale_str.pop_back();
                    }
                    // Enter para salir de modo input
                    else if (event.key.code == sf::Keyboard::Return) {
                        if (current_mode == TIME_SCALE_MODE) {
                             if (!input_time_scale_str.empty()) {
                                try {
                                    time_scale = std::stof(input_time_scale_str);
                                    if (time_scale < 1.0f) time_scale = 1.0f;
                                } catch (...) {}
                            }
                            input_time_scale_str = "";
                        }
                        current_mode = NONE;
                    }
                }
                // Crear proceso (fuera de input mode)
                else if (event.key.code == sf::Keyboard::Return && current_mode == NONE) {
                    int pid = input_pid_str.empty() ? 100 : std::stoi(input_pid_str);
                    int nice = input_nice_str.empty() ? 0 : std::stoi(input_nice_str);
                    double burst = input_burst_str.empty() ? 20.0 : std::stod(input_burst_str);
                    
                    if (nice < -20) nice = -20;
                    if (nice > 19) nice = 19;
                    
                    // Agregar proceso
                    sf::Color color(rand() % 200 + 55, rand() % 200 + 55, rand() % 200 + 55);
                    scheduler.add_process(pid, nice, burst, color);
                    std::cout << "Added Process " << pid << " (Nice: " << nice << ", Burst: " << burst << ")" << std::endl;
                    
                    // Reset inputs
                    input_pid_str = "";
                    input_nice_str = "";
                    input_burst_str = "";
                }
            }
            else if (event.type == sf::Event::MouseMoved) {
                mouse_pos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                mouse_pressed = true;
                mouse_clicked = true;
            }
            else if (event.type == sf::Event::MouseButtonReleased) {
                mouse_pressed = false;
            }
        }

        if (!paused) {
            scheduler.tick(1.0);
        }
        visualizer.render(scheduler, paused, time_scale, mouse_pos, mouse_pressed, mouse_clicked, 
                         input_pid_str, input_nice_str, input_burst_str, current_mode,
                         current_mode == TIME_SCALE_MODE, input_time_scale_str);

        // Delay en base a la escala para visualizacion : 1ms CFS = time_scale ms real
        std::this_thread::sleep_for(std::chrono::milliseconds((int)time_scale));
    }
}