#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/// @brief Коды ошибок для операций двусвязного списка
typedef enum {
    DLL_SUCCESS,         ///< Операция выполнена успешно
    DLL_EMPTY_LIST,     ///< Ошибка: список пуст
    DLL_ALLOCATION_ERROR ///< Ошибка: не удалось выделить память
} DLL_ErrorCode;

/// @brief Узел стека
typedef struct StackNode {
    int data;                     ///< Данные узла
    struct StackNode* next;       ///< Указатель на следующий элемент в стеке
} StackNode;

/// @brief Стек, используемый для реализации двусвязного списка
typedef struct Stack {
    StackNode* top;               ///< Указатель на верхний элемент стека
} Stack;

/// @brief Двусвязный список, реализованный через два стека
typedef struct {
    Stack prevStack;              ///< Стек для хранения «предыдущих» элементов
    Stack nextStack;              ///< Стек для хранения «следующих» элементов
} DoublyLinkedList;

/// @brief Инициализация стека
/// @param stack Указатель на структуру стека
void stackInit(Stack* stack) {
    stack->top = NULL;
}

/// @brief Проверка, пуст ли стек
/// @param stack Указатель на структуру стека
/// @return true, если стек пуст, иначе false
bool stackIsEmpty(Stack* stack) {
    return stack->top == NULL;
}

/// @brief Добавляет элемент в стек
/// @param stack Указатель на структуру стека
/// @param value Значение для добавления
/// @return Код ошибки (DLL_SUCCESS или DLL_ALLOCATION_ERROR)
DLL_ErrorCode stackPush(Stack* stack, int value) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));

    if (!newNode) return DLL_ALLOCATION_ERROR;

    newNode->data = value;
    newNode->next = stack->top;
    stack->top = newNode;

    return DLL_SUCCESS;
}

/// @brief Удаляет элемент из стека
/// @param stack Указатель на структуру стека
/// @param[out] value Указатель для сохранения значения удаленного элемента
/// @return Код ошибки (DLL_SUCCESS или DLL_EMPTY_LIST)
DLL_ErrorCode stackPop(Stack* stack, int* value) {
    if (stackIsEmpty(stack)) return DLL_EMPTY_LIST;

    StackNode* temp = stack->top;
    *value = temp->data;
    stack->top = stack->top->next;

    free(temp);

    return DLL_SUCCESS;
}

/// @brief Инициализация двусвязного списка через два стека
/// @param list Указатель на структуру двусвязного списка
void doublyLinkedListInit(DoublyLinkedList* list) {
    stackInit(&list->prevStack);
    stackInit(&list->nextStack);
}

/// @brief Добавляет элемент в конец списка
/// @param list Указатель на структуру двусвязного списка
/// @param value Значение для добавления
/// @return Код ошибки (DLL_SUCCESS или DLL_ALLOCATION_ERROR)
DLL_ErrorCode doublyLinkedListPushBack(DoublyLinkedList* list, int value) {
    return stackPush(&list->nextStack, value);
}

/// @brief Удаляет элемент с конца списка
/// @param list Указатель на структуру двусвязного списка
/// @param[out] value Указатель для сохранения значения удаленного элемента
/// @return Код ошибки (DLL_SUCCESS, DLL_EMPTY_LIST или DLL_ALLOCATION_ERROR)
DLL_ErrorCode doublyLinkedListPopBack(DoublyLinkedList* list, int* value) {
    if (stackIsEmpty(&list->nextStack) && stackIsEmpty(&list->prevStack)) 
        return DLL_EMPTY_LIST; 
    
    if (stackIsEmpty(&list->nextStack)) {
        int temp;
        while (!stackIsEmpty(&list->prevStack)) {
            stackPop(&list->prevStack, &temp);
            stackPush(&list->nextStack, temp);
        }
    }

    return stackPop(&list->nextStack, value);
}

/// @brief Добавляет элемент в начало списка
/// @param list Указатель на структуру двусвязного списка
/// @param value Значение для добавления
/// @return Код ошибки (DLL_SUCCESS или DLL_ALLOCATION_ERROR)
DLL_ErrorCode doublyLinkedListPushFront(DoublyLinkedList* list, int value) {
    return stackPush(&list->prevStack, value);
}

/// @brief Удаляет элемент с начала списка
/// @param list Указатель на структуру двусвязного списка
/// @param[out] value Указатель для сохранения значения удаленного элемента
/// @return Код ошибки (DLL_SUCCESS, DLL_EMPTY_LIST или DLL_ALLOCATION_ERROR)
DLL_ErrorCode doublyLinkedListPopFront(DoublyLinkedList* list, int* value) {
    if (stackIsEmpty(&list->prevStack) && stackIsEmpty(&list->nextStack)) 
        return DLL_EMPTY_LIST; 
    

    if (stackIsEmpty(&list->prevStack)) {
        int temp;
        while (!stackIsEmpty(&list->nextStack)) {
            stackPop(&list->nextStack, &temp);
            stackPush(&list->prevStack, temp);
        }
    }

    return stackPop(&list->prevStack, value);
}

/// @brief Проверка, пуст ли список
/// @param list Указатель на структуру двусвязного списка
/// @return true, если список пуст, иначе false
bool doublyLinkedListIsEmpty(DoublyLinkedList* list) {
    return stackIsEmpty(&list->prevStack) && stackIsEmpty(&list->nextStack);
}

/// @brief Тестирование операций двусвязного списка
void testDoublyLinkedList() {
    DoublyLinkedList list;
    doublyLinkedListInit(&list);
    int value;

    // Тестирование добавления в конец и удаление с конца
    doublyLinkedListPushBack(&list, 10);
    doublyLinkedListPushBack(&list, 20);
    assert(doublyLinkedListPopBack(&list, &value) == DLL_SUCCESS && value == 20);
    assert(doublyLinkedListPopBack(&list, &value) == DLL_SUCCESS && value == 10);

    // Тестирование добавления в начало и удаление с начала
    doublyLinkedListPushFront(&list, 30);
    doublyLinkedListPushFront(&list, 40);
    assert(doublyLinkedListPopFront(&list, &value) == DLL_SUCCESS && value == 40);
    assert(doublyLinkedListPopFront(&list, &value) == DLL_SUCCESS && value == 30);

    // Проверка на пустоту
    assert(doublyLinkedListIsEmpty(&list) == true);
}

int main() {
    testDoublyLinkedList();

    printf("All doubly linked list tests passed!\n");

    return 0;
}
