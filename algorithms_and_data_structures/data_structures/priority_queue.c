#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

/// @brief Коды ошибок для операций с очередью
typedef enum {
    PRIORITY_QUEUE_OK,            ///< Операция выполнена успешно
    PRIORITY_QUEUE_NULL_POINTER,  ///< Указатель на очередь равен NULL
    PRIORITY_QUEUE_EMPTY,         ///< Очередь пуста
    PRIORITY_QUEUE_MEMORY_ERROR   ///< Ошибка выделения памяти
} PriorityQueueError;

/// @brief Узел односвязного списка с приоритетом
typedef struct Node {
    int data;                  ///< Данные узла
    int priority;              ///< Приоритет узла
    struct Node* next;         ///< Указатель на следующий узел
} Node;

/// @brief Структура приоритетной очереди на основе односвязного списка
typedef struct {
    Node* front;               ///< Указатель на первый элемент очереди
    int size;                  ///< Текущий размер очереди
} PriorityQueue;

/**
 * @brief Инициализирует пустую приоритетную очередь
 * @param queue Указатель на структуру очереди
 * @return Код ошибки (PRIORITY_QUEUE_OK при успешной инициализации)
 */
PriorityQueueError priorityQueueInit(PriorityQueue* queue) {
    if (!queue) return PRIORITY_QUEUE_NULL_POINTER;

    queue->front = NULL;
    queue->size = 0;

    return PRIORITY_QUEUE_OK;
}

/**
 * @brief Добавляет элемент в очередь в соответствии с его приоритетом
 * @param queue Указатель на структуру очереди
 * @param value Значение для добавления
 * @param priority Приоритет значения
 * @return Код ошибки (PRIORITY_QUEUE_OK при успешной вставке)
 */
PriorityQueueError priorityQueueEnqueue(PriorityQueue* queue, int value, int priority) {
    if (!queue) return PRIORITY_QUEUE_NULL_POINTER;

    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) return PRIORITY_QUEUE_MEMORY_ERROR;

    newNode->data = value;
    newNode->priority = priority;
    newNode->next = NULL;

    if (!queue->front || queue->front->priority < priority) {
        newNode->next = queue->front;
        queue->front = newNode;
    } else {
        Node* current = queue->front;
        while (current->next && current->next->priority >= priority) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }

    queue->size++;

    return PRIORITY_QUEUE_OK;
}

/**
 * @brief Удаляет элемент с наивысшим приоритетом из очереди
 * @param queue Указатель на структуру очереди
 * @param value Указатель для сохранения удалённого значения
 * @return Код ошибки (PRIORITY_QUEUE_OK при успешном удалении, PRIORITY_QUEUE_EMPTY если очередь пуста)
 */
PriorityQueueError priorityQueueDequeue(PriorityQueue* queue, int* value) {
    if (!queue) return PRIORITY_QUEUE_NULL_POINTER;

    if (!queue->front) return PRIORITY_QUEUE_EMPTY;

    Node* temp = queue->front;
    *value = temp->data;
    queue->front = temp->next;

    free(temp);
    
    queue->size--;

    return PRIORITY_QUEUE_OK;
}

/**
 * @brief Возвращает значение элемента с наивысшим приоритетом, не удаляя его
 * @param queue Указатель на структуру очереди
 * @param value Указатель для сохранения значения
 * @return Код ошибки (PRIORITY_QUEUE_OK при успешном получении значения, PRIORITY_QUEUE_EMPTY если очередь пуста)
 */
PriorityQueueError priorityQueuePeek(PriorityQueue* queue, int* value) {
    if (!queue) return PRIORITY_QUEUE_NULL_POINTER;

    if (!queue->front) return PRIORITY_QUEUE_EMPTY;

    *value = queue->front->data;

    return PRIORITY_QUEUE_OK;
}

/**
 * @brief Проверяет, пуста ли очередь
 * @param queue Указатель на структуру очереди
 * @return true, если очередь пуста, иначе false
 */
bool priorityQueueIsEmpty(PriorityQueue* queue) { return queue && queue->front == NULL; }

/**
 * @brief Возвращает текущий размер очереди
 * @param queue Указатель на структуру очереди
 * @return Текущий размер очереди
 */
int priorityQueueSize(PriorityQueue* queue) { return queue ? queue->size : -1; }

/**
 * @brief Очищает очередь, удаляя все элементы
 * @param queue Указатель на структуру очереди
 * @return Код ошибки (PRIORITY_QUEUE_OK при успешной очистке)
 */
PriorityQueueError priorityQueueClear(PriorityQueue* queue) {
    if (!queue) return PRIORITY_QUEUE_NULL_POINTER;

    Node* current = queue->front;

    while (current) {
        Node* next = current->next;
        free(current);
        current = next;
    }

    queue->front = NULL;
    queue->size = 0;

    return PRIORITY_QUEUE_OK;
}

/**
 * @brief Удаляет структуру очереди и освобождает память
 * @param queue Указатель на структуру очереди
 * @return Код ошибки (PRIORITY_QUEUE_OK при успешном удалении)
 */
PriorityQueueError priorityQueueDelete(PriorityQueue* queue) {
    if (!queue) return PRIORITY_QUEUE_NULL_POINTER;

    priorityQueueClear(queue);

    free(queue);

    return PRIORITY_QUEUE_OK;
}

/**
 * @brief Тестирует операции приоритетной очереди
 */
void testPriorityQueue() {
    PriorityQueue queue;
    PriorityQueueError err = priorityQueueInit(&queue);
    assert(err == PRIORITY_QUEUE_OK);

    // Проверка добавления элементов с приоритетами
    err = priorityQueueEnqueue(&queue, 10, 1);
    assert(err == PRIORITY_QUEUE_OK);

    err = priorityQueueEnqueue(&queue, 20, 3);
    assert(err == PRIORITY_QUEUE_OK);

    err = priorityQueueEnqueue(&queue, 15, 2);
    assert(err == PRIORITY_QUEUE_OK);

    int value;

    // Проверка извлечения элемента с наивысшим приоритетом
    err = priorityQueueDequeue(&queue, &value);
    assert(err == PRIORITY_QUEUE_OK && value == 20);

    // Проверка получения элемента с наивысшим приоритетом без удаления
    err = priorityQueuePeek(&queue, &value);
    assert(err == PRIORITY_QUEUE_OK && value == 15);

    // Очистка и проверка очереди на пустоту
    err = priorityQueueClear(&queue);
    assert(err == PRIORITY_QUEUE_OK && priorityQueueIsEmpty(&queue));
}

int main() {
    testPriorityQueue();

    printf("All priority queue tests passed!\n");

    return 0;
}
