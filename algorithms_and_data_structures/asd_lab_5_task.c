#include <stdio.h>
#include <stdlib.h>


/**
 * @brief Структура узла списка для хранения коэффициента.
 */
typedef struct Node {
    int coefficient;           /**< Коэффициент многочлена */
    int power;                /**< Степень переменной */
    struct Node* next;        /**< Указатель на следующий узел */
} Node;

/**
 * @brief Структура для представления списка многочлена.
 */
typedef struct {
    Node* head;               /**< Указатель на первый узел списка */
} PolynomialList;

/**
 * @brief Создает новый узел.
 * @param coefficient Коэффициент узла.
 * @param power Степень переменной.
 * @return Указатель на созданный узел.
 */
Node* createNode(int coefficient, int power) {
    Node* newNode = (Node*)malloc(sizeof(Node));

    if (!newNode) return NULL; 
    
    newNode->coefficient = coefficient;
    newNode->power = power;
    newNode->next = NULL;

    return newNode;
}

/**
 * @brief Инициализирует список многочлена.
 * @return Указатель на инициализированный список.
 */
PolynomialList* initPolynomialList() {
    PolynomialList* list = (PolynomialList*)malloc(sizeof(PolynomialList));

    if (!list) return NULL; 
    
    list->head = NULL;
    return list;
}

/**
 * @brief Добавляет новый узел в список многочлена.
 * @param list Указатель на список многочлена.
 * @param coefficient Коэффициент.
 * @param power Степень переменной.
 */
void addTerm(PolynomialList* list, int coefficient, int power) {
    if (coefficient == 0) return;  

    Node* newNode = createNode(coefficient, power);

    if (!newNode) return; 

    if (list->head == NULL || list->head->power < power) {
        newNode->next = list->head;
        list->head = newNode;
        return;
    }

    Node* current = list->head;

    while (current->next != NULL && current->next->power > power) 
        current = current->next;
    

    newNode->next = current->next;
    current->next = newNode;
}

/**
 * @brief Печатает многочлен в формате "a_n*x^n + a_(n-1)*x^(n-1) + ... + a_0".
 * @param list Указатель на список многочлена.
 */
void printPolynomial(PolynomialList* list) {
    Node* current = list->head;
    if (current == NULL) {
        printf("0\n"); 
        return;
    }

    while (current != NULL) {
        if (current->coefficient > 0 && current != list->head) {
            printf(" + ");
        }
        printf("%d*x^%d", current->coefficient, current->power);
        current = current->next;
    }
    printf("\n");
}

/**
 * @brief Освобождает память, занятую списком многочлена.
 * @param list Указатель на список многочлена.
 */
void freePolynomialList(PolynomialList* list) {
    Node* current = list->head;
    Node* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    free(list);
}

int main() {
    PolynomialList* polyList = initPolynomialList();

    addTerm(polyList, 3, 4); // 3*x^4
    addTerm(polyList, 0, 2); // Игнорируется
    addTerm(polyList, 2, 3); // 2*x^3
    addTerm(polyList, 1, 0); // 1
    addTerm(polyList, 4, 1); // 4*x^1

    printPolynomial(polyList);

    freePolynomialList(polyList);
    
    return 0;
}
