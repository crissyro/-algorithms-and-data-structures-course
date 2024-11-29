#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Код ошибок
typedef enum {
    SUCCESS = 0,         /**< Операция выполнена успешно */
    ERR_MEMORY = 1,      /**< Ошибка выделения памяти */
    ERR_NOT_FOUND = 2,   /**< Элемент не найден */
    ERR_INVALID_INPUT = 3 /**< Некорректный ввод */
} ErrorCode;

// Узел AVL-дерева
typedef struct AVLNode {
    int data;               /**< Значение узла */
    int height;             /**< Высота узла */
    struct AVLNode* left;   /**< Левый потомок */
    struct AVLNode* right;  /**< Правый потомок */
} AVLNode;

/**
 * @brief Создает новый узел AVL-дерева.
 * @param data Значение для нового узла.
 * @return Указатель на созданный узел или NULL при ошибке выделения памяти.
 */
AVLNode* create_node(int data) {
    AVLNode* new_node = (AVLNode*)malloc(sizeof(AVLNode));

    if (!new_node) return NULL;
    
    new_node->data = data;
    new_node->height = 1; // Узел всегда создается с высотой 1
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

/**
 * @brief Возвращает высоту узла.
 * @param node Указатель на узел.
 * @return Высота узла или 0, если узел NULL.
 */
int height(AVLNode* node) { return node ? node->height : 0; }

/**
 * @brief Вычисляет баланс-фактор узла.
 * @param node Указатель на узел.
 * @return Баланс-фактор.
 */
int balance_factor(AVLNode* node) { 
    return node ? height(node->left) - height(node->right) : 0;
}

/**
 * @brief Выполняет правый поворот вокруг узла.
 * @param y Узел, вокруг которого выполняется поворот.
 * @return Новый корень поддерева.
 */
AVLNode* rotate_right(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T = x->right;

    x->right = y;
    y->left = T;

    y->height = 1 + (height(y->left) > height(y->right) ? height(y->left) : height(y->right));
    x->height = 1 + (height(x->left) > height(x->right) ? height(x->left) : height(x->right));

    return x;
}

/**
 * @brief Выполняет левый поворот вокруг узла.
 * @param x Узел, вокруг которого выполняется поворот.
 * @return Новый корень поддерева.
 */
AVLNode* rotate_left(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T = y->left;

    y->left = x;
    x->right = T;

    x->height = 1 + (height(x->left) > height(x->right) ? height(x->left) : height(x->right));
    y->height = 1 + (height(y->left) > height(y->right) ? height(y->left) : height(y->right));

    return y;
}

/**
 * @brief Выполняет вставку элемента в AVL-дерево.
 * @param root Указатель на корень дерева.
 * @param data Значение для вставки.
 * @return Указатель на новый корень дерева.
 */
AVLNode* insert(AVLNode* root, int data) {
    if (!root) return create_node(data);
    
    if (data < root->data) root->left = insert(root->left, data);

    else if (data > root->data) root->right = insert(root->right, data);

    else return root;
    

    root->height = 1 + (height(root->left) > height(root->right) ? height(root->left) : height(root->right));

    int balance = balance_factor(root);

    // LL поворот
    if (balance > 1 && data < root->left->data) return rotate_right(root);
    
    // RR поворот
    if (balance < -1 && data > root->right->data) return rotate_left(root);

    // LR поворот
    if (balance > 1 && data > root->left->data) {
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }

    // RL поворот
    if (balance < -1 && data < root->right->data) {
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }

    return root;
}

/**
 * @brief Обход дерева в глубину (inorder).
 * @param root Указатель на корень дерева.
 */
void traverse_inorder(AVLNode* root) {
    if (root) {
        traverse_inorder(root->left);
        printf("%d ", root->data);
        traverse_inorder(root->right);
    }
}

/**
 * @brief Освобождает память, занятую деревом.
 * @param root Указатель на корень дерева.
 */
void free_tree(AVLNode* root) {
    if (root) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

int main() {
    AVLNode* tree = create_node(5);

    tree = insert(tree, 10);
    tree = insert(tree, 20);
    tree = insert(tree, 30);
    tree = insert(tree, 40);
    tree = insert(tree, 50);
    tree = insert(tree, 25);

    printf("Inorder traversal: ");
    traverse_inorder(tree);
    printf("\n");

    free_tree(tree);

    return 0;
}
