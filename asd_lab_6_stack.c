#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/** @file
 *  @brief Реализация стека.
 */

#define MAX 100  ///< Максимальный размер стека

/// Определение ошибок
typedef enum {
    STACK_OK = 0,         ///< Операция успешна
    STACK_OVERFLOW = -1,  ///< Стек переполнен
    STACK_UNDERFLOW = -2  ///< Стек пуст
} StackError;

typedef struct {
    int items[MAX];  ///< Массив для хранения элементов стека
    int top;         ///< Индекс верхнего элемента стека
} Stack;

/// Инициализация стека
#define initStack(s) ((s)->top = -1)

/// Проверка, пуст ли стек
#define isStackEmpty(s) ((s)->top == -1)

/// Проверка, полон ли стек
#define isStackFull(s) ((s)->top == MAX - 1)

/**
 * @brief Добавление элемента в стек
 * 
 * @param s указатель на стек
 * @param value значение, которое нужно добавить
 * @return StackError результат операции (STACK_OK или STACK_OVERFLOW)
 */
StackError stackPush(Stack *s, int value) {
    if (isStackFull(s)) return STACK_OVERFLOW;

    s->items[++(s->top)] = value;
    
    return STACK_OK;
}

/**
 * @brief Извлечение элемента из стека
 * 
 * @param s указатель на стек
 * @param result указатель на переменную, куда будет записан извлеченный элемент
 * @return StackError результат операции (STACK_OK или STACK_UNDERFLOW)
 */
StackError stackPop(Stack *s, int *result) {
    if (isStackEmpty(s)) return STACK_UNDERFLOW;

    *result = s->items[(s->top)--];

    return STACK_OK;
}

/**
 * @brief Получение верхнего элемента стека без его удаления
 * 
 * @param s указатель на стек
 * @param result указатель на переменную, куда будет записан верхний элемент
 * @return StackError результат операции (STACK_OK или STACK_UNDERFLOW)
 */
StackError stackGetTopElement(Stack *s, int *result) {
    if (isStackEmpty(s)) return STACK_UNDERFLOW;
    
    *result = s->items[s->top];

    return STACK_OK;
}

void stackTest() {
    Stack stack;
    initStack(&stack);

    int result;
    StackError err;

    err = stackPush(&stack, 5);
    assert(err == STACK_OK);

    err = stackPush(&stack, 10);
    assert(err == STACK_OK);

    err = stackGetTopElement(&stack, &result);
    assert(err == STACK_OK && result == 10);

    err = stackPop(&stack, &result);
    assert(err == STACK_OK && result == 10);

    err = stackPop(&stack, &result);
    assert(err == STACK_OK && result == 5);

    for (int i = 0; i < MAX; i++) {
        err = stackPush(&stack, i);
        assert(err == STACK_OK);
    }

    err = stackPush(&stack, 100);
    assert(err == STACK_OVERFLOW);

    for (int i = 0; i < MAX; i++) {
        err = stackPop(&stack, &result);
        assert(err == STACK_OK);
    }

    err = stackPop(&stack, &result);
    assert(err == STACK_UNDERFLOW);
}

int main() {
    stackTest();

    printf("Все тесты пройдены успешно!\n");

    return 0;
}