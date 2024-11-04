#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/**
 * @brief Определяет булевы значения
 */
#define TRUE 1
#define FALSE 0

/**
 * @brief Ошибки, возвращаемые функциями работы с деревом
 */
typedef enum {
    ERROR_NONE = 0,   ///< Нет ошибок
    ERROR_MEMORY,     ///< Ошибка выделения памяти
    ERROR_NOT_FOUND,  ///< Элемент не найден
    ERROR_DUPLICATE   ///< Дубликат элемента
} Error;

/**
 * @brief Структура узла бинарного дерева
 */
typedef struct TreeNode {
    int value;              ///< Значение узла
    struct TreeNode *left;   ///< Левый потомок
    struct TreeNode *right;  ///< Правый потомок
} TreeNode;

/**
 * @brief Структура бинарного дерева
 */
typedef struct BinaryTree {
    TreeNode *root;          ///< Корневой узел дерева
} BinaryTree;

/**
 * @brief Инициализация бинарного дерева
 * @param tree Указатель на бинарное дерево
 * @return ERROR_NONE в случае успеха, ERROR_MEMORY в случае ошибки
 */
Error treeInit(BinaryTree *tree) {
    if (!tree) return ERROR_MEMORY;
    
    tree->root = NULL;

    return ERROR_NONE;
}

/**
 * @brief Создание нового узла
 * @param value Значение нового узла
 * @return Указатель на созданный узел или NULL в случае ошибки
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
 * @brief Освобождение памяти, занятой узлом и его потомками
 * @param node Указатель на узел
 */
void freeNode(TreeNode *node) {
    if (node) {
        freeNode(node->left);
        freeNode(node->right);
        free(node);
    }
}

/**
 * @brief Проверка наличия левого потомка
 * @param node Указатель на узел
 * @return TRUE, если левый потомок существует, иначе FALSE
 */
int hasLeftChild(TreeNode *node) { return node && node->left; }

/**
 * @brief Проверка наличия правого потомка
 * @param node Указатель на узел
 * @return TRUE, если правый потомок существует, иначе FALSE
 */
int hasRightChild(TreeNode *node) { return node && node->right; }

/**
 * @brief Вставка значения в бинарное дерево
 * @param tree Указатель на бинарное дерево
 * @param value Значение для вставки
 * @return ERROR_NONE в случае успеха, ERROR_DUPLICATE если значение уже существует, ERROR_MEMORY при ошибке памяти
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
 * @brief Поиск узла с заданным значением в дереве
 * @param tree Указатель на бинарное дерево
 * @param value Значение для поиска
 * @param result Указатель на найденный узел
 * @return ERROR_NONE, если узел найден, ERROR_NOT_FOUND если не найден
 */
Error treeFind(BinaryTree *tree, int value, TreeNode **result) {
    if (!tree || !tree->root) return ERROR_NOT_FOUND;

    TreeNode *current = tree->root;

    while (current) {
        if (value == current->value) {
            if (result) *result = current;
            return ERROR_NONE;
        } else if (value < current->value) current = current->left;

        else current = current->right;
    }

    return ERROR_NOT_FOUND;
}

/**
 * @brief Удаление узла с заданным значением из дерева
 * @param tree Указатель на бинарное дерево
 * @param value Значение для удаления
 * @return ERROR_NONE в случае успеха, ERROR_NOT_FOUND если узел не найден
 */
Error treeRemove(BinaryTree *tree, int value) {
    if (!tree || !tree->root) return ERROR_NOT_FOUND;

    TreeNode **current = &(tree->root);
    TreeNode *parent = NULL;

    while (*current && (*current)->value != value) {
        parent = *current;
        if (value < (*current)->value) current = &((*current)->left);

        else current = &((*current)->right);
    }

    if (!(*current)) return ERROR_NOT_FOUND;

    TreeNode *nodeToDelete = *current;

    if (!nodeToDelete->left && !nodeToDelete->right) *current = NULL;

    else if (!nodeToDelete->left) *current = nodeToDelete->right;

    else if (!nodeToDelete->right) *current = nodeToDelete->left;

    else {
        TreeNode *successorParent = nodeToDelete;
        TreeNode *successor = nodeToDelete->right;

        while (successor->left) {
            successorParent = successor;
            successor = successor->left;
        }

        nodeToDelete->value = successor->value;

        if (successorParent->left == successor) successorParent->left = successor->right;

        else successorParent->right = successor->right;

        nodeToDelete = successor;
    }

    free(nodeToDelete);
    
    return ERROR_NONE;
}

/**
 * @brief Освобождение памяти, занятой деревом
 * @param tree Указатель на бинарное дерево
 */
void treeFree(BinaryTree *tree) {
    if (tree) {
        freeNode(tree->root);
        tree->root = NULL;
    }
}

/**
 * @brief Проверка, является ли дерево пустым
 * @param tree Указатель на бинарное дерево
 * @return TRUE, если дерево пустое, иначе FALSE
 */
int treeIsEmpty(BinaryTree *tree) { return tree->root == NULL; }

/**
 * @brief Подсчет количества узлов в дереве
 * @param node Указатель на узел дерева
 * @return Количество узлов в дереве
 */
int treeNodeCount(TreeNode *node) {
    if (!node) return 0;

    return 1 + treeNodeCount(node->left) + treeNodeCount(node->right);
}

/**
 * @brief Подсчет высоты дерева
 * @param node Указатель на узел дерева
 * @return Высота дерева
 */
int treeHeight(TreeNode *node) {
    if (!node) return 0;

    int leftHeight = treeHeight(node->left);
    int rightHeight = treeHeight(node->right);
    
    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

/**
 * @brief Тестирование основных функций работы с деревом
 */
void test() {
    BinaryTree tree;

    assert(treeInit(&tree) == ERROR_NONE);
    assert(treeInsert(&tree, 10) == ERROR_NONE);
    assert(treeInsert(&tree, 5) == ERROR_NONE);
    assert(treeInsert(&tree, 15) == ERROR_NONE);

    assert(treeInsert(&tree, 5) == ERROR_DUPLICATE);

    TreeNode *found = NULL;

    assert(treeFind(&tree, 10, &found) == ERROR_NONE && found->value == 10);
    assert(treeFind(&tree, 5, &found) == ERROR_NONE && found->value == 5);
    assert(treeFind(&tree, 15, &found) == ERROR_NONE && found->value == 15);
    assert(treeFind(&tree, 20, &found) == ERROR_NOT_FOUND);

    assert(hasLeftChild(tree.root) == TRUE);
    assert(hasRightChild(tree.root) == TRUE);
    assert(hasLeftChild(tree.root->left) == FALSE);
    assert(hasRightChild(tree.root->left) == FALSE);

    assert(treeRemove(&tree, 5) == ERROR_NONE);
    assert(treeFind(&tree, 5, &found) == ERROR_NOT_FOUND);
    assert(treeRemove(&tree, 20) == ERROR_NOT_FOUND);

    assert(treeIsEmpty(&tree) == FALSE);

    treeFree(&tree);

    assert(treeIsEmpty(&tree) == TRUE);
}

/**
 * @brief Главная функция программы
 * @return Код завершения программы
 */
int main() {
    test();

    printf("Все тесты пройдены успешно.\n");

    return 0;
}
