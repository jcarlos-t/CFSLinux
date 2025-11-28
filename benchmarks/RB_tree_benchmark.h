#ifndef RB_TREE_BENCHMARK_H
#define RB_TREE_BENCHMARK_H

#include <iostream>
#include <algorithm>
#include <cmath>

template<typename T>
struct Node_benchmark {
    Node_benchmark* left;
    Node_benchmark* right;
    Node_benchmark* parent;
    T key;
    bool color;
    
    Node_benchmark(T k) {
        left = nullptr;
        right = nullptr;
        parent = nullptr;
        key = k;
        color = true;
    }
    
    ~Node_benchmark() {
        if (left) delete left;
        if (right) delete right;
    }
};

template<typename T>
class RB_tree_benchmark {
private:
    Node_benchmark<T>* root;
    long long comparison_count;
    int node_count;
    
    void left_rotation(Node_benchmark<T>* x) {
        Node_benchmark<T>* y = x->right;
        x->right = y->left;
        if (y->left != nullptr)
            y->left->parent = x;
        y->parent = x->parent;
        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->left = x;
        x->parent = y;
    }
    
    void right_rotation(Node_benchmark<T>* y) {
        Node_benchmark<T>* x = y->left;
        y->left = x->right;
        if (x->right != nullptr)
            x->right->parent = y;
        x->parent = y->parent;
        if (y->parent == nullptr)
            root = x;
        else if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;
        x->right = y;
        y->parent = x;
    }
    
    // Complejidad: O(log n) - proceso de fix-up en dos fases: inserción inicial + verificación ascendente
    // Recorre la altura del árbol. Número de rotaciones constante (máximo 2)
    void add_leaf_fixup(Node_benchmark<T>* k) {
        while (k != root && k->parent->color == true) {
            if (k->parent == k->parent->parent->left) {
                Node_benchmark<T>* u = k->parent->parent->right;
                if (u != nullptr && u->color == true) {
                    // Caso 1: Padre y tío son rojos
                    // Recolorear padre y tío a negro, abuelo a rojo, continuar desde abuelo
                    k->parent->color = false;
                    u->color = false;
                    k->parent->parent->color = true;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->right) {
                        // Caso 2: Padre rojo, tío negro, k es hijo derecho
                        // Rotación izquierda sobre padre para convertir en Caso 3
                        k = k->parent;
                        left_rotation(k);
                    }
                    // Caso 3: Padre rojo, tío negro, k es hijo izquierdo
                    // Rotación derecha sobre abuelo y recolorear
                    k->parent->color = false;
                    k->parent->parent->color = true;
                    right_rotation(k->parent->parent);
                }
            } else {
                Node_benchmark<T>* u = k->parent->parent->left;
                if (u != nullptr && u->color == true) {
                    // Caso 1: Padre y tío son rojos (simétrico)
                    k->parent->color = false;
                    u->color = false;
                    k->parent->parent->color = true;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->left) {
                        // Caso 2: Padre rojo, tío negro, k es hijo izquierdo (simétrico)
                        // Rotación derecha sobre padre para convertir en Caso 3
                        k = k->parent;
                        right_rotation(k);
                    }
                    // Caso 3: Padre rojo, tío negro, k es hijo derecho (simétrico)
                    // Rotación izquierda sobre abuelo y recolorear
                    k->parent->color = false;
                    k->parent->parent->color = true;
                    left_rotation(k->parent->parent);
                }
            }
        }
        root->color = false;
    }
    
    void delete_leaf_fixup(Node_benchmark<T>* x, Node_benchmark<T>* x_parent) {
        while (x != root && (x == nullptr || x->color == false)) {
            if (x == x_parent->left) {
                Node_benchmark<T>* w = x_parent->right;
                if (w->color == true) {
                    w->color = false;
                    x_parent->color = true;
                    left_rotation(x_parent);
                    w = x_parent->right;
                }
                if ((w->left == nullptr || w->left->color == false) &&
                    (w->right == nullptr || w->right->color == false)) {
                    w->color = true;
                    x = x_parent;
                    x_parent = x->parent;
                } else {
                    if (w->right == nullptr || w->right->color == false) {
                        if (w->left) w->left->color = false;
                        w->color = true;
                        right_rotation(w);
                        w = x_parent->right;
                    }
                    w->color = x_parent->color;
                    x_parent->color = false;
                    if (w->right) w->right->color = false;
                    left_rotation(x_parent);
                    x = root;
                }
            } else {
                Node_benchmark<T>* w = x_parent->left;
                if (w->color == true) {
                    w->color = false;
                    x_parent->color = true;
                    right_rotation(x_parent);
                    w = x_parent->left;
                }
                if ((w->right == nullptr || w->right->color == false) &&
                    (w->left == nullptr || w->left->color == false)) {
                    w->color = true;
                    x = x_parent;
                    x_parent = x->parent;
                } else {
                    if (w->left == nullptr || w->left->color == false) {
                        if (w->right) w->right->color = false;
                        w->color = true;
                        left_rotation(w);
                        w = x_parent->left;
                    }
                    w->color = x_parent->color;
                    x_parent->color = false;
                    if (w->left) w->left->color = false;
                    right_rotation(x_parent);
                    x = root;
                }
            }
        }
        if (x) x->color = false;
    }
    
    Node_benchmark<T>* minimum(Node_benchmark<T>* node) {
        while (node->left != nullptr) {
            comparison_count++;
            node = node->left;
        }
        comparison_count++;
        return node;
    }
    
    Node_benchmark<T>* find_node(T key) {
        Node_benchmark<T>* current = root;
        while (current != nullptr) {
            comparison_count++;
            if (key == current->key)
                return current;
            comparison_count++;
            if (key < current->key)
                current = current->left;
            else
                current = current->right;
        }
        comparison_count++;
        return nullptr;
    }
    
    void transplant(Node_benchmark<T>* u, Node_benchmark<T>* v) {
        if (u->parent == nullptr)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        if (v != nullptr)
            v->parent = u->parent;
    }
    
    int calculate_height(Node_benchmark<T>* node) {
        if (node == nullptr)
            return 0;
        int left_height = calculate_height(node->left);
        int right_height = calculate_height(node->right);
        return 1 + std::max(left_height, right_height);
    }

public:
    RB_tree_benchmark() {
        root = nullptr;
        comparison_count = 0;
        node_count = 0;
    }
    
    ~RB_tree_benchmark() {
        if (root) delete root;
    }
    
    void add_leaf(T key) {
        Node_benchmark<T>* new_node = new Node_benchmark<T>(key);
        Node_benchmark<T>* parent = nullptr;
        Node_benchmark<T>* current = root;
        while (current != nullptr) {
            comparison_count++;
            parent = current;
            comparison_count++;
            if (new_node->key < current->key)
                current = current->left;
            else
                current = current->right;
        }
        comparison_count++;
        new_node->parent = parent;
        if (parent == nullptr) {
            root = new_node;
        } else {
            comparison_count++;
            if (new_node->key < parent->key) {
                parent->left = new_node;
            } else {
                parent->right = new_node;
            }
        }
        node_count++;
        add_leaf_fixup(new_node);
    }
    
    bool delete_leaf(T key) {
        Node_benchmark<T>* z = find_node(key);
        if (z == nullptr) return false;
        Node_benchmark<T>* y = z;
        Node_benchmark<T>* x;
        Node_benchmark<T>* x_parent;
        bool y_original_color = y->color;
        if (z->left == nullptr) {
            x = z->right;
            x_parent = z->parent;
            transplant(z, z->right);
        } else if (z->right == nullptr) {
            x = z->left;
            x_parent = z->parent;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->parent == z) {
                x_parent = y;
            } else {
                x_parent = y->parent;
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        z->left = nullptr;
        z->right = nullptr;
        delete z;
        node_count--;
        if (y_original_color == false)
            delete_leaf_fixup(x, x_parent);
        return true;
    }
    
    bool find(T key) {
        return find_node(key) != nullptr;
    }
    long long get_comparisons() const {
        return comparison_count;
    }
    
    void reset_comparisons() {
        comparison_count = 0;
    }
    int get_height() {
        return calculate_height(root);
    }
    
    int get_node_count() const {
        return node_count;
    }
    
    size_t get_memory_usage() const {
        return node_count * sizeof(Node_benchmark<T>);
    }
    
    double get_theoretical_max_height() const {
        if (node_count == 0)
            return 0.0;
        return 2.0 * std::log2(node_count + 1);
    }
};

#endif // RB_TREE_BENCHMARK_H
