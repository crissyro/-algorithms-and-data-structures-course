#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/** @file
 *  @brief Реализация очереди.
 */

#define MAX 100  ///< Максимальный размер очереди

/// Определение ошибок
typedef enum {
    QUEUE_OK = 0,         ///< Операция успешна
    QUEUE_OVERFLOW = -1,  ///< Очередь переполнена
    QUEUE_UNDERFLOW = -2  ///< Очередь пуста
} QueueError;

typedef struct {
    int data[MAX];  ///< Массив для хранения элементов очереди
    int index_first_element;      ///< Индекс первого элемента очереди
    int index_last_element;       ///< Индекс последнего элемента очереди
    int size;       ///< Текущий размер очереди
} Queue;

/// Инициализация очереди
#define initQueue(q) ((q)->index_first_element = 0, \
                      (q)->index_last_element = -1, \
                      (q)->size = 0)

/// Проверка на пустоту
#define isQueueEmpty(q) ((q)->size == 0)

/// Проверка на переполнение
#define isQueueFull(q) ((q)->size == MAX)

/**
 * @brief Добавление элемента в очередь
 * 
 * @param q указатель на структуру очереди
 * @param value значение для добавления
 * @return QueueError результат операции (QUEUE_OK или QUEUE_OVERFLOW)
 */
QueueError queuePush(Queue *q, int value) {
    if (isQueueFull(q)) return QUEUE_OVERFLOW;
    
    q->index_last_element = (q->index_last_element + 1) % MAX;
    q->data[q->index_last_element] = value;
    q->size++;

    return QUEUE_OK;
}

/**
 * @brief Извлечение элемента из очереди
 * 
 * @param q указатель на структуру очереди
 * @param result указатель на переменную, куда будет записан извлеченный элемент
 * @return QueueError результат операции (QUEUE_OK или QUEUE_UNDERFLOW)
 */
QueueError queuePop(Queue *q, int *result) {
    if (isQueueEmpty(q)) return QUEUE_UNDERFLOW;
    
    *result = q->data[q->index_first_element];
    q->index_first_element = (q->index_first_element + 1) % MAX;
    q->size--;

    return QUEUE_OK;
}

/**
 * @brief Получение первого элемента очереди без его удаления
 * 
 * @param q указатель на структуру очереди
 * @param result указатель на переменную, куда будет записан первый элемент
 * @return QueueError результат операции (QUEUE_OK или QUEUE_UNDERFLOW)
 */
QueueError queueGetFirstElement(Queue *q, int *result) {
    if (isQueueEmpty(q)) return QUEUE_UNDERFLOW;

    *result = q->data[q->index_first_element];

    return QUEUE_OK;
}

void testQueue() {
    Queue queue;
    initQueue(&queue);

    int result;
    QueueError err;

    err = queuePush(&queue, 5);
    assert(err == QUEUE_OK);

    err = queuePush(&queue, 10);
    assert(err == QUEUE_OK);

    err = queueGetFirstElement(&queue, &result);
    assert(err == QUEUE_OK && result == 5);

    err = queuePop(&queue, &result);
    assert(err == QUEUE_OK && result == 5);

    err = queuePop(&queue, &result);
    assert(err == QUEUE_OK && result == 10);

    for (int i = 0; i < MAX; i++) {
        err = queuePush(&queue, i);
        assert(err == QUEUE_OK);
    }

    err =  queuePush(&queue, 100);
    assert(err == QUEUE_OVERFLOW);

    for (int i = 0; i < MAX; i++) {
        err = queuePop(&queue, &result);
        assert(err == QUEUE_OK);
    }

    err = queuePop(&queue, &result);
    assert(err == QUEUE_UNDERFLOW);
}

int main() {
    testQueue();

    printf("Все тесты пройдены успешно!\n");

    return 0;
}