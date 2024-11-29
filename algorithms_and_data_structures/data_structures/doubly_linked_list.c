#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

/// @brief Коды ошибок для операций с двусвязным списком
typedef enum {
    DOUBLY_LINKED_LIST_OK,            ///< Операция выполнена успешно
    DOUBLY_LINKED_LIST_NULL_POINTER,  ///< Указатель на список равен NULL
    DOUBLY_LINKED_LIST_EMPTY,         ///< Список пуст
    DOUBLY_LINKED_LIST_MEMORY_ERROR   ///< Ошибка выделения памяти
} DoublyLinkedListError;

/// @brief Узел двусвязного списка
typedef struct Node {
    int data;                   ///< Данные узла
    struct Node* next;          ///< Указатель на следующий узел
    struct Node* prev;          ///< Указатель на предыдущий узел
} Node;

/// @brief Структура двусвязного списка
typedef struct {
    Node* head;                 ///< Указатель на первый элемент списка
    Node* tail;                 ///< Указатель на последний элемент списка
    int size;                   ///< Текущий размер списка
} DoublyLinkedList;

/**
 * @brief Инициализирует пустой двусвязный список
 * @param list Указатель на структуру списка
 * @return Код ошибки (DOUBLY_LINKED_LIST_OK при успешной инициализации)
 */
DoublyLinkedListError doublyLinkedListInit(DoublyLinkedList* list) {
    if (!list) return DOUBLY_LINKED_LIST_NULL_POINTER;

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return DOUBLY_LINKED_LIST_OK;
}

/**
 * @brief Добавляет элемент в начало списка
 * @param list Указатель на структуру списка
 * @param value Значение для добавления
 * @return Код ошибки (DOUBLY_LINKED_LIST_OK при успешной вставке)
 */
DoublyLinkedListError doublyLinkedListPushFront(DoublyLinkedList* list, int value) {
    if (!list) return DOUBLY_LINKED_LIST_NULL_POINTER;

    Node* newNode = (Node*)malloc(sizeof(Node));
    
    if (!newNode) return DOUBLY_LINKED_LIST_MEMORY_ERROR;

    newNode->data = value;
    newNode->next = list->head;
    newNode->prev = NULL;

    if (list->head) list->head->prev = newNode;

    else list->tail = newNode;
    
    list->head = newNode;
    list->size++;

    return DOUBLY_LINKED_LIST_OK;
}

/**
 * @brief Добавляет элемент в конец списка
 * @param list Указатель на структуру списка
 * @param value Значение для добавления
 * @return Код ошибки (DOUBLY_LINKED_LIST_OK при успешной вставке)
 */
DoublyLinkedListError doublyLinkedListPushBack(DoublyLinkedList* list, int value) {
    if (!list) return DOUBLY_LINKED_LIST_NULL_POINTER;

    Node* newNode = (Node*)malloc(sizeof(Node));

    if (!newNode) return DOUBLY_LINKED_LIST_MEMORY_ERROR;

    newNode->data = value;
    newNode->next = NULL;
    newNode->prev = list->tail;

    if (list->tail) list->tail->next = newNode;

    else list->head = newNode;
    

    list->tail = newNode;
    list->size++;

    return DOUBLY_LINKED_LIST_OK;
}

/**
 * @brief Удаляет элемент из начала списка
 * @param list Указатель на структуру списка
 * @param value Указатель для сохранения удалённого значения
 * @return Код ошибки (DOUBLY_LINKED_LIST_OK при успешном удалении, DOUBLY_LINKED_LIST_EMPTY если список пуст)
 */
DoublyLinkedListError doublyLinkedListPopFront(DoublyLinkedList* list, int* value) {
    if (!list) return DOUBLY_LINKED_LIST_NULL_POINTER;

    if (!list->head) return DOUBLY_LINKED_LIST_EMPTY;

    Node* temp = list->head;
    *value = temp->data;

    list->head = list->head->next;

    if (list->head) list->head->prev = NULL;

    else list->tail = NULL;
    

    free(temp);

    list->size--;

    return DOUBLY_LINKED_LIST_OK;
}

/**
 * @brief Удаляет элемент из конца списка
 * @param list Указатель на структуру списка
 * @param value Указатель для сохранения удалённого значения
 * @return Код ошибки (DOUBLY_LINKED_LIST_OK при успешном удалении, DOUBLY_LINKED_LIST_EMPTY если список пуст)
 */
DoublyLinkedListError doublyLinkedListPopBack(DoublyLinkedList* list, int* value) {
    if (!list) return DOUBLY_LINKED_LIST_NULL_POINTER;

    if (!list->tail) return DOUBLY_LINKED_LIST_EMPTY;

    Node* temp = list->tail;
    *value = temp->data;

    list->tail = list->tail->prev;

    if (list->tail) list->tail->next = NULL;

    else list->head = NULL;
    

    free(temp);

    list->size--;

    return DOUBLY_LINKED_LIST_OK;
}

/**
 * @brief Проверяет, пуст ли список
 * @param list Указатель на структуру списка
 * @return true, если список пуст, иначе false
 */
bool doublyLinkedListIsEmpty(DoublyLinkedList* list) { return list && list->head == NULL; }

/**
 * @brief Возвращает текущий размер списка
 * @param list Указатель на структуру списка
 * @return Текущий размер списка
 */
int doublyLinkedListSize(DoublyLinkedList* list) { return list ? list->size : -1; }

/**
 * @brief Очищает список, удаляя все элементы
 * @param list Указатель на структуру списка
 * @return Код ошибки (DOUBLY_LINKED_LIST_OK при успешной очистке)
 */
DoublyLinkedListError doublyLinkedListClear(DoublyLinkedList* list) {
    if (!list) return DOUBLY_LINKED_LIST_NULL_POINTER;

    Node* current = list->head;
    while (current) {
        Node* next = current->next;
        free(current);
        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return DOUBLY_LINKED_LIST_OK;
}

/**
 * @brief Удаляет структуру списка и освобождает память
 * @param list Указатель на структуру списка
 * @return Код ошибки (DOUBLY_LINKED_LIST_OK при успешном удалении)
 */
DoublyLinkedListError doublyLinkedListDelete(DoublyLinkedList* list) {
    if (!list) return DOUBLY_LINKED_LIST_NULL_POINTER;

    doublyLinkedListClear(list);

    free(list);

    return DOUBLY_LINKED_LIST_OK;
}

/**
 * @brief Тестирует операции двусвязного списка
 */
void testDoublyLinkedList() {
    DoublyLinkedList list;
    DoublyLinkedListError err = doublyLinkedListInit(&list);
    assert(err == DOUBLY_LINKED_LIST_OK);

    // Проверка добавления элементов в начало и конец списка
    err = doublyLinkedListPushFront(&list, 10);
    assert(err == DOUBLY_LINKED_LIST_OK);

    err = doublyLinkedListPushBack(&list, 20);
    assert(err == DOUBLY_LINKED_LIST_OK);

    err = doublyLinkedListPushFront(&list, 5);
    assert(err == DOUBLY_LINKED_LIST_OK);

    int value;

    // Проверка удаления элементов из начала списка
    err = doublyLinkedListPopFront(&list, &value);
    assert(err == DOUBLY_LINKED_LIST_OK && value == 5);

    // Проверка удаления элементов из конца списка
    err = doublyLinkedListPopBack(&list, &value);
    assert(err == DOUBLY_LINKED_LIST_OK && value == 20);

    // Очистка списка и проверка на пустоту
    err = doublyLinkedListClear(&list);
    assert(err == DOUBLY_LINKED_LIST_OK && doublyLinkedListIsEmpty(&list));
}

int main() {
    testDoublyLinkedList();

    printf("All doubly linked list tests passed!\n");

    return 0;
}
