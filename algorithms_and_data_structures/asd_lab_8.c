#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define TABLE_SIZE 100  ///< Размер хеш-таблицы
#define PRIME 7         ///< Простое число для вторичного хеширования

/**
 * @brief Структура элемента таблицы.
 */
typedef struct {
    int key;      ///< Ключ элемента.
    int value;    ///< Значение элемента.
} HashItem;

/**
 * @brief Структура хеш-таблицы.
 */
typedef struct {
    HashItem** items;  ///< Массив указателей на элементы.
    int size;          ///< Размер таблицы.
} HashTable;

/**
 * @brief Перечисление кодов ошибок для операций с хеш-таблицей.
 */
typedef enum {
    HT_SUCCESS,          ///< Успешное выполнение операции.
    HT_KEY_NOT_FOUND,    ///< Ключ не найден в таблице.
    HT_TABLE_FULL,       ///< Таблица заполнена.
    HT_MEMORY_ERROR,     ///< Ошибка выделения памяти.
    HT_KEY_EXISTS        ///< Ключ уже существует в таблице.
} HashTableStatus;

/**
 * @brief Функция первичного хеширования.
 * @param key Ключ.
 * @return Индекс в таблице.
 */
int primaryHash(int key) { return key % TABLE_SIZE; }

/**
 * @brief Функция вторичного хеширования.
 * @param key Ключ.
 * @return Шаг для двойного хеширования.
 */
int secondaryHash(int key) { return PRIME - (key % PRIME); }

/**
 * @brief Инициализация хеш-таблицы.
 * @param size Размер таблицы.
 * @return Указатель на созданную таблицу.
 */
HashTable* createTable(int size) {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));

    if (!table) return NULL;

    table->size = size;
    table->items = (HashItem**)calloc(size, sizeof(HashItem*));

    return table;
}

/**
 * @brief Освобождение памяти, занятой хеш-таблицей.
 * @param table Указатель на хеш-таблицу.
 */
void freeTable(HashTable* table) {
    if (!table) return;

    for (int i = 0; i < table->size; i++) 
        if (table->items[i]) free(table->items[i]);
    
    free(table->items);
    free(table);
}

/**
 * @brief Вставка элемента в хеш-таблицу.
 * @param table Указатель на таблицу.
 * @param key Ключ элемента.
 * @param value Значение элемента.
 * @return Код состояния операции.
 */
HashTableStatus insertItem(HashTable* table, int key, int value) {
    if (!table) return HT_MEMORY_ERROR;

    int index = primaryHash(key);
    int step = secondaryHash(key);

    for (int i = 0; i < table->size; i++) {
        int newIndex = (index + i * step) % table->size;
        
        if (!table->items[newIndex]) { // Пустая ячейка
            table->items[newIndex] = (HashItem*)malloc(sizeof(HashItem));

            if (!table->items[newIndex]) return HT_MEMORY_ERROR;

            table->items[newIndex]->key = key;
            table->items[newIndex]->value = value;

            return HT_SUCCESS;

        } else if (table->items[newIndex]->key == key) 
            return HT_KEY_EXISTS;
        
    }

    return HT_TABLE_FULL;
}

/**
 * @brief Поиск элемента в хеш-таблице.
 * @param table Указатель на таблицу.
 * @param key Ключ элемента.
 * @param value Указатель для хранения найденного значения.
 * @return Код состояния операции.
 */
HashTableStatus findItem(HashTable* table, int key, int* value) {
    if (!table) return HT_MEMORY_ERROR;

    int index = primaryHash(key);
    int step = secondaryHash(key);

    for (int i = 0; i < table->size; i++) {
        int newIndex = (index + i * step) % table->size;
        
        if (table->items[newIndex] && table->items[newIndex]->key == key) {
            *value = table->items[newIndex]->value;

            return HT_SUCCESS;
        }
    }

    return HT_KEY_NOT_FOUND;
}

/**
 * @brief Удаление элемента из хеш-таблицы.
 * @param table Указатель на таблицу.
 * @param key Ключ элемента.
 * @return Код состояния операции.
 */
HashTableStatus deleteItem(HashTable* table, int key) {
    if (!table) return HT_MEMORY_ERROR;

    int index = primaryHash(key);
    int step = secondaryHash(key);

    for (int i = 0; i < table->size; i++) {
        int newIndex = (index + i * step) % table->size;
        
        if (table->items[newIndex] && table->items[newIndex]->key == key) {
            free(table->items[newIndex]);

            table->items[newIndex] = NULL;

            return HT_SUCCESS;
        }
    }

    return HT_KEY_NOT_FOUND;
}

/**
 * @brief Вывод хеш-таблицы в консоль.
 * @param table Указатель на таблицу.
 */
void printTable(const HashTable* table) {
    if (!table) {
        printf("Хеш-таблица не инициализирована.\n");
        return;
    }

    printf("\n------------------ Хеш-таблица ------------------\n");
    printf("|  Индекс  |       Ключ       |     Значение    |\n");
    printf("|----------|------------------|-----------------|\n");

    for (int i = 0; i < table->size; i++) {
        if (table->items[i] != NULL) {
            printf("| %8d | %15d | %13d  |\n", i, table->items[i]->key, table->items[i]->value);
        } else {
            printf("| %8d |       ---       |      Пусто     |\n", i);
        }
    }

    printf("------------------------------------------------\n\n");
}

/**
 * @brief Тест функции вставки.
 */
void testInsertItem() {
    HashTable* table = createTable(TABLE_SIZE);
    assert(table != NULL);

    printf("Тест вставки:\n");

    assert(insertItem(table, 1, 100) == HT_SUCCESS);
    assert(insertItem(table, 2, 200) == HT_SUCCESS);

    assert(insertItem(table, 1, 300) == HT_KEY_EXISTS);

    for (int i = 0; i < TABLE_SIZE; i++) 
        insertItem(table, i + 3, i * 10);
    
    assert(insertItem(table, 999, 500) == HT_TABLE_FULL);

    printTable(table);

    freeTable(table);

    printf("Тест вставки пройден.\n\n");
}

/**
 * @brief Тест функции поиска.
 */
void testFindItem() {
    HashTable* table = createTable(TABLE_SIZE);
    assert(table != NULL);

    int value;
    printf("Тест поиска:\n");

    insertItem(table, 1, 100);
    assert(findItem(table, 1, &value) == HT_SUCCESS);
    assert(value == 100);

    assert(findItem(table, 99, &value) == HT_KEY_NOT_FOUND);

    insertItem(table, TABLE_SIZE + 1, 200);
    assert(findItem(table, TABLE_SIZE + 1, &value) == HT_SUCCESS);
    assert(value == 200);

    printTable(table);

    freeTable(table);

    printf("Тест поиска пройден.\n\n");
}

/**
 * @brief Тест функции удаления.
 */
void testDeleteItem() {
    HashTable* table = createTable(TABLE_SIZE);
    assert(table != NULL);

    printf("Тест удаления:\n");

    insertItem(table, 1, 100);
    assert(deleteItem(table, 1) == HT_SUCCESS);

    assert(deleteItem(table, 2) == HT_KEY_NOT_FOUND);
    assert(deleteItem(table, 1) == HT_KEY_NOT_FOUND);

    printTable(table);

    freeTable(table);

    printf("Тест удаления пройден.\n\n");
}

/**
 * @brief Главная функция для выполнения тестов.
 */
int main() {

    testInsertItem();
    testFindItem();
    testDeleteItem();

    printf("Все тесты пройдены.\n");
    
    return 0;
}
