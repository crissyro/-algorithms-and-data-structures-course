#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Перечисление для кодов ошибок.
 */
typedef enum {
    LIST_OK = 0,                /**< Операция выполнена успешно */
    LIST_MEMORY_ERROR,          /**< Ошибка выделения памяти */
    LIST_EMPTY,                 /**< Список пуст */
    LIST_NOT_FOUND,             /**< Элемент не найден */
    LIST_NULL_POINTER,          /**< Нулевой указатель */
    LIST_INVALID_OPERATION      /**< Некорректная операция */
} ListError;

/**
 * @brief Структура узла списка.
 */
typedef struct Node {
    int data;                   /**< Данные узла */
    struct Node* next;         /**< Указатель на следующий узел */
} Node;

/**
 * @brief Структура для представления списка.
 */
typedef struct {
    Node* head;                /**< Указатель на первый узел списка */
    Node* current;             /**< Указатель на текущий узел */
} List;

/**
 * @brief Инициализирует новый список.
 * @return Указатель на инициализированный список, или NULL в случае ошибки.
 */
List* initList() {
    List* list = (List*)malloc(sizeof(List));
    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->current = NULL;

    return list;
}

/**
 * @brief Добавляет новый узел в конец списка.
 * @param list Указатель на список.
 * @param data Данные, которые нужно сохранить в узле.
 * @return Код ошибки.
 */
ListError putNode(List* list, int data) {
    if (list == NULL) return LIST_NULL_POINTER;

    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) return LIST_MEMORY_ERROR;

    newNode->data = data;
    newNode->next = NULL;

    if (list->head == NULL) {
        list->head = newNode;
    } else {
        Node* temp = list->head;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newNode;
    }

    return LIST_OK;
}

/**
 * @brief Получает данные текущего узла.
 * @param list Указатель на список.
 * @param data Указатель на переменную, в которую будут сохранены данные.
 * @return Код ошибки.
 */
ListError getNode(List* list, int* data) {
    if (list == NULL || data == NULL) return LIST_NULL_POINTER;

    if (list->current == NULL) return LIST_EMPTY;

    *data = list->current->data;

    return LIST_OK;
}

/**
 * @brief Выводит все элементы списка.
 * @param list Указатель на список.
 * @return Код ошибки (всегда LIST_OK).
 */
int readList(List* list) {
    if (list == NULL) 
        return LIST_NULL_POINTER; 
    
    Node* temp = list->head;

    while (temp != NULL) {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }

    printf("NULL\n"); 
    
    return LIST_OK;
}

/**
 * @brief Проверяет, пуст ли список.
 * @param list Указатель на список.
 * @return 1, если список пуст; 0 в противном случае.
 */
int isEmptyList(List* list) { 
    return (list == NULL || list->head == NULL); 
}

/**
 * @brief Подсчитывает количество узлов в списке.
 * @param list Указатель на список.
 * @return Количество узлов в списке.
 */
int count(List* list) {
    if (list == NULL) return 0;
    
    int count = 0;
    Node* temp = list->head;

    while (temp != NULL) {
        count++;
        temp = temp->next;
    }

    return count;
}

/**
 * @brief Возвращает указатель на первый узел списка.
 * @param list Указатель на список.
 * @return Указатель на первый узел, или NULL, если список пуст.
 */
Node* beginPtr(List* list) {
    if (list == NULL) return NULL;
    
    return list->head;
}

/**
 * @brief Возвращает указатель на последний узел списка.
 * @param list Указатель на список.
 * @return Указатель на последний узел, или NULL, если список пуст.
 */
Node* endPtr(List* list) {
    if (list == NULL) return NULL;
    
    Node* temp = list->head;

    while (temp && temp->next != NULL) temp = temp->next;
    
    return temp;
}

/**
 * @brief Перемещает указатель current на следующий узел.
 * @param list Указатель на список.
 * @return Код ошибки.
 */
ListError movePtr(List* list) {
    if (list == NULL || list->current == NULL) return LIST_NULL_POINTER;
    
    list->current = list->current->next;

    return LIST_OK;
}

/**
 * @brief Перемещает указатель current на узел с заданным индексом.
 * @param list Указатель на список.
 * @param index Индекс узла, на который нужно переместить указатель.
 * @return Код ошибки.
 */
ListError moveTo(List* list, int index) {
    if (list == NULL) return LIST_NULL_POINTER;
    
    list->current = list->head;

    for (int i = 0; i < index && list->current != NULL; i++) 
        list->current = list->current->next;
    
    if (list->current == NULL) return LIST_NOT_FOUND;
    
    return LIST_OK;
}

/**
 * @brief Создает копию списка.
 * @param list Указатель на оригинальный список.
 * @return Указатель на новый список, или NULL в случае ошибки.
 */
List* copyList(List* list) {
    if (list == NULL) return NULL;
    
    List* newList = initList();

    if (newList == NULL) return NULL;
    
    Node* temp = list->head;

    while (temp != NULL) {
        putNode(newList, temp->data);
        temp = temp->next;
    }

    return newList;
}

/**
 * @brief Освобождает память, занятую списком.
 * @param list Указатель на список.
 */
void freeList(List* list) {
    if (list == NULL) return;

    Node* temp;
    
    while (list->head != NULL) {
        temp = list->head;
        list->head = list->head->next;
        free(temp);
    }

    free(list);
}
