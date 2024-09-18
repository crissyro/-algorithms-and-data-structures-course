#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define MAX_SIZE 100

typedef struct string_t {
    char *data;           // Указатель на строку
    unsigned capacity;    // Максимальная емкость строки
    unsigned length;      // Текущая длина строки
} string_t;

typedef string_t *string_t_ptr;

/**
 * @brief Инициализирует строку и выделяет память под нее.
 *
 * @param str Указатель на структуру строки.
 * @param size Максимальный размер строки.
 */
void InitStr(string_t_ptr str, unsigned size) {
    str->data = (char *)malloc((size + 1) * sizeof(char));
    if (!str->data) {
        fprintf(stderr, "Ошибка: невозможно выделить память\n");
        exit(EXIT_FAILURE);
    }
    str->capacity = size;
    str->length = 0;
}

/**
 * @brief Записывает данные в строку str из исходной строки source.
 *
 * @param str Указатель на структуру строки.
 * @param source Исходная строка.
 */
void WriteToStr(string_t_ptr str, const char *source) {
    for (unsigned i = 0; source[i] != '\0'; i++) {
        if (str->length >= str->capacity - 1) {
            fprintf(stderr, "Ошибка: достигнут максимальный размер строки\n");
            return;
        }
        str->data[str->length++] = source[i];
    }
    str->data[str->length] = '\0';
}

/**
 * @brief Удаляет определенное количество символов из строки, начиная с указанной позиции.
 *
 * @param str Указатель на строку.
 * @param start_index Позиция начала удаления.
 * @param count Количество символов для удаления.
 */
void Delete(string_t_ptr str, unsigned start_index, unsigned count) {
    if (start_index >= str->length) {
        fprintf(stderr, "Ошибка: индекс находится за пределами строки\n");
        return;
    }
    if (count == 0) {
        return;
    }
    if (start_index + count > str->length) {
        count = str->length - start_index;
    }
    memmove(str->data + start_index, str->data + start_index + count, str->length - start_index - count + 1);
    str->length -= count;
}

/**
 * @brief Вставляет подстроку substring в строку str, начиная с позиции start_index.
 *
 * @param substring Указатель на подстроку.
 * @param str Указатель на строку.
 * @param start_index Позиция начала вставки.
 */
void Insert(string_t_ptr substring, string_t_ptr str, unsigned start_index) {
    if (start_index > str->length || substring == NULL) {
        fprintf(stderr, "Ошибка: некорректные параметры вставки\n");
        return;
    }

    unsigned new_length = str->length + substring->length;
    if (new_length >= str->capacity) {
        fprintf(stderr, "Ошибка: строка не может вместить новую подстроку\n");
        return;
    }

    memmove(str->data + start_index + substring->length, str->data + start_index, str->length - start_index + 1);
    memcpy(str->data + start_index, substring->data, substring->length);
    str->length = new_length;
}

/**
 * @brief Конкатенирует строки str1 и str2, результат помещает в result.
 *
 * @param str1 Первая строка.
 * @param str2 Вторая строка.
 * @param result Результирующая строка.
 */
void Concat(string_t_ptr str1, string_t_ptr str2, string_t_ptr result) {
    if (str1 == NULL || str2 == NULL || result == NULL) {
        fprintf(stderr, "Ошибка: некорректные параметры для конкатенации\n");
        return;
    }

    unsigned len1 = str1->length;
    unsigned len2 = str2->length;

    if (len1 + len2 >= result->capacity) {
        fprintf(stderr, "Ошибка: результирующая строка не может вместить объединенные строки\n");
        return;
    }

    memcpy(result->data, str1->data, len1);
    memcpy(result->data + len1, str2->data, len2);
    result->length = len1 + len2;
    result->data[result->length] = '\0';
}

/**
 * @brief Копирует символы из строки str в подстроку substring.
 *
 * @param str Исходная строка.
 * @param start_index Начальный индекс копирования.
 * @param count Количество символов для копирования.
 * @param substring Подстрока для записи скопированных данных.
 */
void Copy(string_t_ptr str, unsigned start_index, unsigned count, string_t_ptr substring) {
    if (str == NULL || substring == NULL) {
        fprintf(stderr, "Ошибка: некорректные параметры для копирования\n");
        return;
    }
    if (count >= substring->capacity) {
        fprintf(stderr, "Ошибка: подстрока не может вместить копируемые символы\n");
        return;
    }
    if (start_index >= str->length) {
        fprintf(stderr, "Ошибка: начальный индекс выходит за пределы строки\n");
        return;
    }

    if (start_index + count > str->length) {
        count = str->length - start_index;
    }

    memcpy(substring->data, str->data + start_index, count);
    substring->length = count;
    substring->data[substring->length] = '\0';
}

/**
 * @brief Освобождает динамическую память строки str.
 *
 * @param str Указатель на строку.
 */
void DoneStr(string_t_ptr str) {
    if (str != NULL) {
        free(str->data);
        str->data = NULL;
        str->capacity = 0;
        str->length = 0;
    }
}

/**
 * @brief Пропускает начальные и множественные пробелы в строке.
 * 
 * @param str Указатель на строку.
 * @return Указатель на первый непустой символ строки.
 */
const char* SkipSpaces(const char* str) {
    while (*str == ' ') {
        str++;
    }
    return str;
}

/**
 * @brief Сравнивает две строки с игнорированием множественных пробелов.
 * 
 * @param s1 Первая строка для сравнения.
 * @param s2 Вторая строка для сравнения.
 * 
 * @return 1 если строки равны, 0 если не равны.
 */
bool WordCmp(const char* s1, const char* s2) {
    while (*s1 != '\0' && *s2 != '\0') {
        // Пропускаем множественные пробелы
        s1 = SkipSpaces(s1);
        s2 = SkipSpaces(s2);

        // Если символы не равны, возвращаем 0
        if (*s1 != *s2) {
            return false;
        }

        // Переходим к следующему символу
        s1++;
        s2++;
    }

    return (*s1 == '\0' && *s2 == '\0') ? true : false;
}

void TestInitStr() {
    string_t str;
    InitStr(&str, 10);
    assert(str.capacity == 10);
    assert(str.length == 0);
    assert(str.data != NULL);
    printf("TestInitStr passed!\n");
    DoneStr(&str);
}

void TestWriteToStr() {
    string_t str;
    InitStr(&str, 10);
    WriteToStr(&str, "Hello");
    assert(strcmp(str.data, "Hello") == 0);
    assert(str.length == 5);
    printf("TestWriteToStr passed!\n");
    DoneStr(&str);
}

void TestDelete() {
    string_t str;
    InitStr(&str, 20);
    WriteToStr(&str, "Hello World");
    Delete(&str, 5, 1);
    assert(strcmp(str.data, "HelloWorld") == 0);
    printf("TestDelete passed!\n");
    DoneStr(&str);
}

void TestInsert() {
    string_t str, sub;
    InitStr(&str, 20);
    InitStr(&sub, 10);
    WriteToStr(&str, "Helo World");
    WriteToStr(&sub, "l");
    Insert(&sub, &str, 3);
    assert(strcmp(str.data, "Hello World") == 0);
    printf("TestInsert passed!\n");
    DoneStr(&str);
    DoneStr(&sub);
}

void TestConcat() {
    string_t str1, str2, result;
    InitStr(&str1, 10);
    InitStr(&str2, 10);
    InitStr(&result, 20);

    WriteToStr(&str1, "Hello");
    WriteToStr(&str2, "World");

    Concat(&str1, &str2, &result);
    assert(strcmp(result.data, "HelloWorld") == 0);
    printf("TestConcat passed!\n");

    DoneStr(&str1);
    DoneStr(&str2);
    DoneStr(&result);
}

void TestWordCmp() {
    assert(WordCmp("Hello   world", "Hello world"));
    assert(!WordCmp("Hello   world", "Hello  world!"));
    printf("TestWordCmp passed!\n");
}

int main() {
    TestInitStr();
    TestWriteToStr();
    TestDelete();
    TestInsert();
    TestConcat();
    TestWordCmp();

    printf("All tests passed!\n");

    return 0;
}
