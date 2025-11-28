#ifndef VISUALIZER
#define VISUALIZER

#include <SFML/Graphics.hpp>
#include "CFS_Scheduler.h"
#include <string>
#include <sstream>

class Visualizer {
private:
    sf::RenderWindow& window;
    sf::Font font;
    float node_radius = 40.0f; // radio de los nodos
    float vertical_gap = 100.0f; // distancia entre niveles

    void draw_node(Node<Process>* node, float x, float y, float horizontal_gap) {
        if (!node) return;

        // Dibujo de conecciones
        if (node->left) {
            sf::Vertex line[] = {
                sf::Vertex{sf::Vector2f(x, y), sf::Color::White},
                sf::Vertex{sf::Vector2f(x - horizontal_gap, y + vertical_gap), sf::Color::White}
            };
            window.draw(line, 2, sf::PrimitiveType::Lines);
            draw_node(node->left, x - horizontal_gap, y + vertical_gap, horizontal_gap / 2);
        }
        if (node->right) {
            sf::Vertex line[] = {
                sf::Vertex{sf::Vector2f(x, y), sf::Color::White},
                sf::Vertex{sf::Vector2f(x + horizontal_gap, y + vertical_gap), sf::Color::White}
            };
            window.draw(line, 2, sf::PrimitiveType::Lines);
            draw_node(node->right, x + horizontal_gap, y + vertical_gap, horizontal_gap / 2);
        }

        // Dibujo del nodo
        sf::CircleShape circle(node_radius);
        circle.setPosition({x - node_radius, y - node_radius});
        circle.setFillColor(node->color ? sf::Color::Red : sf::Color::Black);
        circle.setOutlineThickness(2);
        circle.setOutlineColor(sf::Color::White);
        window.draw(circle);

        // Texto en nodo: PID, VR, NV, BT
        sf::Text text;
        text.setFont(font);
        std::stringstream ss;
        ss << "PID=" << node->key.pid << "\n" 
           << "VR=" << (int)node->key.vruntime << "\n"
           << "NV=" << node->key.nice << "\n"
           << "BT=" << (int)node->key.burst_time;
        text.setString(ss.str());
        text.setCharacterSize(10);
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Bold); 
        
        // Centrar el texto
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(sf::Vector2f(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f));
        text.setPosition(sf::Vector2f(x, y));
        
        window.draw(text);
    }

    void draw_process_info(Process* p, float x, float y) {
        sf::RectangleShape box(sf::Vector2f(200, 120));
        box.setPosition(sf::Vector2f(x, y));
        box.setFillColor(sf::Color(50, 50, 50));
        box.setOutlineThickness(2);
        box.setOutlineColor(sf::Color::White);
        window.draw(box);

        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(14);
        text.setFillColor(sf::Color(43, 255, 10));
        text.setPosition(sf::Vector2f(x + 10, y + 10));
        text.setStyle(sf::Text::Bold); 

        if (p) {
            std::stringstream ss;
            ss << "RUNNING PROCESS\n";
            ss << "PID: " << p->pid << "\n";
            ss << "Nice value: " << p->nice << "\n";
            ss << "Vruntime: " << (int)p->vruntime << "\n";
            ss << "Burst: " << (int)p->burst_time << "\n";
            ss << "Rem time: " << (int)p->remaining_time;
            text.setString(ss.str());
        } else {
            text.setString("IDLE");
        }
        window.draw(text);
    }

    // Draw del CPU, dentro se llama a draw_process_info
    void draw_cpu(Process* p, float x, float y) {
        
        // CPU Box
        sf::RectangleShape cpuBox(sf::Vector2f(200, 120));
        cpuBox.setPosition(sf::Vector2f(x, y));
        cpuBox.setFillColor(sf::Color(50, 50, 50));
        cpuBox.setOutlineThickness(3);
        cpuBox.setOutlineColor(sf::Color(56, 247, 2));
        window.draw(cpuBox);

        // CPU Label
        sf::Text label;
        label.setFont(font);
        label.setString("CPU");
        label.setCharacterSize(20);
        label.setFillColor(sf::Color(56, 247, 2));
        label.setPosition(sf::Vector2f(x + x/2, y - 30));
        label.setStyle(sf::Text::Bold); 
        window.draw(label);


        if (p) {
            // Dibujar proceso dentro del CPU
            sf::CircleShape circle(30.0f);
            circle.setPosition(sf::Vector2f(x + 70, y + 30));
            circle.setFillColor(p->color);
            window.draw(circle);

            sf::Text pText;
            pText.setFont(font);
            std::stringstream ss;
            ss << "P" << p->pid;
            pText.setString(ss.str());
            pText.setCharacterSize(16);
            pText.setFillColor(sf::Color::Black);
            pText.setStyle(sf::Text::Bold);
            sf::FloatRect textRect = pText.getLocalBounds();
            pText.setOrigin(sf::Vector2f(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f));
            pText.setPosition(sf::Vector2f(x + 100, y + 50));
            window.draw(pText);
            
            // Detalles del proceso
            draw_process_info(p, x + 230, y);
        } else {
            sf::Text idleText;
            idleText.setFont(font);
            idleText.setString("IDLE");
            idleText.setCharacterSize(24);
            idleText.setFillColor(sf::Color(100, 100, 100));
            idleText.setPosition(sf::Vector2f(x + 70, y + 45));
            window.draw(idleText);
        }
    }

    void draw_controls(bool paused, float time_scale, bool input_active, const std::string& input_str) {
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::White);
        text.setPosition(sf::Vector2f(30.0f, 30.0f));
        
        std::stringstream ss;
        ss << "Controls:\n";
        ss << "[P] Pause/Resume (" << (paused ? "PAUSED" : "RUNNING") << ")\n";
        ss << "[T] Set Time Scale: ";
        
        if (input_active) {
            ss << input_str << "_";
        } else {
            ss << (int)time_scale << "ms";
        }
        
        text.setString(ss.str());
        window.draw(text);
    }

    struct ProcessInput {
        int pid = 0;
        int nice = 0;
        double burst_time = 0;
        bool submit = false;
    };



    void draw_process_input_panel(sf::Vector2f mouse_pos, bool mouse_clicked, 
                                   const std::string& pid_str, const std::string& nice_str, 
                                   const std::string& burst_str, int input_mode, float x, float y) {
        
        // Panel fondo
        sf::RectangleShape panel(sf::Vector2f(260, 180));
        panel.setPosition(sf::Vector2f(x, y));
        panel.setFillColor(sf::Color(40, 40, 40));
        panel.setOutlineThickness(2);
        panel.setOutlineColor(sf::Color::Blue);
        window.draw(panel);
        
        // Title
        sf::Text title;
        title.setFont(font);
        title.setString("Add Process");
        title.setCharacterSize(16);
        title.setFillColor(sf::Color::White);
        title.setPosition(sf::Vector2f(x + 40, y + 10));
        window.draw(title);
        
        // Inputs dinamicos
        sf::Text display;
        display.setFont(font);
        std::stringstream ss;
        
        // Mode names: 0=NONE, 1=PID_MODE, 2=NICE_MODE, 3=BURST_MODE
        const char* mode_indicator[] = {"", " <--", " <--", " <--"};
        sf::Color active_color = sf::Color::Yellow;
        sf::Color normal_color = sf::Color::White;
        
        ss << "Current Settings:\n\n";
        
        // PID field
        display.setString("PID:   ");
        display.setCharacterSize(14);
        display.setFillColor(input_mode == 1 ? active_color : normal_color);
        display.setPosition(sf::Vector2f(x + 20, y + 45));
        window.draw(display);
        
        display.setString(pid_str.empty() ? "100" : pid_str);
        display.setFillColor(sf::Color::Green);
        display.setPosition(sf::Vector2f(x + 80, y + 45));
        window.draw(display);
        
        if (input_mode == 1) {
            display.setString(" <-- [I]");
            display.setFillColor(active_color);
            display.setPosition(sf::Vector2f(x + 150, y + 45));
            window.draw(display);
        } else {
            display.setString("  [I]");
            display.setFillColor(sf::Color(150, 150, 150));
            display.setPosition(sf::Vector2f(x + 150, y + 45));
            window.draw(display);
        }
        
        // Nice field
        display.setString("Nice:  ");
        display.setCharacterSize(14);
        display.setFillColor(input_mode == 2 ? active_color : normal_color);
        display.setPosition(sf::Vector2f(x + 20, y + 75));
        window.draw(display);
        
        display.setString(nice_str.empty() ? "0" : nice_str);
        display.setFillColor(sf::Color::Green);
        display.setPosition(sf::Vector2f(x + 80, y + 75));
        window.draw(display);
        
        if (input_mode == 2) {
            display.setString(" <-- [N]");
            display.setFillColor(active_color);
            display.setPosition(sf::Vector2f(x + 150, y + 75));
            window.draw(display);
        } else {
            display.setString("  [N]");
            display.setFillColor(sf::Color(150, 150, 150));
            display.setPosition(sf::Vector2f(x + 150, y + 75));
            window.draw(display);
        }
        
        // Burst field
        display.setString("Burst: ");
        display.setCharacterSize(14);
        display.setFillColor(input_mode == 3 ? active_color : normal_color);
        display.setPosition(sf::Vector2f(x + 20, y + 105));
        window.draw(display);
        
        display.setString(burst_str.empty() ? "20" : burst_str);
        display.setFillColor(sf::Color::Green);
        display.setPosition(sf::Vector2f(x + 80, y + 105));
        window.draw(display);
        
        if (input_mode == 3) {
            display.setString(" <-- [B]");
            display.setFillColor(active_color);
            display.setPosition(sf::Vector2f(x + 150, y + 105));
            window.draw(display);
        } else {
            display.setString("  [B]");
            display.setFillColor(sf::Color(150, 150, 150));
            display.setPosition(sf::Vector2f(x + 150, y + 105));
            window.draw(display);
        }
        
        // Instructions
        display.setString("\n[ENTER] to Add / reset");
        display.setCharacterSize(14);
        display.setFillColor(sf::Color(180, 180, 180));
        display.setPosition(sf::Vector2f(x + 20, y + 140));
        window.draw(display);
    }

    void draw_timers(CFS_Scheduler& scheduler, float x, float y) {
        // Dibujar tiempo de CPU y tiempo del slice sobrante
        sf::Text timeText;
        timeText.setFont(font);
        timeText.setCharacterSize(18);
        timeText.setFillColor(sf::Color::White);
        timeText.setPosition(sf::Vector2f(x, y));
        std::stringstream ss;
        ss << "Virtual Time: " << std::fixed << std::setprecision(2) << scheduler.get_global_time() << "ms\n";
        ss << "Slice Remaining: " << std::fixed << std::setprecision(2) << scheduler.get_current_slice_remaining() << "ms";
        timeText.setString(ss.str());
        window.draw(timeText);
    }

public:
    Visualizer(sf::RenderWindow& w) : window(w) {
        // Cargar fuentes
        if (!font.loadFromFile("assets/font.ttf")) {
            std::cerr << "Failed to load font. Text will not be displayed." << std::endl;
        }
    }

    void render(CFS_Scheduler& scheduler, bool paused, float time_scale, sf::Vector2f mouse_pos, 
                bool mouse_pressed, bool mouse_clicked, const std::string& pid_str, 
                const std::string& nice_str, const std::string& burst_str, int input_mode,
                bool time_input_active, const std::string& time_input_str) {
        window.clear(sf::Color(20, 20, 20));

        draw_controls(paused, time_scale, time_input_active, time_input_str);
        
        draw_process_input_panel(mouse_pos, mouse_clicked, pid_str, nice_str, burst_str, input_mode, 
            window.getSize().x - 300.0f, window.getSize().y - 210.0f);

        // Draw Tree
        Node<Process>* root = scheduler.get_tree_root();
        if (root) {
            draw_node(root, window.getSize().x / 2.0f, 100.0f, window.getSize().x / 5.0f);
        }

        // Draw CPU and Info
        draw_cpu(scheduler.get_current_process(), 50.0f, window.getSize().y - 140.0f);

        draw_timers(scheduler, window.getSize().x - 250.0f, 50.0f);

        window.display();
    }
};

#endif // VISUALIZER