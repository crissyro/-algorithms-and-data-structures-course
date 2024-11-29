#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

/// @brief Коды ошибок для операций со стеком
typedef enum {
    STACK_OK,           ///< Операция выполнена успешно
    STACK_NULL_POINTER, ///< Указатель на стек равен NULL
    STACK_EMPTY,        ///< Стек пуст
    STACK_MEMORY_ERROR  ///< Ошибка выделения памяти
} StackError;

/// @brief Узел односвязного списка
typedef struct Node {
    int data;              ///< Данные узла
    struct Node* next;     ///< Указатель на следующий узел
} Node;

/// @brief Структура стека на основе односвязного списка
typedef struct {
    Node* top;             ///< Указатель на верхний элемент стека
} Stack;

/**
 * @brief Инициализирует пустой стек
 * @param stack Указатель на структуру стека
 * @return Код ошибки (STACK_OK при успешной инициализации)
 */
StackError stackInit(Stack* stack) {
    if (!stack) return STACK_NULL_POINTER;

    stack->top = NULL;

    return STACK_OK;
}

/**
 * @brief Добавляет элемент на вершину стека
 * @param stack Указатель на структуру стека
 * @param value Значение для добавления
 * @return Код ошибки (STACK_OK при успешной вставке)
 */
StackError stackPush(Stack* stack, int value) {
    if (!stack) return STACK_NULL_POINTER;

    Node* newNode = (Node*)malloc(sizeof(Node));

    if (!newNode) return STACK_MEMORY_ERROR;

    newNode->data = value;
    newNode->next = stack->top;
    stack->top = newNode;

    return STACK_OK;
}

/**
 * @brief Удаляет элемент с вершины стека
 * @param stack Указатель на структуру стека
 * @param value Указатель для сохранения удалённого значения
 * @return Код ошибки (STACK_OK при успешном удалении, STACK_EMPTY если стек пуст)
 */
StackError stackPop(Stack* stack, int* value) {
    if (!stack) return STACK_NULL_POINTER;

    if (!stack->top) return STACK_EMPTY;

    Node* temp = stack->top;
    *value = temp->data;
    stack->top = temp->next;

    free(temp);

    return STACK_OK;
}

/**
 * @brief Возвращает значение верхнего элемента стека, не удаляя его
 * @param stack Указатель на структуру стека
 * @param value Указатель для сохранения значения
 * @return Код ошибки (STACK_OK при успешном получении значения, STACK_EMPTY если стек пуст)
 */
StackError stackPeek(Stack* stack, int* value) {
    if (!stack) return STACK_NULL_POINTER;

    if (!stack->top) return STACK_EMPTY;

    *value = stack->top->data;

    return STACK_OK;
}

/**
 * @brief Проверяет, пуст ли стек
 * @param stack Указатель на структуру стека
 * @return true, если стек пуст, иначе false
 */
bool stackIsEmpty(Stack* stack) { return stack && stack->top == NULL; }

/**
 * @brief Очищает стек, удаляя все элементы
 * @param stack Указатель на структуру стека
 * @return Код ошибки (STACK_OK при успешной очистке)
 */
StackError stackClear(Stack* stack) {
    if (!stack) return STACK_NULL_POINTER;

    Node* current = stack->top;

    while (current) {
        Node* next = current->next;
        free(current);
        current = next;
    }

    stack->top = NULL;

    return STACK_OK;
}

/**
 * @brief Удаляет структуру стека и освобождает память
 * @param stack Указатель на структуру стека
 * @return Код ошибки (STACK_OK при успешном удалении)
 */
StackError stackDelete(Stack* stack) {
    if (!stack) return STACK_NULL_POINTER;

    stackClear(stack);
    
    free(stack);

    return STACK_OK;
}

/**
 * @brief Тестирует операции стека
 */
void testStack() {
    Stack stack;
    StackError err = stackInit(&stack);
    assert(err == STACK_OK);

    // Проверка добавления элементов
    err = stackPush(&stack, 10);
    assert(err == STACK_OK);

    err = stackPush(&stack, 20);
    assert(err == STACK_OK);

    int value;

    // Проверка получения верхнего элемента
    err = stackPeek(&stack, &value);
    assert(err == STACK_OK && value == 20);

    // Проверка удаления элементов
    err = stackPop(&stack, &value);
    assert(err == STACK_OK && value == 20);

    err = stackPop(&stack, &value);
    assert(err == STACK_OK && value == 10);

    // Проверка на пустоту стека
    assert(stackIsEmpty(&stack) == true);

    // Проверка очистки стека
    err = stackPush(&stack, 30);
    err = stackClear(&stack);
    assert(err == STACK_OK && stackIsEmpty(&stack));
}

int main() {
    testStack();

    printf("All stack tests passed!\n");

    return 0;
}
