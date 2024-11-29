#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

/// @brief Коды ошибок для операций с очередью
typedef enum {
    QUEUE_OK,            ///< Операция выполнена успешно
    QUEUE_NULL_POINTER,  ///< Указатель на очередь равен NULL
    QUEUE_EMPTY,         ///< Очередь пуста
    QUEUE_MEMORY_ERROR   ///< Ошибка выделения памяти
} QueueError;

/// @brief Узел односвязного списка
typedef struct Node {
    int data;              ///< Данные узла
    struct Node* next;     ///< Указатель на следующий узел
} Node;

/// @brief Структура очереди на основе односвязного списка
typedef struct {
    Node* front;           ///< Указатель на первый элемент очереди
    Node* last;            ///< Указатель на последний элемент очереди
    int size;              ///< Текущий размер очереди
} Queue;

/**
 * @brief Инициализирует пустую очередь
 * @param queue Указатель на структуру очереди
 * @return Код ошибки (QUEUE_OK при успешной инициализации)
 */
QueueError queueInit(Queue* queue) {
    if (!queue) return QUEUE_NULL_POINTER;

    queue->front = NULL;
    queue->last = NULL;
    queue->size = 0;

    return QUEUE_OK;
}

/**
 * @brief Добавляет элемент в конец очереди
 * @param queue Указатель на структуру очереди
 * @param value Значение для добавления
 * @return Код ошибки (QUEUE_OK при успешной вставке)
 */
QueueError queueEnqueue(Queue* queue, int value) {
    if (!queue) return QUEUE_NULL_POINTER;

    Node* newNode = (Node*)malloc(sizeof(Node));

    if (!newNode) return QUEUE_MEMORY_ERROR;

    newNode->data = value;
    newNode->next = NULL;

    if (!queue->last) queue->front = newNode;

    else queue->last->next = newNode;
    
    queue->last = newNode;
    queue->size++;

    return QUEUE_OK;
}

/**
 * @brief Удаляет элемент из начала очереди
 * @param queue Указатель на структуру очереди
 * @param value Указатель для сохранения удалённого значения
 * @return Код ошибки (QUEUE_OK при успешном удалении, QUEUE_EMPTY если очередь пуста)
 */
QueueError queueDequeue(Queue* queue, int* value) {
    if (!queue) return QUEUE_NULL_POINTER;

    if (!queue->front) return QUEUE_EMPTY;

    Node* temp = queue->front;
    *value = temp->data;
    queue->front = temp->next;

    if (!queue->front) queue->last = NULL;
    
    free(temp);

    queue->size--;

    return QUEUE_OK;
}

/**
 * @brief Возвращает значение первого элемента очереди, не удаляя его
 * @param queue Указатель на структуру очереди
 * @param value Указатель для сохранения значения
 * @return Код ошибки (QUEUE_OK при успешном получении значения, QUEUE_EMPTY если очередь пуста)
 */
QueueError queuePeekFront(Queue* queue, int* value) {
    if (!queue) return QUEUE_NULL_POINTER;

    if (!queue->front) return QUEUE_EMPTY;

    *value = queue->front->data;

    return QUEUE_OK;
}

/**
 * @brief Возвращает значение последнего элемента очереди, не удаляя его
 * @param queue Указатель на структуру очереди
 * @param value Указатель для сохранения значения
 * @return Код ошибки (QUEUE_OK при успешном получении значения, QUEUE_EMPTY если очередь пуста)
 */
QueueError queuePeekLast(Queue* queue, int* value) {
    if (!queue) return QUEUE_NULL_POINTER;
    
    if (!queue->last) return QUEUE_EMPTY;

    *value = queue->last->data;

    return QUEUE_OK;
}

/**
 * @brief Проверяет, пуста ли очередь
 * @param queue Указатель на структуру очереди
 * @return true, если очередь пуста, иначе false
 */
bool queueIsEmpty(Queue* queue) { return queue && queue->front == NULL; }

/**
 * @brief Возвращает текущий размер очереди
 * @param queue Указатель на структуру очереди
 * @return Текущий размер очереди
 */
int queueSize(Queue* queue) { return queue ? queue->size : -1; }

/**
 * @brief Очищает очередь, удаляя все элементы
 * @param queue Указатель на структуру очереди
 * @return Код ошибки (QUEUE_OK при успешной очистке)
 */
QueueError queueClear(Queue* queue) {
    if (!queue) return QUEUE_NULL_POINTER;

    Node* current = queue->front;

    while (current) {
        Node* next = current->next;
        free(current);
        current = next;
    }

    queue->front = NULL;
    queue->last = NULL;
    queue->size = 0;

    return QUEUE_OK;
}

/**
 * @brief Удаляет структуру очереди и освобождает память
 * @param queue Указатель на структуру очереди
 * @return Код ошибки (QUEUE_OK при успешном удалении)
 */
QueueError queueDelete(Queue* queue) {
    if (!queue) return QUEUE_NULL_POINTER;

    queueClear(queue);

    free(queue);

    return QUEUE_OK;
}

/**
 * @brief Тестирует операции очереди
 */
void testQueue() {
    Queue queue;
    QueueError err = queueInit(&queue);
    assert(err == QUEUE_OK);

    // Проверка добавления элементов
    err = queueEnqueue(&queue, 10);
    assert(err == QUEUE_OK);

    err = queueEnqueue(&queue, 20);
    assert(err == QUEUE_OK);

    int value;

    // Проверка получения первого и последнего элементов
    err = queuePeekFront(&queue, &value);
    assert(err == QUEUE_OK && value == 10);

    err = queuePeekLast(&queue, &value);
    assert(err == QUEUE_OK && value == 20);

    // Проверка удаления элементов
    err = queueDequeue(&queue, &value);
    assert(err == QUEUE_OK && value == 10);

    err = queueDequeue(&queue, &value);
    assert(err == QUEUE_OK && value == 20);

    // Проверка на пустоту очереди
    assert(queueIsEmpty(&queue) == true);

    // Проверка очистки очереди
    err = queueEnqueue(&queue, 30);
    err = queueClear(&queue);
    assert(err == QUEUE_OK && queueIsEmpty(&queue));
}

int main() {
    testQueue();

    printf("All queue tests passed!\n");

    return 0;
}
