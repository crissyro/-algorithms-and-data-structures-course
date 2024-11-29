#include <stdio.h>
#include <stdlib.h>

/** @enum GraphError
 *  @brief Ошибки при работе с графом.
 */
typedef enum {
    GRAPH_SUCCESS = 0,       /**< Успешное выполнение операции */
    GRAPH_ERR_MEMORY,        /**< Ошибка выделения памяти */
    GRAPH_ERR_INVALID_NODE,  /**< Неверный узел */
    GRAPH_ERR_EDGE_EXISTS,   /**< Ребро уже существует */
    GRAPH_ERR_EDGE_NOT_FOUND /**< Ребро не найдено */
} GraphError;

/** @struct Graph
 *  @brief Структура для представления графа как списка смежности.
 *  
 *  @var Graph::num_nodes
 *  Количество вершин в графе.
 *  
 *  @var Graph::adj_list
 *  Массив указателей на список смежности для каждой вершины.
 */
typedef struct Graph {
    int num_nodes;          /**< Количество вершин в графе */
    struct Node** adj_list; /**< Массив указателей на список смежности */
} Graph;

/** @struct Node
 *  @brief Узел списка смежности, представляющий рёбра.
 *  
 *  @var Node::node
 *  Индекс вершины, с которой связано ребро.
 *  
 *  @var Node::next
 *  Указатель на следующий узел списка смежности.
 */
typedef struct Node {
    int node;           /**< Индекс вершины */
    struct Node* next;  /**< Указатель на следующий узел */
} Node;

static void dfs_helper(Graph* graph, int start, int* visited);
static Node* create_node(int node);
static void free_list(Node* head);

/**
 * @brief Создаёт граф с заданным количеством вершин.
 * @param num_nodes Количество вершин в графе.
 * @return Указатель на созданный граф.
 */
Graph* create_graph(int num_nodes) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));

    if (!graph) return NULL;
    

    graph->num_nodes = num_nodes;
    graph->adj_list = (Node**)malloc(num_nodes * sizeof(Node*));
    if (!graph->adj_list) {
        free(graph);
        return NULL;
    }

    for (int i = 0; i < num_nodes; i++) graph->adj_list[i] = NULL;
    
    return graph;
}

/**
 * @brief Освобождает память, выделенную для графа.
 * @param graph Указатель на граф.
 */
void destroy_graph(Graph* graph) {
    if (graph) {
        for (int i = 0; i < graph->num_nodes; i++) free_list(graph->adj_list[i]);
        
        free(graph->adj_list);
        free(graph);
    }
}

/**
 * @brief Добавляет ребро в граф.
 * @param graph Указатель на граф.
 * @param src Индекс исходной вершины.
 * @param dest Индекс целевой вершины.
 * @return Код ошибки выполнения.
 */
GraphError add_edge(Graph* graph, int src, int dest) {
    if (src >= graph->num_nodes || dest >= graph->num_nodes) return GRAPH_ERR_INVALID_NODE;
    
    Node* new_node = create_node(dest);

    if (!new_node) return GRAPH_ERR_MEMORY;
    
    new_node->next = graph->adj_list[src];
    graph->adj_list[src] = new_node;

    // Проверяем, не существует ли уже такое ребро в обратном направлении
    Node* temp = graph->adj_list[src];
    while (temp) {
        if (temp->node == dest) return GRAPH_ERR_EDGE_EXISTS;
        
        temp = temp->next;
    }

    return GRAPH_SUCCESS;
}

/**
 * @brief Удаляет ребро из графа.
 * @param graph Указатель на граф.
 * @param src Индекс исходной вершины.
 * @param dest Индекс целевой вершины.
 * @return Код ошибки выполнения.
 */
GraphError remove_edge(Graph* graph, int src, int dest) {
    if (src >= graph->num_nodes || dest >= graph->num_nodes) return GRAPH_ERR_INVALID_NODE;
    
    Node* temp = graph->adj_list[src];
    Node* prev = NULL;

    while (temp != NULL && temp->node != dest) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) return GRAPH_ERR_EDGE_NOT_FOUND;
    
    if (prev == NULL) graph->adj_list[src] = temp->next;

    else prev->next = temp->next;
    
    free(temp);

    return GRAPH_SUCCESS;
}

/**
 * @brief Выполняет поиск в глубину (DFS) для графа.
 * @param graph Указатель на граф.
 * @param start Индекс начальной вершины.
 */
void dfs(Graph* graph, int start) {
    int* visited = (int*)malloc(graph->num_nodes * sizeof(int));

    if (!visited) return;
    
    for (int i = 0; i < graph->num_nodes; i++) visited[i] = 0;
    
    dfs_helper(graph, start, visited);
    free(visited);
}

/**
 * @brief Рекурсивная вспомогательная функция для DFS.
 * @param graph Указатель на граф.
 * @param start Индекс текущей вершины.
 * @param visited Массив посещённых вершин.
 */
static void dfs_helper(Graph* graph, int start, int* visited) {
    visited[start] = 1;
    printf("%d ", start);

    Node* adj_list = graph->adj_list[start];
    while (adj_list) {
        if (!visited[adj_list->node]) {
            dfs_helper(graph, adj_list->node, visited);
        }
        adj_list = adj_list->next;
    }
}

/**
 * @brief Выполняет поиск в ширину (BFS) для графа.
 * @param graph Указатель на граф.
 * @param start Индекс начальной вершины.
 */
void bfs(Graph* graph, int start) {
    int* visited = (int*)malloc(graph->num_nodes * sizeof(int));

    if (!visited) return;
    
    for (int i = 0; i < graph->num_nodes; i++) visited[i] = 0;

    visited[start] = 1;
    printf("BFS: %d ", start);

    Node* queue = create_node(start);
    Node* tail = queue;

    while (queue) {
        int current = queue->node;
        Node* adj_list = graph->adj_list[current];

        while (adj_list) {
            if (!visited[adj_list->node]) {
                visited[adj_list->node] = 1;
                tail->next = create_node(adj_list->node);
                tail = tail->next;
                printf("%d ", adj_list->node);
            }
            adj_list = adj_list->next;
        }

        Node* temp = queue;
        queue = queue->next;
        free(temp);
    }

    free(visited);
}

/**
 * @brief Создаёт новый узел списка смежности.
 * @param node Индекс вершины.
 * @return Указатель на созданный узел.
 */
static Node* create_node(int node) {
    Node* new_node = (Node*)malloc(sizeof(Node));

    if (!new_node) return NULL;
    
    new_node->node = node;
    new_node->next = NULL;
    return new_node;
}

/**
 * @brief Освобождает память, выделенную для списка смежности.
 * @param head Указатель на голову списка.
 */
static void free_list(Node* head) {
    while (head) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }
}

