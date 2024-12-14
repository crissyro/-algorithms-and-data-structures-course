#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstdio>


class Graph {
private:
    std::vector<std::vector<bool>> adjMatrix; // Матрица смежности
    int verticesAmount; // Количество вершин
    int edgesAmount;    // Количество рёбер

public:
    Graph(int n, int m) : verticesAmount(n), edgesAmount(m) {
        adjMatrix.resize(n, std::vector<bool>(n, false));
    }

    void clear() {
        for (int i = 0; i < verticesAmount; ++i) {
            for (int j = 0; j < verticesAmount; ++j) {
                adjMatrix[i][j] = false;
            }
        }
    }

    void generateRandom() {
        clear();
        srand(static_cast<unsigned int>(time(nullptr)));

        // Генерация случайных рёбер
        for (int j = 0; j < edgesAmount; ++j) {
            int i = rand() % verticesAmount;
            int k = rand() % verticesAmount;
            while (i == k || adjMatrix[i][k]) {
                k = rand() % verticesAmount;
            }
            adjMatrix[i][k] = adjMatrix[k][i] = true;  // Неориентированный граф
        }
    }

    bool isEulerian() const {
        for (int i = 0; i < verticesAmount; ++i) {
            int degree = 0;
            for (int j = 0; j < verticesAmount; ++j) {
                if (adjMatrix[i][j]) degree++;
            }
            if (degree % 2 != 0) return false;
        }
        return true;
    }

    bool isHamiltonian() {
        std::vector<bool> visited(verticesAmount, false);
        return dfs(0, 0, visited);
    }

    bool dfs(int v, int count, std::vector<bool>& visited) {
        visited[v] = true;
        count++;
        if (count == verticesAmount) return true;

        for (int i = 0; i < verticesAmount; ++i) {
            if (adjMatrix[v][i] && !visited[i]) {
                if (dfs(i, count, visited)) return true;
            }
        }
        visited[v] = false;
        return false;
    }
};

// Генерация графов за 10 секунд для разного количества вершин и рёбер
void tenSecGen(int n, int last) {
    double time_spent = 0;
    int count_eil = 0, count_gam = 0, count_un = 0;
    int m = n;  

    Graph* Graph_ = new Graph(n, m);

    printf("____________________________________________________\n");
    printf("| Кол-во вершин | Количество рёбер | Эйлеровых графов | Гамильтоновых графов | Всех графов |\n");

    while (m <= last) {
        while (time_spent < 10) {
            clock_t begin = clock();
            Graph_->generateRandom();
            clock_t end = clock();
            time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

            if (Graph_->isHamiltonian()) {
                count_gam++;
            }

            if (Graph_->isEulerian()) {
                count_eil++;
            }
            count_un++;
        }

        printf("| %13d | %16d | %18d | %20d | %11d |\n", n, m, count_eil, count_gam, count_un);

        m++;
        delete Graph_;
        Graph_ = new Graph(n, m);
        time_spent = 0;
        count_eil = 0;
        count_gam = 0;
        count_un = 0;
    }

    delete Graph_;
}

int main() {
    setlocale(LC_ALL, "Russian");

    // Таблица для разных количеств вершин
    printf("\n\nТаблица для 8 вершин: \n");
    tenSecGen(8, 20);

    printf("\n\nТаблица для 9 вершин: \n");
    tenSecGen(9, 21);

    printf("\n\nТаблица для 10 вершин: \n");
    tenSecGen(10, 22);

    return 0;
}
