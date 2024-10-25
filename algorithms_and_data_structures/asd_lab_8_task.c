#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100
#define TABLE_SIZE 200

/**
 * @struct Node
 * Структура для хранения пар (ключ-значение) в хэш-таблице.
 */
typedef struct Node {
    char word[MAX_LENGTH];
    int value;
    struct Node *next;
} Node;

/**
 * @struct HashTable
 * Структура для хэш-таблицы.
 */
typedef struct HashTable {
    Node **table;
} HashTable;

/**
 * @brief Создает хэш-таблицу.
 * @return Указатель на созданную хэш-таблицу.
 */
HashTable *createHashTable() {
    HashTable *hashTable = malloc(sizeof(HashTable));
    hashTable->table = malloc(sizeof(Node *) * TABLE_SIZE);

    for (int i = 0; i < TABLE_SIZE; i++) 
        hashTable->table[i] = NULL;
    

    return hashTable;
}

/**
 * @brief Хэш-функция.
 * @param word Строка, для которой нужно вычислить хэш.
 * @return Индекс в хэш-таблице.
 */
unsigned int hash(const char *word) {
    unsigned int hashValue = 0;

    while (*word) 
        hashValue = (hashValue << 5) + *word++;
    

    return hashValue % TABLE_SIZE;
}

/**
 * @brief Добавляет пару (ключ-значение) в хэш-таблицу.
 * @param hashTable Указатель на хэш-таблицу.
 * @param word Ключ, который нужно добавить.
 * @param value Значение, соответствующее ключу.
 */
void hashTableInsert(HashTable *hashTable, const char *word, int value) {
    unsigned int index = hash(word);
    Node *newNode = malloc(sizeof(Node));
    strcpy(newNode->word, word);
    newNode->value = value;
    newNode->next = hashTable->table[index];
    hashTable->table[index] = newNode;
}

/**
 * @brief Поиск значения по ключу в хэш-таблице.
 * @param hashTable Указатель на хэш-таблицу.
 * @param word Ключ, который нужно найти.
 * @return Значение, соответствующее ключу, или -1, если не найдено.
 */
int hashTableSearch(HashTable *hashTable, const char *word) {
    unsigned int index = hash(word);
    Node *node = hashTable->table[index];
    
    while (node) {
        if (strcmp(node->word, word) == 0) {
            return node->value;
        }
        node = node->next;
    }

    return -1;
}

/**
 * @brief Освобождает память, занятую хэш-таблицей.
 * @param hashTable Указатель на хэш-таблицу.
 */
void freeHashTable(HashTable *hashTable) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *node = hashTable->table[i];

        while (node) {
            Node *temp = node;
            node = node->next;
            free(temp);
        }
    }

    free(hashTable->table);
    free(hashTable);
}

/**
 * @brief Создает словарь чисел, загружая его из файла.
 * @param filename Имя файла со словарем.
 * @param hashTable Указатель на хэш-таблицу.
 * @return 0 в случае успеха, -1 в случае ошибки.
 */
int createNumberDict(const char *filename, HashTable *hashTable) {
    FILE *file = fopen(filename, "r");

    if (!file) {
        perror("Ошибка при открытии файла");
        return -1;
    }

    char word[MAX_LENGTH];
    int value;

    while (fscanf(file, "%s %d", word, &value) == 2) 
        hashTableInsert(hashTable, word, value);
    
    fclose(file);

    return 0;
}

/**
 * @brief Обрабатывает текст, заменяя словесные числа на числовые.
 * @param hashTable Указатель на хэш-таблицу.
 * @param inputFilename Имя входного файла.
 * @param outputFilename Имя выходного файла.
 */
void processText(HashTable *hashTable, const char *inputFilename, const char *outputFilename) {
    FILE *inputFile = fopen(inputFilename, "r");
    FILE *outputFile = fopen(outputFilename, "w");

    if (!inputFile || !outputFile) {
        perror("Ошибка при открытии файла");
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), inputFile)) {
        char *token = strtok(line, " ");
        int sumNumbers = 0;
        int hasNumber = 0;

        while (token) {
            int value = hashTableSearch(hashTable, token);
            if (value != -1) {
                sumNumbers += value;
                hasNumber = 1;
            } else {
                if (hasNumber) {
                    fprintf(outputFile, "%d ", sumNumbers);
                    sumNumbers = 0;
                    hasNumber = 0;
                }
                fprintf(outputFile, "%s ", token);
            }

            token = strtok(NULL, " ");
        }

        if (hasNumber) fprintf(outputFile, "%d ", sumNumbers);
        
        fprintf(outputFile, "\n");
    }

    fclose(inputFile);
    fclose(outputFile);
}

/**
 * @brief Основная функция программы.
 * @return Код завершения.
 */
int main() {
    HashTable *hashTable = createHashTable();

    createNumberDict("numbers.txt", hashTable);
    processText(hashTable, "input.txt", "output.txt");

    printf("Обработка завершена. Результат записан в output.txt.\n");

    freeHashTable(hashTable);

    return 0;
}
