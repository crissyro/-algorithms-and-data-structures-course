#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/// @brief Коды ошибок для операций хеш-таблицы
typedef enum {
    HASH_TABLE_SUCCESS,           ///< Операция выполнена успешно
    HASH_TABLE_KEY_NOT_FOUND,    ///< Ошибка: ключ не найден
    HASH_TABLE_ALLOCATION_ERROR, ///< Ошибка: не удалось выделить память
    HASH_TABLE_ALREADY_EXISTS,   ///< Ошибка: ключ уже существует
    HASH_TABLE_FULL              ///< Ошибка: таблица заполнена
} HashTable_ErrorCode;

/// @brief Узел связного списка для хранения пары ключ-значение
typedef struct HashNode {
    char* key;                    ///< Ключ (строка)
    int value;                    ///< Значение
    struct HashNode* next;        ///< Указатель на следующий узел
} HashNode;

/// @brief Хеш-таблица с разрешением коллизий с помощью связного списка
typedef struct {
    HashNode** buckets;           ///< Массив списков (бакетов)
    size_t size;                  ///< Размер таблицы (количество бакетов)
    size_t count;                 ///< Текущее количество элементов
} HashTable;

/// @brief Инициализация хеш-таблицы
/// @param table Указатель на хеш-таблицу
/// @param size Размер таблицы
void hashTableInit(HashTable* table, size_t size) {
    table->size = size;
    table->count = 0;
    table->buckets = (HashNode**)calloc(size, sizeof(HashNode*));
}

/// @brief Вычисляет улучшенный хеш для строки с использованием алгоритма DJB2
/// @param key Ключ (строка)
/// @param size Размер таблицы
/// @return Индекс бакета для заданного ключа
size_t hashFunction(const char* key, size_t size) {
    size_t hash = 5381;
    int c;

    while ((c = *key++)) hash = ((hash << 5) + hash) + c;  
    
    return hash % size;
}

/// @brief Вставляет новую пару ключ-значение в таблицу, если ключа нет
/// @param table Указатель на хеш-таблицу
/// @param key Ключ
/// @param value Значение
/// @return Код ошибки (успех, уже существует или ошибка выделения)
HashTable_ErrorCode hashTableInsertUnique(HashTable* table, const char* key, int value) {
    size_t index = hashFunction(key, table->size);
    HashNode* node = table->buckets[index];

    while (node) {
        if (strcmp(node->key, key) == 0) return HASH_TABLE_ALREADY_EXISTS;
        
        node = node->next;
    }

    node = (HashNode*)malloc(sizeof(HashNode));

    if (!node) return HASH_TABLE_ALLOCATION_ERROR;

    node->key = strdup(key);

    if (!node->key) {
        free(node);
        return HASH_TABLE_ALLOCATION_ERROR;
    }

    node->value = value;
    node->next = table->buckets[index];
    table->buckets[index] = node;
    table->count++;

    return HASH_TABLE_SUCCESS;
}

/// @brief Проверяет, существует ли ключ в таблице
/// @param table Указатель на хеш-таблицу
/// @param key Ключ
/// @return Код ошибки (успех или ключ не найден)
HashTable_ErrorCode hashTableContains(HashTable* table, const char* key) {
    size_t index = hashFunction(key, table->size);
    HashNode* node = table->buckets[index];

    while (node) {
        if (strcmp(node->key, key) == 0) return HASH_TABLE_SUCCESS;
        
        node = node->next;
    }

    return HASH_TABLE_KEY_NOT_FOUND;
}

/// @brief Возвращает значение по ключу из хеш-таблицы
/// @param table Указатель на хеш-таблицу
/// @param key Ключ
/// @param[out] value Указатель для сохранения значения
/// @return Код ошибки (успех или ключ не найден)
HashTable_ErrorCode hashTableGet(HashTable* table, const char* key, int* value) {
    size_t index = hashFunction(key, table->size);
    HashNode* node = table->buckets[index];

    while (node) {
        if (strcmp(node->key, key) == 0) {
            *value = node->value;
            return HASH_TABLE_SUCCESS;
        }
        node = node->next;
    }

    return HASH_TABLE_KEY_NOT_FOUND;
}

/// @brief Возвращает текущее количество элементов в таблице
/// @param table Указатель на хеш-таблицу
/// @return Количество элементов
size_t hashTableCount(HashTable* table) { return table->count; }

/// @brief Удаляет элемент из хеш-таблицы по ключу
/// @param table Указатель на хеш-таблицу
/// @param key Ключ
/// @return Код ошибки (успех или ключ не найден)
HashTable_ErrorCode hashTableRemove(HashTable* table, const char* key) {
    size_t index = hashFunction(key, table->size);
    HashNode* node = table->buckets[index];
    HashNode* prev = NULL;

    while (node) {
        if (strcmp(node->key, key) == 0) {
            if (prev) {
                prev->next = node->next;
            } else {
                table->buckets[index] = node->next;
            }
            free(node->key);
            free(node);
            table->count--;
            return HASH_TABLE_SUCCESS;
        }
        prev = node;
        node = node->next;
    }
    return HASH_TABLE_KEY_NOT_FOUND;
}

/// @brief Выводит все ключи хеш-таблицы
/// @param table Указатель на хеш-таблицу
/// @return Массив строк с ключами (пользователь должен освободить память)
char** hashTableKeys(HashTable* table) {
    char** keys = (char**)malloc(table->count * sizeof(char*));
    if (!keys) return NULL;

    size_t idx = 0;
    for (size_t i = 0; i < table->size; i++) {
        HashNode* node = table->buckets[i];
        while (node) {
            keys[idx++] = strdup(node->key);
            node = node->next;
        }
    }
    return keys;
}

/// @brief Очистка хеш-таблицы и освобождение памяти
/// @param table Указатель на хеш-таблицу
void hashTableClear(HashTable* table) {
    for (size_t i = 0; i < table->size; i++) {
        HashNode* node = table->buckets[i];
        while (node) {
            HashNode* temp = node;
            node = node->next;
            free(temp->key);
            free(temp);
        }
    }
    free(table->buckets);
    table->buckets = NULL;
    table->count = 0;
}

/// @brief Функция для запуска всех тестов хеш-таблицы
void hashTableTests() {
    HashTable table;
    hashTableInit(&table, 10);
    
    // Тест: Вставка уникальных значений
    assert(hashTableInsertUnique(&table, "apple", 100) == HASH_TABLE_SUCCESS);
    assert(hashTableInsertUnique(&table, "banana", 200) == HASH_TABLE_SUCCESS);
    assert(hashTableInsertUnique(&table, "apple", 300) == HASH_TABLE_ALREADY_EXISTS);

    // Тест: Получение значения по ключу
    int value;
    assert(hashTableGet(&table, "apple", &value) == HASH_TABLE_SUCCESS);
    assert(value == 100);

    assert(hashTableGet(&table, "banana", &value) == HASH_TABLE_SUCCESS);
    assert(value == 200);

    // Тест: Получение значения для несуществующего ключа
    assert(hashTableGet(&table, "cherry", &value) == HASH_TABLE_KEY_NOT_FOUND);

    // Тест: Проверка существования ключа
    assert(hashTableContains(&table, "apple") == HASH_TABLE_SUCCESS);
    assert(hashTableContains(&table, "cherry") == HASH_TABLE_KEY_NOT_FOUND);

    // Тест: Удаление элемента
    assert(hashTableRemove(&table, "apple") == HASH_TABLE_SUCCESS);
    assert(hashTableRemove(&table, "apple") == HASH_TABLE_KEY_NOT_FOUND);

    // Тест: Вставка и проверка нового значения после удаления
    assert(hashTableInsertUnique(&table, "apple", 300) == HASH_TABLE_SUCCESS);
    assert(hashTableGet(&table, "apple", &value) == HASH_TABLE_SUCCESS);
    assert(value == 300);

    // Тест: Количество элементов
    assert(hashTableCount(&table) == 2);

    // Тест: Получение всех ключей
    char** keys = hashTableKeys(&table);
    assert(keys != NULL);
    assert(strcmp(keys[0], "banana") == 0 || strcmp(keys[0], "apple") == 0);
    assert(strcmp(keys[1], "banana") == 0 || strcmp(keys[1], "apple") == 0);
    free(keys[0]);
    free(keys[1]);
    free(keys);

    // Тест: Очистка таблицы
    hashTableClear(&table);
    assert(hashTableCount(&table) == 0);
}

int main() {
    hashTableTests();

    printf("All hash table tests passed!\n");

    return 0;
}

