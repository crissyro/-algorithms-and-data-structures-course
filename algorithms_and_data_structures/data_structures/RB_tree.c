#include <stdio.h>
#include <stdlib.h>

/**
 * @file red_black_tree.c
 * @brief Реализация красно-чёрного дерева на C.
 */

/** @enum NodeColor
 *  @brief Цвет узла: красный или чёрный.
 */
typedef enum { RED, BLACK } NodeColor;

typedef struct RBNode {
    int data;                  /**< Значение узла */
    NodeColor color;           /**< Цвет узла */
    struct RBNode* left;       /**< Левый потомок */
    struct RBNode* right;      /**< Правый потомок */
    struct RBNode* parent;     /**< Родительский узел */
} RBNode;

/**
 * @brief Создаёт новый узел красно-чёрного дерева.
 * @param data Значение нового узла.
 * @return Указатель на созданный узел.
 */
RBNode* create_node(int data) {
    RBNode* node = (RBNode*)malloc(sizeof(RBNode));
    
    node->data = data;
    node->color = RED; // Новый узел всегда красный
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;

    return node;
}

/**
 * @brief Выполняет левый поворот вокруг узла x.
 * @param root Указатель на корень дерева.
 * @param x Узел, вокруг которого выполняется поворот.
 * @return Новый корень дерева.
 */
RBNode* left_rotate(RBNode* root, RBNode* x) {
    RBNode* y = x->right;

    x->right = y->left;

    if (y->left) y->left->parent = x;
    
    y->parent = x->parent;

    if (!x->parent) root = y;

    else if (x == x->parent->left) x->parent->left = y;

    else x->parent->right = y;
    
    y->left = x;
    x->parent = y;

    return root;
}

/**
 * @brief Выполняет правый поворот вокруг узла y.
 * @param root Указатель на корень дерева.
 * @param y Узел, вокруг которого выполняется поворот.
 * @return Новый корень дерева.
 */
RBNode* right_rotate(RBNode* root, RBNode* y) {
    RBNode* x = y->left;
    y->left = x->right;

    if (x->right) x->right->parent = y;
    
    x->parent = y->parent;

    if (!y->parent) root = x;

    else if (y == y->parent->left) y->parent->left = x;

    else y->parent->right = x;
    
    x->right = y;
    y->parent = x;

    return root;
}

/**
 * @brief Исправляет дерево после вставки нового узла.
 * @param root Указатель на корень дерева.
 * @param node Указатель на вставленный узел.
 * @return Новый корень дерева.
 */
RBNode* fix_insert(RBNode* root, RBNode* node) {
    while (node != root && node->parent->color == RED) {
        if (node->parent == node->parent->parent->left) {
            RBNode* uncle = node->parent->parent->right;
            if (uncle && uncle->color == RED) {
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    root = left_rotate(root, node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                root = right_rotate(root, node->parent->parent);
            }
        } else {
            RBNode* uncle = node->parent->parent->left;
            if (uncle && uncle->color == RED) {
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    root = right_rotate(root, node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                root = left_rotate(root, node->parent->parent);
            }
        }
    }

    root->color = BLACK;
    return root;
}

/**
 * @brief Вставляет новый узел в красно-чёрное дерево.
 * @param root Указатель на корень дерева.
 * @param data Значение нового узла.
 * @return Новый корень дерева.
 */
RBNode* insert(RBNode* root, int data) {
    RBNode* node = create_node(data);
    if (!root) {
        node->color = BLACK;
        return node;
    }

    RBNode* parent = NULL;
    RBNode* current = root;

    while (current) {
        parent = current;
        if (data < current->data) {
            current = current->left;
        } else if (data > current->data) {
            current = current->right;
        } else {
            free(node);
            return root;
        }
    }

    node->parent = parent;
    if (data < parent->data) {
        parent->left = node;
    } else {
        parent->right = node;
    }

    return fix_insert(root, node);
}

/**
 * @brief Выполняет обход дерева в порядке возрастания (inorder).
 * @param root Указатель на корень дерева.
 */
void inorder_traversal(RBNode* root) {
    if (root) {
        inorder_traversal(root->left);
        printf("%d (%s) ", root->data, root->color == RED ? "RED" : "BLACK");
        inorder_traversal(root->right);
    }
}

/**
 * @brief Освобождает память, выделенную для дерева.
 * @param root Указатель на корень дерева.
 */
void free_tree(RBNode* root) {
    if (root) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

int main() {
    RBNode* tree = NULL;

    tree = insert(tree, 10);
    tree = insert(tree, 20);
    tree = insert(tree, 30);
    tree = insert(tree, 15);

    printf("Inorder traversal:\n");
    inorder_traversal(tree);
    printf("\n");

    free_tree(tree);
    return 0;
}