#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

/// @brief Коды ошибок для операций со списком
typedef enum {
    LIST_OK,           ///< Операция выполнена успешно
    LIST_NULL_POINTER,     ///< Указатель на список равен NULL
    LIST_EMPTY,            ///< Список пуст
    LIST_NOT_FOUND,        ///< Элемент не найден
    LIST_MEMORY_ERROR      ///< Ошибка выделения памяти
} ListError;

/// @brief Узел односвязного списка
typedef struct Node {
    int data;              ///< Данные узла
    struct Node* next;     ///< Указатель на следующий узел
} Node;

/// @brief Структура односвязного списка
typedef struct {
    Node* head;            ///< Указатель на первый элемент списка
} LinkedList;

/**
 * @brief Инициализирует пустой список
 * @param list Указатель на структуру списка
 * @return Код ошибки (LIST_OK при успешной инициализации)
 */
ListError listInit(LinkedList* list) {
    if (!list) return LIST_NULL_POINTER;

    list->head = NULL;

    return LIST_OK;
}

/**
 * @brief Вставка элемента в начало списка
 * @param list Указатель на структуру списка
 * @param value Значение для вставки
 * @return Код ошибки (LIST_OK при успешной вставке)
 */
ListError listInsertFront(LinkedList* list, int value) {
    if (!list) return LIST_NULL_POINTER;

    Node* newNode = (Node*)malloc(sizeof(Node));

    if (!newNode) return LIST_MEMORY_ERROR;

    newNode->data = value;
    newNode->next = list->head;
    list->head = newNode;

    return LIST_OK;
}

/**
 * @brief Поиск элемента в списке
 * @param list Указатель на структуру списка
 * @param value Значение для поиска
 * @param err Указатель на переменную для возврата кода ошибки
 * @return Указатель на найденный узел (NULL, если элемент не найден)
 */
Node* listFindElement(LinkedList* list, int value, ListError* err) {
    if (!list) {
        *err = LIST_NULL_POINTER;
        return NULL;
    }

    Node* current = list->head;

    while (current) {

        if (current->data == value) {
            *err = LIST_OK;
            return current;
        }

        current = current->next;
    }

    *err = LIST_NOT_FOUND;

    return NULL;
}

/**
 * @brief Вставка элемента правее указанного
 * @param list Указатель на структуру списка
 * @param target Значение элемента, правее которого будет вставка
 * @param value Значение для вставки
 * @return Код ошибки (LIST_OK при успешной вставке)
 */
ListError listInsertAfter(LinkedList* list, int target, int value) {
    if (!list) return LIST_NULL_POINTER;
    
    ListError err;

    Node* targetNode = listFindElement(list, target, &err);

    if (err != LIST_OK) return err;

    Node* newNode = (Node*)malloc(sizeof(Node));

    if (!newNode) return LIST_MEMORY_ERROR;

    newNode->data = value;
    newNode->next = targetNode->next;
    targetNode->next = newNode;

    return LIST_OK;
}

/**
 * @brief Вставка элемента левее указанного
 * @param list Указатель на структуру списка
 * @param target Значение элемента, левее которого будет вставка
 * @param value Значение для вставки
 * @return Код ошибки (LIST_OK при успешной вставке)
 */
ListError listInsertBefore(LinkedList* list, int target, int value) {
    if (!list) return LIST_NULL_POINTER;

    if (!list->head) return LIST_EMPTY;

    if (list->head->data == target) return listInsertFront(list, value);
    
    Node* current = list->head;

    while (current->next && current->next->data != target) current = current->next;
    

    if (!current->next) return LIST_NOT_FOUND;

    Node* newNode = (Node*)malloc(sizeof(Node));

    if (!newNode) return LIST_MEMORY_ERROR;

    newNode->data = value;
    newNode->next = current->next;
    current->next = newNode;

    return LIST_OK;
}

/**
 * @brief Удаление элемента из списка
 * @param list Указатель на структуру списка
 * @param value Значение элемента, который нужно удалить
 * @return Код ошибки (LIST_OK при успешном удалении)
 */
ListError listRemoveElement(LinkedList* list, int value) {
    if (!list) return LIST_NULL_POINTER;

    if (!list->head) return LIST_EMPTY;

    Node* current = list->head;
    Node* prev = NULL;

    while (current && current->data != value) {
        prev = current;
        current = current->next;
    }

    if (!current) return LIST_NOT_FOUND;

    if (prev) prev->next = current->next;

    else list->head = current->next;

    free(current);

    return LIST_OK;
}

/// @brief Проверка на пустоту списка
/// @param list Указатель на структуру списка
/// @return true, если список пуст, иначе false
bool listIsEmpty(LinkedList* list) { return list && list->head == NULL; }

/// @brief Проверка, содержит ли список указанный элемент
/// @param list Указатель на структуру списка
/// @param value Значение элемента для поиска
/// @param err Указатель на переменную для возврата кода ошибки
/// @return true, если элемент найден, иначе false
bool listContains(LinkedList* list, int value, ListError* err) {
    if (!list) {
        *err = LIST_NULL_POINTER;
        return false;
    }

    Node* current = list->head;
    
    while (current) {
        if (current->data == value) {
            *err = LIST_OK;
            return true;
        }

        current = current->next;
    }

    *err = LIST_NOT_FOUND;

    return false;
}

/// @brief Проверка, является ли указанный узел последним в списке
/// @param node Указатель на узел списка
/// @return true, если узел последний, иначе false
bool isLastNode(Node* node) { return node && node->next == NULL; }

/**
 * @brief Очистка списка, удаление всех элементов
 * @param list Указатель на структуру списка
 * @return Код ошибки (LIST_OK при успешной очистке)
 */
ListError listClear(LinkedList* list) {
    if (!list) return LIST_NULL_POINTER;

    Node* current = list->head;

    while (current) {
        Node* next = current->next;
        free(current);
        current = next;
    }

    list->head = NULL;

    return LIST_OK;
}

/**
 * @brief Удаление списка и освобождение памяти
 * @param list Указатель на структуру списка
 * @return Код ошибки (LIST_OK при успешном удалении)
 */
ListError listDelete(LinkedList* list) {
    if (!list) return LIST_NULL_POINTER;

    listClear(list);

    return LIST_OK;
}

void testLinkedList() {
    LinkedList list;
    ListError err = listInit(&list);
    assert(err == LIST_OK);

    err = listInsertFront(&list, 10);
    assert(err == LIST_OK);
    assert(list.head->data == 10);

    err = listInsertAfter(&list, 10, 20);
    assert(err == LIST_OK);
    assert(list.head->next->data == 20);

    err = listInsertBefore(&list, 10, 5);
    assert(err == LIST_OK);
    assert(list.head->data == 5);

    Node* foundNode = listFindElement(&list, 20, &err);
    assert(err == LIST_OK && foundNode->data == 20);

    err = listRemoveElement(&list, 10);
    assert(err == LIST_OK);
    assert(list.head->next->data == 20);

    err = listClear(&list);
    assert(err == LIST_OK);
    assert(list.head == NULL);

    err = listInsertFront(&list, 30);
    err = listInsertFront(&list, 40);
    assert(err == LIST_OK);

    err = listDelete(&list);
    assert(err == LIST_OK);
}

int main() {
    testLinkedList();

    printf("All tests passed!\n");

    return 0;
}
