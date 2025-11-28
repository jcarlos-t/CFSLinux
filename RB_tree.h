#ifndef RB_TREE_H
#define RB_TREE_H
#include <iostream>
template<typename T>
struct Node {
    Node* left;
    Node* right;
    Node* parent;
    T key;
    bool color; // false = black, true = red
    Node(T k) {
        left = nullptr;
        right = nullptr;
        parent = nullptr;
        key = k;
        color = true; // nuevo nodo siempre rojo
    }
    ~Node() {
        if (left) delete left;
        if (right) delete right;
    }
};
template<typename T>
class RB_tree {
private:
    Node<T>* root;

    // Complejidad: O(1) - operaciones de punteros constantes
    void left_rotation(Node<T>* x) {
        Node<T>* y = x->right;
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

    // Complejidad: O(1) - operaciones de punteros constantes
    void right_rotation(Node<T>* y) {
        Node<T>* x = y->left;
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

    // Complejidad: O(log n) - recorre la altura del árbol que está balanceado
    // El número de rotaciones es constante (máximo 2), pero el recorrido hacia arriba es O(log n)
    // Proceso de fix-up en dos fases: inserción inicial + verificación ascendente
    void add_leaf_fixup(Node<T>* k) {
        while (k != root && k->parent->color == true) {
            if (k->parent == k->parent->parent->left) {
                Node<T>* u = k->parent->parent->right; // tío
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
                Node<T>* u = k->parent->parent->left; // tío
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

    // Complejidad: O(log n) - similar a add_leaf_fixup, recorre la altura del árbol
    void delete_leaf_fixup(Node<T>* x, Node<T>* x_parent) {
        while (x != root && (x == nullptr || x->color == false)) {
            if (x == x_parent->left) {
                Node<T>* w = x_parent->right;
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
                Node<T>* w = x_parent->left;
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

    // Complejidad: O(log n) - recorre desde un nodo hasta el mínimo de su subárbol
    // En el peor caso, recorre toda la altura del subárbol
    Node<T>* minimum(Node<T>* node) {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    // Complejidad: O(log n) - recorre desde un nodo hasta el máximo de su subárbol
    Node<T>* maximum(Node<T>* node) {
        while (node->right != nullptr)
            node = node->right;
        return node;
    }

    // Complejidad: O(log n) - búsqueda binaria en árbol balanceado
    Node<T>* find_node(T key) {
        Node<T>* current = root;
        while (current != nullptr) {
            if (key == current->key)
                return current;
            else if (key < current->key)
                current = current->left;
            else
                current = current->right;
        }
        return nullptr;
    }

    // Complejidad: O(1) - solo actualiza punteros
    void transplant(Node<T>* u, Node<T>* v) {
        if (u->parent == nullptr)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        if (v != nullptr)
            v->parent = u->parent;
    }

    // Complejidad: O(n) - visita cada nodo exactamente una vez
    void print_inorder(Node<T>* node) {
        if (node == nullptr) return;
        print_inorder(node->left);
        std::cout << node->key << (node->color ? "(R)" : "(B)") << " ";
        print_inorder(node->right);
    }

    // Complejidad: O(n) - visita cada nodo exactamente una vez
    void print_preorder(Node<T>* node, std::string prefix = "", bool is_left = false, bool is_root = true) {
        if (node == nullptr) return;

        std::cout << prefix;

        if (!is_root) {
            std::cout << (is_left ? "├──L: " : "└──R: ");
        }

        std::cout << node->key << (node->color ? "(R)" : "(B)") << std::endl;

        if (node->left != nullptr || node->right != nullptr) {
            std::string new_prefix = prefix;
            if (!is_root) {
                new_prefix += (is_left ? "│   " : "    ");
            }

            if (node->left != nullptr) {
                print_preorder(node->left, new_prefix, true, false);
            }
            if (node->right != nullptr) {
                print_preorder(node->right, new_prefix, false, false);
            }
        }
    }
public:
    // Complejidad: O(1) - inicialización simple
    RB_tree() {
        root = nullptr;
    }

    // Complejidad: O(1) - creación de un solo nodo
    RB_tree(T key) {
        root = new Node<T>(key);
        root->color = false; // Raíz siempre negra
    }

    // Complejidad: O(n) - el destructor de Node elimina recursivamente todos los nodos
    ~RB_tree() {
        if (root) delete root;
    }

    // Complejidad: O(log n) - búsqueda O(log n) + fixup O(log n) = O(log n)
    // Garantiza balanceo del árbol después de la inserción
    void add_leaf(T key) {
        Node<T>* new_node = new Node<T>(key);
        Node<T>* parent = nullptr;
        Node<T>* current = root;
        // Búsqueda de posición: O(log n)
        while (current != nullptr) {
            parent = current;
            if (new_node->key < current->key)
                current = current->left;
            else
                current = current->right;
        }
        new_node->parent = parent;
        if (parent == nullptr) {
            root = new_node;
        } else if (new_node->key < parent->key) {
            parent->left = new_node;
        } else {
            parent->right = new_node;
        }
        // Rebalanceo: O(log n)
        add_leaf_fixup(new_node);
    }

    // Complejidad: O(log n) - búsqueda O(log n) + eliminación y fixup O(log n) = O(log n)
    // Incluye encontrar el nodo, transplantarlo y rebalancear el árbol
    bool delete_leaf(T key) {
        Node<T>* z = find_node(key); // O(log n)
        if (z == nullptr) return false;
        Node<T>* y = z;
        Node<T>* x;
        Node<T>* x_parent;
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
            y = minimum(z->right); // O(log n)
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
        if (y_original_color == false)
            delete_leaf_fixup(x, x_parent); // O(log n)
        return true;
    }

    // Complejidad: O(log n) - búsqueda binaria en árbol balanceado
    bool find(T key) {
        return find_node(key) != nullptr;
    }

    // Complejidad: O(log n) - búsqueda del nodo O(log n) + encontrar mínimo O(log n) = O(log n)
    // o recorrido hacia arriba O(log n)
    T sucesor(T key) {
        Node<T>* node = find_node(key);
        if (node == nullptr) {
            throw std::runtime_error("Nodo no encontrado");
        }
        if (node->right != nullptr)
            return minimum(node->right)->key;
        Node<T>* parent = node->parent;
        while (parent != nullptr && node == parent->right) {
            node = parent;
            parent = parent->parent;
        }
        if (parent == nullptr)
            throw std::runtime_error("No tiene sucesor");
        return parent->key;
    }

    // Complejidad: O(log n) - búsqueda del nodo O(log n) + encontrar máximo O(log n) = O(log n)
    // o recorrido hacia arriba O(log n)
    T predecesor(T key) {
        Node<T>* node = find_node(key);
        if (node == nullptr) {
            throw std::runtime_error("Nodo no encontrado");
        }
        if (node->left != nullptr)
            return maximum(node->left)->key;
        Node<T>* parent = node->parent;
        while (parent != nullptr && node == parent->left) {
            node = parent;
            parent = parent->parent;
        }
        if (parent == nullptr)
            throw std::runtime_error("No tiene predecesor");
        return parent->key;
    }

    // Complejidad: O(n) - recorre todos los nodos del árbol para imprimirlos
    void print_tree() {
        if (root == nullptr) {
            std::cout << "Árbol vacío" << std::endl;
            return;
        }
        std::cout << "Estructura del árbol:" << std::endl;
        print_preorder(root);
        std::cout << "\nRecorrido inorden: ";
        print_inorder(root);
        std::cout << std::endl;
    }

    Node<T>* get_root() const {
        return root;
    }

    Node<T>* get_min_node() {
        if (root == nullptr) return nullptr;
        return minimum(root);
    }
};
#endif // RB_TREE_H