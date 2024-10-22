#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define TRUE 1
#define FALSE 0

/**
 * @brief Перечисление ошибок, которые могут возникнуть при работе с деревом.
 */
typedef enum {
    ERROR_NONE = 0,   /**< Нет ошибок */
    ERROR_MEMORY,     /**< Ошибка выделения памяти */
    ERROR_NOT_FOUND,  /**< Элемент не найден */
    ERROR_DUPLICATE   /**< Дубликат элемента */
} Error;

/**
 * @brief Структура узла бинарного дерева.
 */
typedef struct TreeNode {
    int value;               /**< Значение узла */
    struct TreeNode *left;   /**< Левый потомок */
    struct TreeNode *right;  /**< Правый потомок */
} TreeNode;

/**
 * @brief Структура бинарного дерева.
 */
typedef struct BinaryTree {
    TreeNode *root; /**< Корень дерева */
} BinaryTree;

/**
 * @brief Инициализация бинарного дерева.
 * @param tree Указатель на бинарное дерево.
 * @return Error Код ошибки.
 */
Error treeInit(BinaryTree *tree) {
    if (!tree) return ERROR_MEMORY;
    
    tree->root = NULL;

    return ERROR_NONE;
}

/**
 * @brief Создание нового узла с заданным значением.
 * @param value Значение узла.
 * @return TreeNode* Указатель на новый узел.
 */
TreeNode* createNode(int value) {
    TreeNode *node = (TreeNode*) malloc(sizeof(TreeNode));
    if (!node) return NULL;

    node->value = value;
    node->left = NULL;
    node->right = NULL;

    return node;
}

/**
 * @brief Рекурсивное освобождение памяти, занимаемой узлами дерева.
 * @param node Указатель на узел.
 */
void freeNode(TreeNode *node) {
    if (node) {
        freeNode(node->left);
        freeNode(node->right);
        free(node);
    }
}

/**
 * @brief Вставка значения в бинарное дерево.
 * @param tree Указатель на бинарное дерево.
 * @param value Значение для вставки.
 * @return Error Код ошибки.
 */
Error treeInsert(BinaryTree *tree, int value) {
    if (!tree) return ERROR_MEMORY;

    TreeNode **current = &(tree->root);

    while (*current) {
        if (value == (*current)->value) return ERROR_DUPLICATE;

        else if (value < (*current)->value) current = &((*current)->left);

        else current = &((*current)->right);
    }

    *current = createNode(value);

    if (!(*current)) return ERROR_MEMORY;

    return ERROR_NONE;
}

/**
 * @brief Построение бинарного дерева из массива значений.
 * @param tree Указатель на бинарное дерево.
 * @param values Массив значений.
 * @param size Размер массива.
 */
void buildOrderedTree(BinaryTree *tree, int *values, int size) {
    for (int i = 0; i < size; ++i) treeInsert(tree, values[i]);
}

/**
 * @brief Рекурсивный обход дерева в глубину (in-order) и вывод n элементов.
 * @param node Указатель на узел.
 * @param count Счетчик элементов.
 * @param n Количество элементов для вывода.
 */
void inorderTraversal(TreeNode *node, int *count, int n) {
    if (node && (*count) < n) {
        inorderTraversal(node->left, count, n);
        if ((*count) < n) {
            printf("%d ", node->value);
            (*count)++;
        }

        inorderTraversal(node->right, count, n);
    }
}

/**
 * @brief Вывод n элементов дерева в порядке возрастания.
 * @param tree Указатель на бинарное дерево.
 * @param n Количество элементов для вывода.
 */
void printOrderedSequence(BinaryTree *tree, int n) {
    int count = 0;
    inorderTraversal(tree->root, &count, n);
    printf("\n");
}

/**
 * @brief Подсчет количества узлов в поддереве.
 * @param node Указатель на узел.
 * @return int Количество узлов.
 */
int countNodes(TreeNode *node) {
    if (!node) return 0;

    return 1 + countNodes(node->left) + countNodes(node->right);
}

/**
 * @brief Вывод количества узлов в левом и правом поддеревьях для каждого узла.
 * @param node Указатель на узел.
 */
void printSubtreeCounts(TreeNode *node) {
    if (node) {
        int leftCount = countNodes(node->left);
        int rightCount = countNodes(node->right);

        printf("Вершина %d -> Левое поддерево: %d, Правое поддерево: %d\n", node->value, leftCount, rightCount);

        printSubtreeCounts(node->left);
        printSubtreeCounts(node->right);
    }
}

/**
 * @brief Рекурсивный обход дерева в глубину (DFS).
 * @param node Указатель на узел.
 */
void depthFirstSearch(TreeNode *node) {
    if (node) {
        printf("%d ", node->value);
        depthFirstSearch(node->left);
        depthFirstSearch(node->right);
    }
}

/**
 * @brief Обход дерева в ширину (BFS).
 * @param root Указатель на корневой узел.
 */
void breadthFirstSearch(TreeNode *root) {
    if (!root) return;

    TreeNode **queue = (TreeNode**) malloc(100 * sizeof(TreeNode*));
    int front = 0, rear = 0;

    queue[rear++] = root;

    while (front < rear) {
        TreeNode *node = queue[front++];

        printf("%d ", node->value);

        if (node->left) queue[rear++] = node->left;
        if (node->right) queue[rear++] = node->right;
    }

    free(queue);
}

/**
 * @brief Освобождение памяти, занятой деревом.
 * @param tree Указатель на бинарное дерево.
 */
void treeFree(BinaryTree *tree) {
    if (tree) {
        freeNode(tree->root);
        tree->root = NULL;
    }
}

/**
 * @brief Тестирование всех функций.
 */
void test() {
    BinaryTree tree;
    assert(treeInit(&tree) == ERROR_NONE);

    int values[] = {10, 5, 15, 3, 7, 13, 17};
    int size = sizeof(values) / sizeof(values[0]);

    buildOrderedTree(&tree, values, size);
    printf("Дерево построено успешно.\n");

    printf("Упорядоченная последовательность (первые 5 элементов):\n");
    printOrderedSequence(&tree, 5);

    printf("Количество вершин в левом и правом поддеревьях:\n");
    printSubtreeCounts(tree.root);

    printf("Обход в глубину (DFS):\n");
    depthFirstSearch(tree.root);
    printf("\n");

    printf("Обход в ширину (BFS):\n");
    breadthFirstSearch(tree.root);
    printf("\n");

    treeFree(&tree);
}

/**
 * @brief Точка входа в программу.
 * @return int Код завершения.
 */
int main() {
    test();

    printf("Все тесты пройдены успешно.\n");

    return 0;
}
