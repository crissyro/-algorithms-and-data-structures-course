#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define QUEUE_SIZE 512

// Код ошибок
typedef enum {
    SUCCESS = 0,         /**< Операция выполнена успешно */
    ERR_MEMORY = 1,      /**< Ошибка выделения памяти */
    ERR_NOT_FOUND = 2,   /**< Элемент не найден */
    ERR_INVALID_INPUT = 3 /**< Некорректный ввод */
} ErrorCode;

// Узел бинарного дерева
typedef struct TreeNode {
    int data;               /**< Значение узла */
    struct TreeNode* left;  /**< Левый потомок */
    struct TreeNode* right; /**< Правый потомок */
} TreeNode;

/**
 * @brief Создает новый узел бинарного дерева.
 * @param data Значение для нового узла.
 * @return Указатель на созданный узел или NULL при ошибке выделения памяти.
 */
TreeNode* create_node(int data) {
    TreeNode* new_node = (TreeNode*)malloc(sizeof(TreeNode));

    if (!new_node) return NULL;
    
    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

/**
 * @brief Вставляет элемент в бинарное дерево.
 * @param root Указатель на корень дерева.
 * @param data Значение для вставки.
 * @return Код ошибки: SUCCESS или ERR_MEMORY.
 */
ErrorCode insert(TreeNode** root, int data) {
    if (!root) return ERR_INVALID_INPUT;

    if (*root == NULL) {
        *root = create_node(data);
        return *root ? SUCCESS : ERR_MEMORY;
    }

    if (data < (*root)->data) return insert(&(*root)->left, data);

    else return insert(&(*root)->right, data);
}

/**
 * @brief Поиск элемента в бинарном дереве.
 * @param root Указатель на корень дерева.
 * @param data Значение для поиска.
 * @return Указатель на найденный узел или NULL, если элемент не найден.
 */
TreeNode* search(TreeNode* root, int data) {
    if (!root || root->data == data) return root;
    
    if (data < root->data) return search(root->left, data);

    else return search(root->right, data);
    
}

/**
 * @brief Находит минимальный узел в дереве.
 * @param root Указатель на корень дерева.
 * @return Указатель на узел с минимальным значением.
 */
TreeNode* find_min(TreeNode* root) {
    while (root && root->left) root = root->left;
    
    return root;
}

/**
 * @brief Удаляет элемент из бинарного дерева.
 * @param root Указатель на корень дерева.
 * @param data Значение для удаления.
 * @return Код ошибки: SUCCESS, ERR_NOT_FOUND или ERR_MEMORY.
 */
ErrorCode delete(TreeNode** root, int data) {
    if (!root || !(*root)) return ERR_NOT_FOUND;

    if (data < (*root)->data) return delete(&(*root)->left, data);

    else if (data > (*root)->data) return delete(&(*root)->right, data);

    else {
        // Узел найден
        if (!(*root)->left) {
            TreeNode* temp = (*root)->right;
            free(*root);
            *root = temp;
        } else if (!(*root)->right) {
            TreeNode* temp = (*root)->left;
            free(*root);
            *root = temp;
        } else {
            TreeNode* temp = find_min((*root)->right);
            (*root)->data = temp->data;
            return delete(&(*root)->right, temp->data);
        }

        return SUCCESS;
    }
}

/**
 * @brief Обход дерева в глубину (прямой порядок).
 * @param root Указатель на корень дерева.
 */
void traverse_inorder(TreeNode* root) {
    if (root) {
        traverse_inorder(root->left);
        printf("%d ", root->data);
        traverse_inorder(root->right);
    }
}

/**
 * @brief Обход дерева в ширину.
 * @param root Указатель на корень дерева.
 */
void traverse_breadth(TreeNode* root) {
    if (!root) return;

    // Создаем очередь
    TreeNode** queue = (TreeNode**)malloc(sizeof(TreeNode*) * QUEUE_SIZE); 
    if (!queue) return;

    int front = 0, rear = 0;
    queue[rear++] = root;

    while (front < rear) {
        TreeNode* current = queue[front++];
        printf("%d ", current->data);

        if (current->left) queue[rear++] = current->left;
        if (current->right) queue[rear++] = current->right;
    }

    free(queue);
}

/**
 * @brief Освобождает память, занятую деревом.
 * @param root Указатель на корень дерева.
 */
void free_tree(TreeNode* root) {
    if (root) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

// Пример тестирования
int main() {
    TreeNode* tree = create_node(5);

    assert(insert(&tree, 50) == SUCCESS);
    assert(insert(&tree, 30) == SUCCESS);
    assert(insert(&tree, 70) == SUCCESS);
    assert(insert(&tree, 20) == SUCCESS);
    assert(insert(&tree, 40) == SUCCESS);
    assert(insert(&tree, 60) == SUCCESS);
    assert(insert(&tree, 80) == SUCCESS);

    printf("Inorder traversal: ");
    traverse_inorder(tree);
    printf("\n");

    printf("Breadth traversal: ");
    traverse_breadth(tree);
    printf("\n");

    assert(search(tree, 40) != NULL);
    assert(search(tree, 90) == NULL);

    assert(delete(&tree, 20) == SUCCESS);
    assert(delete(&tree, 50) == SUCCESS);
    assert(delete(&tree, 100) == ERR_NOT_FOUND);

    printf("Inorder traversal after deletion: ");
    traverse_inorder(tree);
    printf("\n");

    free_tree(tree);

    return 0;
}
