#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 256 // Максимальное количество уникальных символов в строке

// Узел дерева Хаффмана
typedef struct HuffmanNode {
    char symbol;              // Символ
    int freq;                 // Частота символа
    struct HuffmanNode* left; // Левый потомок
    struct HuffmanNode* right;// Правый потомок
} HuffmanNode;

// Очередь для узлов дерева
typedef struct PriorityQueue {
    HuffmanNode** nodes;  // Массив указателей на узлы
    int size;             // Текущий размер очереди
    int capacity;         // Вместимость очереди
} PriorityQueue;

/**
 * @brief Создает новый узел дерева Хаффмана.
 * @param symbol Символ.
 * @param freq Частота символа.
 * @return Указатель на созданный узел.
 */
HuffmanNode* create_node(char symbol, int freq) {
    HuffmanNode* node = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    
    node->symbol = symbol;
    node->freq = freq;
    node->left = NULL;
    node->right = NULL;

    return node;
}

/**
 * @brief Создает приоритетную очередь.
 * @param capacity Вместимость очереди.
 * @return Указатель на созданную очередь.
 */
PriorityQueue* create_priority_queue(int capacity) {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->nodes = (HuffmanNode**)malloc(sizeof(HuffmanNode*) * capacity);
    pq->size = 0;
    pq->capacity = capacity;
    return pq;
}

/**
 * @brief Вставляет узел в приоритетную очередь.
 * @param pq Указатель на очередь.
 * @param node Указатель на узел.
 */
void enqueue(PriorityQueue* pq, HuffmanNode* node) {
    int i = pq->size++;

    while (i > 0 && pq->nodes[(i - 1) / 2]->freq > node->freq) {
        pq->nodes[i] = pq->nodes[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    pq->nodes[i] = node;
}

/**
 * @brief Удаляет и возвращает узел с минимальной частотой.
 * @param pq Указатель на очередь.
 * @return Указатель на узел с минимальной частотой.
 */
HuffmanNode* dequeue(PriorityQueue* pq) {
    HuffmanNode* min = pq->nodes[0];
    pq->nodes[0] = pq->nodes[--pq->size];
    int i = 0, smallest = 0;

    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < pq->size && pq->nodes[left]->freq < pq->nodes[smallest]->freq) 
            smallest = left;
        
        if (right < pq->size && pq->nodes[right]->freq < pq->nodes[smallest]->freq) 
            smallest = right;
        
        if (smallest == i) break;

        HuffmanNode* temp = pq->nodes[i];
        pq->nodes[i] = pq->nodes[smallest];
        pq->nodes[smallest] = temp;

        i = smallest;
    }

    return min;
}

/**
 * @brief Строит дерево Хаффмана.
 * @param freq Частоты символов.
 * @param n Количество уникальных символов.
 * @return Указатель на корень дерева Хаффмана.
 */
HuffmanNode* build_huffman_tree(int* freq, int n) {
    PriorityQueue* pq = create_priority_queue(n);

    for (int i = 0; i < MAX_SYMBOLS; ++i) 
        if (freq[i] > 0) enqueue(pq, create_node(i, freq[i]));
    

    while (pq->size > 1) {
        HuffmanNode* left = dequeue(pq);
        HuffmanNode* right = dequeue(pq);
        HuffmanNode* merged = create_node('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;
        enqueue(pq, merged);
    }

    HuffmanNode* root = dequeue(pq);
    free(pq->nodes);
    free(pq);
    return root;
}

/**
 * @brief Генерирует коды Хаффмана для символов.
 * @param root Указатель на корень дерева.
 * @param code Текущий код (рекурсивный параметр).
 * @param depth Текущая глубина (рекурсивный параметр).
 */
void generate_huffman_codes(HuffmanNode* root, char** codes, char* code, int depth) {
    if (!root->left && !root->right) {
        code[depth] = '\0';
        codes[(unsigned char)root->symbol] = strdup(code);
        return;
    }
    if (root->left) {
        code[depth] = '0';
        generate_huffman_codes(root->left, codes, code, depth + 1);
    }
    if (root->right) {
        code[depth] = '1';
        generate_huffman_codes(root->right, codes, code, depth + 1);
    }
}

/**
 * @brief Освобождает память, занятую деревом.
 * @param root Указатель на корень дерева.
 */
void free_huffman_tree(HuffmanNode* root) {
    if (root) {
        free_huffman_tree(root->left);
        free_huffman_tree(root->right);
        free(root);
    }
}

int main() {
    const char* text = "мама мыла раму";
    int freq[MAX_SYMBOLS] = {0};

    for (const char* p = text; *p; ++p) freq[(unsigned char)*p]++;
    
    // Строим дерево Хаффмана
    HuffmanNode* root = build_huffman_tree(freq, strlen(text));

    char* codes[MAX_SYMBOLS] = {0};
    char code[256];
    generate_huffman_codes(root, codes, code, 0);

    printf("Коды Хаффмана:\n");
    for (int i = 0; i < MAX_SYMBOLS; ++i) {
        if (codes[i]) {
            printf("%c: %s\n", i, codes[i]);
            free(codes[i]);
        }
    }

    free_huffman_tree(root);

    return 0;
}
