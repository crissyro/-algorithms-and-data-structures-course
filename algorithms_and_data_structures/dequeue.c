#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/// @brief Коды ошибок для операций двусторонней очереди
typedef enum {
    DEQUE_SUCCESS,          ///< Операция выполнена успешно
    DEQUE_EMPTY,           ///< Ошибка: очередь пуста
    DEQUE_ALLOCATION_ERROR ///< Ошибка: не удалось выделить память
} Deque_ErrorCode;

/// @brief Узел двусвязного списка
typedef struct DequeNode {
    int data;                      ///< Данные узла
    struct DequeNode* next;        ///< Указатель на следующий узел
    struct DequeNode* prev;        ///< Указатель на предыдущий узел
} DequeNode;

/// @brief Двусторонняя очередь на основе двусвязного списка
typedef struct {
    DequeNode* front;              ///< Указатель на первый элемент
    DequeNode* back;               ///< Указатель на последний элемент
} Deque;

/// @brief Инициализация пустой двусторонней очереди
/// @param deque Указатель на структуру очереди
void dequeInit(Deque* deque) {
    deque->front = NULL;
    deque->back = NULL;
}

/// @brief Проверка, пуст ли дек
/// @param deque Указатель на структуру очереди
/// @return true, если дек пуст, иначе false
bool dequeIsEmpty(Deque* deque) { return deque->front == NULL; }

/// @brief Добавляет элемент в начало очереди
/// @param deque Указатель на структуру очереди
/// @param value Значение для добавления
/// @return Код ошибки (DEQUE_SUCCESS или DEQUE_ALLOCATION_ERROR)
Deque_ErrorCode dequePushFront(Deque* deque, int value) {
    DequeNode* newNode = (DequeNode*)malloc(sizeof(DequeNode));

    if (!newNode) return DEQUE_ALLOCATION_ERROR;

    newNode->data = value;
    newNode->prev = NULL;
    newNode->next = deque->front;

    if (dequeIsEmpty(deque)) deque->back = newNode;

    else deque->front->prev = newNode;
    
    deque->front = newNode;

    return DEQUE_SUCCESS;
}

/// @brief Добавляет элемент в конец очереди
/// @param deque Указатель на структуру очереди
/// @param value Значение для добавления
/// @return Код ошибки (DEQUE_SUCCESS или DEQUE_ALLOCATION_ERROR)
Deque_ErrorCode dequePushBack(Deque* deque, int value) {
    DequeNode* newNode = (DequeNode*)malloc(sizeof(DequeNode));

    if (!newNode) return DEQUE_ALLOCATION_ERROR;

    newNode->data = value;
    newNode->next = NULL;
    newNode->prev = deque->back;

    if (dequeIsEmpty(deque)) deque->front = newNode;

    else deque->back->next = newNode;
    
    deque->back = newNode;

    return DEQUE_SUCCESS;
}

/// @brief Удаляет элемент с начала очереди
/// @param deque Указатель на структуру очереди
/// @param[out] value Указатель для сохранения значения удаленного элемента
/// @return Код ошибки (DEQUE_SUCCESS или DEQUE_EMPTY)
Deque_ErrorCode dequePopFront(Deque* deque, int* value) {
    if (dequeIsEmpty(deque)) return DEQUE_EMPTY;

    DequeNode* temp = deque->front;
    *value = temp->data;
    deque->front = deque->front->next;

    if (deque->front) deque->front->prev = NULL;

    else deque->back = NULL;
    

    free(temp);

    return DEQUE_SUCCESS;
}

/// @brief Удаляет элемент с конца очереди
/// @param deque Указатель на структуру очереди
/// @param[out] value Указатель для сохранения значения удаленного элемента
/// @return Код ошибки (DEQUE_SUCCESS или DEQUE_EMPTY)
Deque_ErrorCode dequePopBack(Deque* deque, int* value) {
    if (dequeIsEmpty(deque)) return DEQUE_EMPTY;

    DequeNode* temp = deque->back;
    *value = temp->data;
    deque->back = deque->back->prev;

    if (deque->back) deque->back->next = NULL;

    else deque->front = NULL;
    

    free(temp);

    return DEQUE_SUCCESS;
}

/// @brief Освобождает всю память, выделенную для двусторонней очереди
/// @param deque Указатель на структуру очереди
void dequeClear(Deque* deque) {
    int tempValue;
    while (dequePopFront(deque, &tempValue) == DEQUE_SUCCESS);
}

/// @brief Тестирование операций двусторонней очереди
void testDeque() {
    Deque deque;
    dequeInit(&deque);
    int value;

    // Тестирование добавления и удаления с конца
    dequePushBack(&deque, 10);
    dequePushBack(&deque, 20);
    assert(dequePopBack(&deque, &value) == DEQUE_SUCCESS && value == 20);
    assert(dequePopBack(&deque, &value) == DEQUE_SUCCESS && value == 10);

    // Тестирование добавления и удаления с начала
    dequePushFront(&deque, 30);
    dequePushFront(&deque, 40);
    assert(dequePopFront(&deque, &value) == DEQUE_SUCCESS && value == 40);
    assert(dequePopFront(&deque, &value) == DEQUE_SUCCESS && value == 30);

    // Проверка на пустоту
    assert(dequeIsEmpty(&deque) == true);

    // Очистка памяти
    dequeClear(&deque);
}

int main() {
    testDeque();

    printf("All deque tests passed!\n");

    return 0;
}
