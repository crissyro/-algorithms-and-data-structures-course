#include <iostream>
#include <vector>
#include <stdexcept>
#include <unordered_set>
#include <cstring>
#include <chrono>

#define MAX_VERTICES 7

// Класс бинарной матрицы
template <typename T>
class BinMatrix {
private:
    unsigned short rows, cols;
    std::vector<std::vector<T>> matrix;

public:
    // Конструктор
    BinMatrix(unsigned short rows, unsigned short cols) {
        if (rows <= 0 || cols <= 0)
            throw std::invalid_argument("Недопустимые размеры матрицы");
        this->rows = rows;
        this->cols = cols;
        matrix.resize(rows, std::vector<T>(cols, 0));
    }

    ~BinMatrix() = default;

    // Инициализация матрицы из двумерного массива
    void initFrom2DArray(const T* array, size_t arrayRows, size_t arrayCols) {
        if (arrayRows != rows || arrayCols != cols)
            throw std::invalid_argument("Размеры массива не совпадают с размерами матрицы");
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                matrix[i][j] = array[i * cols + j];
            }
        }
    }

    // Получение количества строк
    unsigned short getRows() const {
        return rows;
    }

    // Получение количества столбцов
    unsigned short getCols() const {
        return cols;
    }

    // Вывод матрицы
    void printMatrix() const {
        for (const auto& row : matrix) {
            for (auto val : row) {
                std::cout << val << " ";
            }
            std::cout << "\n";
        }
    }

    // Доступ к элементам
    const std::vector<T>& operator[](size_t index) const {
        return matrix[index];
    }

    std::vector<T>& operator[](size_t index) {
        return matrix[index];
    }
};

// Проверка: Является ли последовательность маршрутом
bool graphIsRoute(const std::vector<int>& pos, const BinMatrix<int>& graf) {
    for (size_t i = 1; i < pos.size(); ++i)
        if (!graf[pos[i - 1] - 1][pos[i] - 1]) return false;

    return true;
}

// Проверка: Является ли последовательность цепью
bool graphIsChain(const std::vector<int>& pos, const BinMatrix<int>& graf) {
    if (!graphIsRoute(pos, graf)) return false;

    BinMatrix<int> tempGraf(graf.getRows(), graf.getCols());

    for (size_t i = 0; i < pos.size() - 1; ++i) {
        int u = pos[i] - 1;
        int v = pos[i + 1] - 1;

        if (tempGraf[u][v] || tempGraf[v][u]) return false;

        else {
            tempGraf[u][v] = 1;
            tempGraf[v][u] = 1;
        }
    }

    return true;
}

// Проверка: Является ли последовательность простой цепью
// 
//bool graphIsSimpleChain(const std::vector<int>& pos, const BinMatrix<int>& graf) {
//     if (!graphIsChain(pos, graf)) return false;
//
//     for (size_t i = 0; i < pos.size() - 1; ++i) 
//         for (size_t j = i + 1; j < pos.size(); ++j) if (pos[i] == pos[j]) return false;
//
//     return true;
// }

bool graphIsSimpleChain(const std::vector<int>& pos, const BinMatrix<int>& graf) {
    // Проверяем, является ли граф цепью
    if (!graphIsChain(pos, graf)) return false;

    // Проверяем уникальность вершин
    std::unordered_set<int> visited;
    for (int vertex : pos) {
        if (!visited.insert(vertex).second) 
            return false; // Если вершина уже была посещена
    }

    return true; 
}


// Проверка: Является ли последовательность циклом
bool graphIsCycle(const std::vector<int>& pos, const BinMatrix<int>& graf) {
    if (!graphIsChain(pos, graf)) return false;

    return pos.front() == pos.back();
}

// Проверка: Является ли последовательность простым циклом
bool graphIsSimpleCycle(const std::vector<int>& pos, const BinMatrix<int>& graf) {
    if (!graphIsCycle(pos, graf)) return false;

    for (size_t i = 1; i < pos.size() - 1; ++i)
        for (size_t j = i + 1; j < pos.size() - 1; ++j) if (pos[i] == pos[j]) return false;

    return true;
}

// Функция для получения смежных вершин
void graphGetAdjacentVertices(const BinMatrix<int>& graf, int vertex, std::vector<int>& vertices) {
    vertices.clear();
    for (int i = 0; i < graf.getCols(); ++i)
        if (graf[vertex - 1][i]) vertices.push_back(i + 1);
}

// Рекурсивная функция для поиска маршрутов заданной длины
void graphGetRoutes_(int l, std::vector<int>& currRoute, int currRouteLength, std::vector<bool>& visited,
    const BinMatrix<int>& graf, int& routeCount) {
    if (currRouteLength == l) {
        routeCount++;
        std::cout << "{ ";
        for (int v : currRoute)
            std::cout << v << " ";

        std::cout << "} \n";
        return;
    }

    std::vector<int> adjacentVertices;
    graphGetAdjacentVertices(graf, currRoute.back(), adjacentVertices);

    for (int nextVertex : adjacentVertices) {
        if (!visited[nextVertex - 1]) {
            visited[nextVertex - 1] = true;
            currRoute.push_back(nextVertex);
            graphGetRoutes_(l, currRoute, currRouteLength + 1, visited, graf, routeCount);
            currRoute.pop_back();
            visited[nextVertex - 1] = false;
        }
    }
}

// Функция для поиска всех маршрутов заданной длины из одной вершины
void graphGetRoutes(const BinMatrix<int>& graf, int vertex, int n) {
    if (vertex <= 0 || vertex > graf.getRows()) {
        return;
    }

    std::vector<bool> visited(graf.getRows(), false);
    std::vector<int> startingRoute = { vertex };
    visited[vertex - 1] = true;

    int routeCount = 0;
    graphGetRoutes_(n, startingRoute, 1, visited, graf, routeCount);

    //std::cout << "\nВсего маршрутов длины " << n << " из вершины " << vertex << ": " << routeCount << "\n";
}

void graphGetAdjacentVertices(int graf[MAX_VERTICES][MAX_VERTICES], int vertex, int vertices[], int* count) {
    *count = 0;
    for (int i = 0; i < MAX_VERTICES; ++i)
        if (graf[vertex - 1][i]) vertices[(*count)++] = i + 1;

}

void graphGetRoutesAmount(int currentVertex, int routeLength, int currentRoute[MAX_VERTICES], int routeCount[MAX_VERTICES], int graf[MAX_VERTICES][MAX_VERTICES], int graphSize) {
    int adjacentVertices[MAX_VERTICES];
    int adjacentCount;
    graphGetAdjacentVertices(graf, currentRoute[currentVertex], adjacentVertices, &adjacentCount);

    for (int i = 0; i < adjacentCount; ++i) {
        currentRoute[currentVertex + 1] = adjacentVertices[i];

        if (currentVertex + 2 == routeLength + 1)
            routeCount[currentRoute[currentVertex + 1] - 1]++;

        else
            graphGetRoutesAmount(currentVertex + 1, routeLength, currentRoute, routeCount, graf, graphSize);

    }
}

void graphGetRoutesAmountWrapper(int graf[MAX_VERTICES][MAX_VERTICES], int graphSize, int routeLength) {
    int routeCount[MAX_VERTICES];
    int currentRoute[MAX_VERTICES];

    for (int i = 0; i < graphSize; ++i) {
        currentRoute[0] = i + 1;

        for (int j = 0; j < graphSize; ++j) routeCount[j] = 0;

        graphGetRoutesAmount(0, routeLength, currentRoute, routeCount, graf, graphSize);

        for (int j = 0; j < graphSize; ++j)
            std::cout << routeCount[j] << " ";

        std::cout << "\n";
    }
    std::cout << "\n";
}

// Функция для поиска маршрутов между двумя вершинами графа
void graphGetRoutesBetweenVertices(int l, int vertexEnd, int currRoute[], int* currRouteSize, int routes[MAX_VERTICES][MAX_VERTICES], int* routesCount, int graf[MAX_VERTICES][MAX_VERTICES]) {
    int adjacentVertices[MAX_VERTICES];
    int adjacentCount;

    graphGetAdjacentVertices(graf, currRoute[*currRouteSize - 1],
        adjacentVertices, &adjacentCount);

    for (int j = 0; j < adjacentCount; j++) {
        currRoute[*currRouteSize] = adjacentVertices[j];
        (*currRouteSize)++;

        if (*currRouteSize == l + 1) {
            if (currRoute[*currRouteSize - 1] == vertexEnd) {
                for (int k = 0; k < *currRouteSize; k++)
                    routes[*routesCount][k] = currRoute[k];

                (*routesCount)++;
            }
        }
        else {
            graphGetRoutesBetweenVertices(l, vertexEnd, currRoute, currRouteSize,
                routes, routesCount, graf);
        }

        (*currRouteSize)--;
    }
}


// Функция для поиска всех простых максимальных цепей в графе
//void printAllSimplyChainMaximum(int graf[MAX_VERTICES][MAX_VERTICES], int vertex,
//    int sizeRoute, int size, int* route, int* minSize) {
//     route[size] = vertex;
//
//     for (int i = 0; i < MAX_VERTICES; i++) {
//         bool isContinue = true;
//         for (int j = 0; j < size; j++) {
//             if (route[j] == i + 1) {
//                 isContinue = false;
//                 break;
//             }
//         }
//
//         if (isContinue) {
//             if (graf[vertex - 1][i]) 
//                 printAllSimplyChainMaximum(graf, i + 1, sizeRoute, size + 1, route, minSize);
//         }
//     }
//
//     if (size >= *minSize) {
//         *minSize = size;
//         for (int k = 0; k < size; k++) 
//             std::cout << route[k] << " ";
//
//         std::cout << std::endl;
//     }
// }

void printAllSimplyChainMaximum(int graf[MAX_VERTICES][MAX_VERTICES], int vertex,
    int size, int* route, bool* visited, int& maxSize) {
    // Добавляем текущую вершину в маршрут
    route[size] = vertex;
    visited[vertex - 1] = true;

    bool isDeadEnd = true; // Флаг, что текущую вершину больше нельзя продолжать

    // Проходим по всем соседям текущей вершины
    for (int i = 0; i < MAX_VERTICES; ++i) {
        if (graf[vertex - 1][i] && !visited[i]) {
            isDeadEnd = false; // У вершины есть ещё не посещённые соседи
            printAllSimplyChainMaximum(graf, i + 1, size + 1, route, visited, maxSize);
        }
    }

    // Если достигли конца (нет непосещённых соседей), выводим маршрут
    if (isDeadEnd) {
        if (size + 1 >= maxSize) {
            maxSize = size + 1;
            for (int k = 0; k <= size; ++k)
                std::cout << route[k] << " ";
            std::cout << std::endl;
        }
    }

    // Откатываем состояние
    visited[vertex - 1] = false;
}

// Обёртка для запуска алгоритма
void findAllMaxSimpleChains(int graf[MAX_VERTICES][MAX_VERTICES], int startVertex) {
    int route[MAX_VERTICES];
    bool visited[MAX_VERTICES];
    int maxSize = 0;

    std::memset(visited, false, sizeof(visited));

    printAllSimplyChainMaximum(graf, startVertex, 0, route, visited, maxSize);
}

void task3G1() {
    BinMatrix<int> G1(MAX_VERTICES, MAX_VERTICES);

    int matrix[MAX_VERTICES][MAX_VERTICES] = {
        {0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 1, 1},
        {0, 0, 0, 1, 1, 1, 0},
        {0, 0, 1, 0, 1, 0, 0},
        {0, 0, 1, 1, 0, 1, 0},
        {0, 1, 1, 0, 1, 0, 1},
        {1, 1, 0, 0, 0, 1, 0}
    };

    G1.initFrom2DArray(reinterpret_cast<int*>(matrix), MAX_VERTICES, MAX_VERTICES);

    std::cout << "Граф G1:" << std::endl;

    G1.printMatrix();

    std::cout << std::endl;

    std::vector<std::vector<int>> sequence = { {3, 5, 6, 7, 1, 2},  {3, 6, 7, 1, 2, 6, 5}, {4, 3, 6, 5, 4}, {5, 5, 7, 6, 3, 5}, {5, 3, 6, 7, 1, 6, 5} };

    for (int i = 0; i < sequence.size(); i++) {
        std::cout << std::endl;

        std::cout << "Последовательность вершин: ";
        for (int j = 0; j < sequence[i].size(); j++)
            std::cout << sequence[i][j] << " ";

        std::cout << std::endl;
        std::cout << "Маршрут: " << graphIsRoute(sequence[i], G1) << "\n";
        std::cout << "Цепь: " << graphIsChain(sequence[i], G1) << "\n";
        std::cout << "Простая цепь: " << graphIsSimpleChain(sequence[i], G1) << "\n";
        std::cout << "Цикл: " << graphIsCycle(sequence[i], G1) << "\n";
        std::cout << "Простой цикл: " << graphIsSimpleCycle(sequence[i], G1) << "\n";
    }

    std::cout << std::endl;
}

void task3G2() {
    BinMatrix<int> G2(MAX_VERTICES, MAX_VERTICES);

    int matrix[MAX_VERTICES][MAX_VERTICES] = {
        { 0, 1, 0, 1, 0, 1, 1 },
        { 1, 0, 1, 1, 0, 0, 0 },
        { 0, 1, 0, 1, 1, 1, 0 },
        { 1, 1, 1, 0, 0, 0, 0 },
        { 0, 0, 1, 0, 0, 1, 0 },
        { 1, 0, 1, 0, 1, 0, 1 },
        { 1, 0, 0, 0, 0, 1, 0 }

    };

    G2.initFrom2DArray(reinterpret_cast<int*>(matrix), MAX_VERTICES, MAX_VERTICES);

    std::cout << "Граф G2:" << std::endl;

    G2.printMatrix();

    std::cout << std::endl;

    std::vector<std::vector<int>> sequence = { {3, 5, 6, 7, 1, 2},  {3, 6, 7, 1, 2, 6, 5}, {4, 3, 6, 5, 4}, {5, 5, 7, 6, 3, 5}, {5, 3, 6, 7, 1, 6, 5} };

    for (int i = 0; i < sequence.size(); i++) {
        std::cout << std::endl;

        std::cout << "Последовательность вершин: ";
        for (int j = 0; j < sequence[i].size(); j++)
            std::cout << sequence[i][j] << " ";

        std::cout << std::endl;
        std::cout << "Маршрут: " << graphIsRoute(sequence[i], G2) << "\n";
        std::cout << "Цепь: " << graphIsChain(sequence[i], G2) << "\n";
        std::cout << "Простая цепь: " << graphIsSimpleChain(sequence[i], G2) << "\n";
        std::cout << "Цикл: " << graphIsCycle(sequence[i], G2) << "\n";
        std::cout << "Простой цикл: " << graphIsSimpleCycle(sequence[i], G2) << "\n";
    }

    std::cout << std::endl;
}

void task3() {
    task3G1();
    task3G2();
}

void task4() {
    BinMatrix<int> G1(MAX_VERTICES, MAX_VERTICES);
    int matrix1[MAX_VERTICES][MAX_VERTICES] = {
        {0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 1, 1},
        {0, 0, 0, 1, 1, 1, 0},
        {0, 0, 1, 0, 1, 0, 0},
        {0, 0, 1, 1, 0, 1, 0},
        {0, 1, 1, 0, 1, 0, 1},
        {1, 1, 0, 0, 0, 1, 0}
    };

    G1.initFrom2DArray(reinterpret_cast<int*>(matrix1), MAX_VERTICES, MAX_VERTICES);

    std::cout << "Граф G1:\n";
    G1.printMatrix();

    std::cout << std::endl;
    int len = 10;
    std::cout << "Все маршруты в графе G1 длинной " << len << " : " << std::endl;

    for (int i = 1; i <= MAX_VERTICES; ++i) {
        graphGetRoutes(G1, i, len);
    }

    std::cout << std::endl;

    BinMatrix<int> G2(MAX_VERTICES, MAX_VERTICES);
    int matrix2[MAX_VERTICES][MAX_VERTICES] = {
        {0, 1, 0, 1, 0, 1, 1},
        {1, 0, 1, 1, 0, 0, 0},
        {0, 1, 0, 1, 1, 1, 0},
        {1, 1, 1, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 1, 0},
        {1, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 1, 0}
    };
    G2.initFrom2DArray(reinterpret_cast<int*>(matrix2), MAX_VERTICES, MAX_VERTICES);

    std::cout << "\nГраф G2:\n";
    G2.printMatrix();

    std::cout << std::endl;;
    std::cout << "Все маршруты в графе G2 длинной " << len << " : " << std::endl;

    for (int i = 1; i <= MAX_VERTICES; ++i) {
        graphGetRoutes(G2, i, len);
    }

    std::cout << std::endl;
}

void task5() {
    int graf1[MAX_VERTICES][MAX_VERTICES] = {
        {0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 1, 1},
        {0, 0, 0, 1, 1, 1, 0},
        {0, 0, 1, 0, 1, 0, 0},
        {0, 0, 1, 1, 0, 1, 0},
        {0, 1, 1, 0, 1, 0, 1},
        {1, 1, 0, 0, 0, 1, 0}
    };

    int graf2[MAX_VERTICES][MAX_VERTICES] = {
        {0, 1, 0, 1, 0, 1, 1},
        {1, 0, 1, 1, 0, 0, 0},
        {0, 1, 0, 1, 1, 1, 0},
        {1, 1, 1, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 1, 0},
        {1, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 1, 0}
    };

    int length = 3;
    //std::cout << "Введите длину: ";
    //std::cin >> length;

    std::cout << "\nВ графе G1:\n";
    graphGetRoutesAmountWrapper(graf1, MAX_VERTICES, length);

    std::cout << "В графе G2:\n";
    graphGetRoutesAmountWrapper(graf2, MAX_VERTICES, length);
}

// Умножение двух матриц размера n x n
void multiplyMatrices(const std::vector<std::vector<int>>& mat1, const std::vector<std::vector<int>>& mat2,
    std::vector<std::vector<int>>& result, int n) {
    std::vector<std::vector<int>> temp(n, std::vector<int>(n, 0));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k)
                temp[i][j] += mat1[i][k] * mat2[k][j];
        }
    }

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) result[i][j] = temp[i][j];
}

// Возведение матрицы в степень
void powerMatrix(std::vector<std::vector<int>>& matrix, int n, int power) {
    std::vector<std::vector<int>> result(n, std::vector<int>(n, 0));

    for (int i = 0; i < n; ++i) result[i][i] = 1;

    std::vector<std::vector<int>> base = matrix;

    while (power > 0) {
        if (power % 2 == 1) multiplyMatrices(result, base, result, n);

        multiplyMatrices(base, base, base, n);
        power /= 2;
    }

    matrix = result;
}

void graphGetRoutesAmountMatrix(const std::vector<std::vector<int>>& graf, int graphSize, int routeLength) {
    std::vector<std::vector<int>> adjMatrix = graf;

    // Возводим матрицу в степень routeLength
    powerMatrix(adjMatrix, graphSize, routeLength);

    // Вывод количества маршрутов
    for (int i = 0; i < graphSize; ++i) {
        for (int j = 0; j < graphSize; ++j) {
            std::cout << adjMatrix[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void task5Alternative() {
    std::vector<std::vector<int>> graf1 = {
        {0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 1, 1},
        {0, 0, 0, 1, 1, 1, 0},
        {0, 0, 1, 0, 1, 0, 0},
        {0, 0, 1, 1, 0, 1, 0},
        {0, 1, 1, 0, 1, 0, 1},
        {1, 1, 0, 0, 0, 1, 0}
    };

    std::vector<std::vector<int>> graf2 = {
        {0, 1, 0, 1, 0, 1, 1},
        {1, 0, 1, 1, 0, 0, 0},
        {0, 1, 0, 1, 1, 1, 0},
        {1, 1, 1, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 1, 0},
        {1, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 1, 0}
    };

    int length = 3;
    //std::cout << "Введите длину маршрута: ";
    //std::cin >> length;

    std::cout << "\nВ графе G1:\n";
    graphGetRoutesAmountMatrix(graf1, MAX_VERTICES, length);

    std::cout << "В графе G2:\n";
    graphGetRoutesAmountMatrix(graf2, MAX_VERTICES, length);
}

void task6G1() {
    int graf1[MAX_VERTICES][MAX_VERTICES] = {
        {0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 1, 1},
        {0, 0, 0, 1, 1, 1, 0},
        {0, 0, 1, 0, 1, 0, 0},
        {0, 0, 1, 1, 0, 1, 0},
        {0, 1, 1, 0, 1, 0, 1},
        {1, 1, 0, 0, 0, 1, 0}
    };
    std::cout << "\nДля графа G1:" << std::endl;
    int vertexStart, vertexEnd, length;
    std::cout << "Введите начальную вершину: ";
    std::cin >> vertexStart;
    std::cout << "Введите конечную вершину: ";
    std::cin >> vertexEnd;
    std::cout << "Введите длину маршрута: ";
    std::cin >> length;

    int routes1[MAX_VERTICES][MAX_VERTICES] = { 0 };
    int currRoute1[MAX_VERTICES] = { 0 };
    int currRouteSize1 = 0;
    int routesCount1 = 0;

    currRoute1[0] = vertexStart;
    currRouteSize1 = 1;

    std::cout << "\nМаршруты длиной " << length << " из вершины " << vertexStart
        << " в вершину " << vertexEnd << ":\n";

    graphGetRoutesBetweenVertices(length, vertexEnd, currRoute1, &currRouteSize1,
        routes1, &routesCount1, graf1);

    for (int i = 0; i < routesCount1; ++i) {
        for (int j = 0; j <= length; ++j)
            std::cout << routes1[i][j] << " ";

        std::cout << "\n";
    }

    if (routesCount1 == 0) std::cout << "Маршруты не найдены.\n";

    std::cout << std::endl;
}

void task6G2() {

    int graf2[MAX_VERTICES][MAX_VERTICES] = {
        {0, 1, 0, 1, 0, 1, 1},
        {1, 0, 1, 1, 0, 0, 0},
        {0, 1, 0, 1, 1, 1, 0},
        {1, 1, 1, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 1, 0},
        {1, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 1, 0}
    };
    std::cout << "\nДля графа G2:" << std::endl;
    int vertexStart, vertexEnd, length;
    std::cout << "Введите начальную вершину: ";
    std::cin >> vertexStart;
    std::cout << "Введите конечную вершину: ";
    std::cin >> vertexEnd;
    std::cout << "Введите длину маршрута: ";
    std::cin >> length;

    int routes1[MAX_VERTICES][MAX_VERTICES] = { 0 };
    int currRoute1[MAX_VERTICES] = { 0 };
    int currRouteSize1 = 0;
    int routesCount1 = 0;

    currRoute1[0] = vertexStart;
    currRouteSize1 = 1;

    std::cout << "\nМаршруты длиной " << length << " из вершины " << vertexStart
        << " в вершину " << vertexEnd << ":\n";

    graphGetRoutesBetweenVertices(length, vertexEnd, currRoute1, &currRouteSize1,
        routes1, &routesCount1, graf2);

    for (int i = 0; i < routesCount1; ++i) {
        for (int j = 0; j <= length; ++j)
            std::cout << routes1[i][j] << " ";

        std::cout << "\n";
    }

    if (routesCount1 == 0) std::cout << "Маршруты не найдены.\n";

    std::cout << std::endl;
}

void task6() {
    task6G1();
    task6G2();
}

void task7() {
    int grafG1[MAX_VERTICES][MAX_VERTICES] = {
       {0, 1, 0, 0, 0, 0, 1},
       {1, 0, 0, 0, 0, 1, 1},
       {0, 0, 0, 1, 1, 1, 0},
       {0, 0, 1, 0, 1, 0, 0},
       {0, 0, 1, 1, 0, 1, 0},
       {0, 1, 1, 0, 1, 0, 1},
       {1, 1, 0, 0, 0, 1, 0}
    };

    int grafG2[MAX_VERTICES][MAX_VERTICES] = {
        {0, 1, 0, 1, 0, 1, 1},
        {1, 0, 1, 1, 0, 0, 0},
        {0, 1, 0, 1, 1, 1, 0},
        {1, 1, 1, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 1, 0},
        {1, 0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 1, 0}
    };

    //int start;
    //printf("Введите начальную вершину: ");
    //std::cin >> start;

    //int route[MAX_VERTICES];
    //int minSize = 0;

    //printf("Для графа G1:\n");
    //printAllSimplyChainMaximum(grafG1, start, MAX_VERTICES, 0, route, &minSize);

    //printf("\nДля графа G2:\n");
    //minSize = 0; 
    //printAllSimplyChainMaximum(grafG2, start, MAX_VERTICES, 0, route, &minSize);

    //std::cout << "===================================" << std::endl;

    int startVertex;
    std::cout << "Введите начальную вершину: ";
    std::cin >> startVertex;

    std::cout << "Для графа G1:\n";
    findAllMaxSimpleChains(grafG1, startVertex);

    std::cout << "\nДля графа G2:\n";
    findAllMaxSimpleChains(grafG2, startVertex);
}

void task5TimeTest() {
    auto start_time_no_opt = std::chrono::steady_clock::now();

    task5();

    auto end_time_no_opt = std::chrono::steady_clock::now();
    auto elapsed_ns_no_opt = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time_no_opt - start_time_no_opt);


    auto start_time = std::chrono::steady_clock::now();

    task5Alternative();

    auto end_time = std::chrono::steady_clock::now();
    auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);


    std::cout << "\n Время нового способо решения задачи: " << elapsed_ns.count() << " ns\n";

    std::cout << "\n Время старого способо решения задачи: " << elapsed_ns_no_opt.count() << " ns\n";
}

int main() {
    setlocale(LC_ALL, "Russian");
    
    task7();

    return 0;
}
