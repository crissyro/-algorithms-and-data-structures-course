#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/// @file
/// @brief Моделирование вычислительной системы с дискретным временем.

#define MAX_TASKS 100      ///< Максимальное количество задач в очередях и стеке
#define MAX_PROCESSORS 2   ///< Максимальное количество процессоров
#define NAME_SIZE 16

/// @brief Тип ошибки для обработки очередей и стеков
typedef enum {
    SUCCESS = 0,  ///< Успех
    QUEUE_FULL,   ///< Очередь переполнена
    QUEUE_EMPTY,  ///< Очередь пуста
    STACK_FULL,   ///< Стек переполнен
    STACK_EMPTY   ///< Стек пуст
} ErrorCode;

/// @brief Структура задачи
typedef struct {
    char name[NAME_SIZE]; ///< Имя задачи
    int time;      ///< Время обработки
} Task;

/// @brief Структура очереди
typedef struct {
    Task tasks[MAX_TASKS]; ///< Массив задач
    int index_first_element, index_last_element, size; ///< Индексы начала, конца и размер очереди
} Queue;

/// @brief Структура стека
typedef struct {
    Task tasks[MAX_TASKS]; ///< Массив задач
    int top;               ///< Индекс верхнего элемента стека
} Stack;

/// @brief Структура процессора
typedef struct {
    bool isBusy;    ///< Занятость процессора
    Task *currentTask; ///< Текущая задача
} Processor;

/// Макросы для работы с очередью
#define initQueue(q)    ((q)->index_first_element = 0, \
                         (q)->index_last_element = -1, \
                         (q)->size = 0)

#define isQueueEmpty(q) ((q)->size == 0)
#define isQueueFull(q)  ((q)->size == MAX_TASKS)

/// Макросы для работы со стеком
#define initStack(s)    ((s)->top = -1)
#define isStackEmpty(s) ((s)->top == -1)
#define isStackFull(s)  ((s)->top == (MAX_TASKS - 1))

/// Макросы для работы с процессором
#define initProcessor(p) ((p)->isBusy = false, (p)->currentTask = NULL)

/// @brief Добавление задачи в очередь
/// @param q Указатель на очередь
/// @param task Задача
/// @return Код ошибки
ErrorCode queueAddTask(Queue *q, Task task) {
    if (isQueueFull(q)) return QUEUE_FULL;
    
    q->index_last_element = (q->index_last_element + 1) % MAX_TASKS;
    q->tasks[q->index_last_element] = task;
    q->size++;

    return SUCCESS;
}

/// @brief Удаление задачи из очереди
/// @param q Указатель на очередь
/// @return Код ошибки и задача
ErrorCode queueRemoveTask(Queue *q, Task *task) {
    if (isQueueEmpty(q)) return QUEUE_EMPTY;
    
    *task = q->tasks[q->index_first_element];
    q->index_first_element = (q->index_first_element + 1) % MAX_TASKS;
    q->size--;

    return SUCCESS;
}

/// @brief Добавление задачи в стек
/// @param s Указатель на стек
/// @param task Задача
/// @return Код ошибки
ErrorCode stackPush(Stack *s, Task task) {
    if (isStackFull(s)) return STACK_FULL;
    
    s->tasks[++(s->top)] = task;

    return SUCCESS;
}

/// @brief Удаление задачи из стека
/// @param s Указатель на стек
/// @return Код ошибки и задача
ErrorCode stackPop(Stack *s, Task *task) {
    if (isStackEmpty(s)) return STACK_EMPTY;
    
    *task = s->tasks[(s->top)--];
    
    return SUCCESS;
}

/// @brief Основной цикл моделирования
/// @param N Количество шагов моделирования
/// @param F1 Очередь 1
/// @param F2 Очередь 2
/// @param S Стек
/// @param P1 Процессор 1
/// @param P2 Процессор 2
void simulate(int N, Queue *F1, Queue *F2, Stack *S, Processor *P1, Processor *P2) {
    for (int time = 0; time <= N; time++) {
        printf("Время: %d\n", time);

        Task task;

        if (!P1->isBusy && !isQueueEmpty(F1)) {
            queueRemoveTask(F1, &task);
            P1->currentTask = &task;
            P1->isBusy = true;
        }

        if (!P2->isBusy && !isQueueEmpty(F2)) {
            queueRemoveTask(F2, &task);
            P2->currentTask = &task;
            P2->isBusy = true;
        }

        if (P1->isBusy) {
            P1->currentTask->time--;
            if (P1->currentTask->time == 0) {
                P1->isBusy = false;
                P1->currentTask = NULL;
            }
        }

        if (P2->isBusy) {
            P2->currentTask->time--;
            if (P2->currentTask->time == 0) {
                P2->isBusy = false;
                P2->currentTask = NULL;
            }
        }

        printf("Очередь F1: %s\n", isQueueEmpty(F1) ? "пусто" : "задачи есть");
        printf("Очередь F2: %s\n", isQueueEmpty(F2) ? "пусто" : "задачи есть");
        printf("Процессор P1: %s\n", P1->isBusy ? "занят" : "свободен");
        printf("Процессор P2: %s\n", P2->isBusy ? "занят" : "свободен");
        printf("Стек: %s\n\n", isStackEmpty(S) ? "пусто" : "есть задачи");
    }
}

int main() {
    Queue F1, F2;
    Stack S;
    Processor P1, P2;

    initQueue(&F1);
    initQueue(&F2);
    initStack(&S);
    initProcessor(&P1);
    initProcessor(&P2);

    Task tasks[12] = {
        {"T1", 3}, {"T2", 5}, {"T3", 2}, {"T4", 4}, {"T5", 6}, 
        {"T6", 3}, {"T7", 7}, {"T8", 2}, {"T9", 1}, {"T10", 4},
        {"T11", 3}, {"T12", 5}
    };

    for (int i = 0; i < 6; i++) {
        queueAddTask(&F1, tasks[i]);
    }
    for (int i = 6; i < 12; i++) {
        queueAddTask(&F2, tasks[i]);
    }

    simulate(10, &F1, &F2, &S, &P1, &P2);

    return 0;
}
