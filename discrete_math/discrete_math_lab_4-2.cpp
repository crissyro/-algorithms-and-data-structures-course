#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <queue>
#include <iomanip>
#include <ctime>

class Graph {
public:
    Graph(int vertices, int edges);
    void randGenerate();
    bool isConnected() const;
    bool isEulerian() const;
    bool isHamiltonian() const;
    void output() const;

private:
    std::vector<std::vector<bool>> values;
    int verticesAmountR;
    int verticesAmountC;

    bool isAdjacent(int a, int b) const;
    bool findHamiltonianCycle(int i, int n, std::vector<bool>& path, int current_vertex) const;
    void bfs(int start, std::vector<bool>& visited) const;
};

Graph::Graph(int vertices, int edges)
    : verticesAmountR(vertices), verticesAmountC(edges), values(vertices, std::vector<bool>(edges, false)) {}

void Graph::randGenerate() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, verticesAmountR - 1);

    for (int j = 0; j < verticesAmountC; ++j) {
        int u, v;
        do {
            u = dis(gen);
            v = dis(gen);
        } while (u == v);

        values[u][j] = true;
        values[v][j] = true;
    }
}

bool Graph::isConnected() const {
    if (verticesAmountR == 0) return true;
    
    std::vector<bool> visited(verticesAmountR, false);
    bfs(0, visited);

    for (bool v : visited) 
        if (!v) return false;
    
    return true;
}

void Graph::bfs(int start, std::vector<bool>& visited) const {
    std::queue<int> q;
    q.push(start);
    visited[start] = true;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int j = 0; j < verticesAmountC; ++j) {
            if (values[u][j]) {
                for (int v = 0; v < verticesAmountR; ++v) {
                    if (v != u && values[v][j] && !visited[v]) {
                        visited[v] = true;
                        q.push(v);
                    }
                }
            }
        }
    }
}

bool Graph::isEulerian() const {
    if (!isConnected()) return false;
    
    for (int i = 0; i < verticesAmountR; ++i) {
        int degree = 0;

        for (int j = 0; j < verticesAmountC; ++j) 
            if (values[i][j]) ++degree;
        
        if (degree % 2 != 0) return false;
        
    }

    return true;
}

bool Graph::isAdjacent(int a, int b) const {
    for (int j = 0; j < verticesAmountC; ++j) 
        if (values[a][j] && values[b][j]) return true;
    
    return false;
}

bool Graph::findHamiltonianCycle(int i, int n, std::vector<bool>& path, int current_vertex) const {
    if (i == n) {
        if (isAdjacent(current_vertex, path[0])) return true;

        else return false; 
    }

    for (int next_vertex = 0; next_vertex < n; ++next_vertex) {
        if (!path[next_vertex] && isAdjacent(current_vertex, next_vertex)) {
            path[next_vertex] = true;

            if (findHamiltonianCycle(i + 1, n, path, next_vertex)) 
                return true;
            
            path[next_vertex] = false;
        }
    }

    return false;
}

bool Graph::isHamiltonian() const {
    if (!isConnected()) return false;
    
    std::vector<bool> path(verticesAmountR, false);
    path[0] = true;

    if (!findHamiltonianCycle(1, verticesAmountR, path, 0)) return false;

    else return true;
    
}

void Graph::output() const {
    for (int i = 0; i < verticesAmountR; ++i) {
        for (int j = 0; j < verticesAmountC; ++j) 
            std::cout << values[i][j] << " ";
        
        std::cout << std::endl;
    }
}

void printHeader() {
    std::cout << "____________________________________________________________________________________________________\n";
    std::cout << "| " << std::setw(13) << "Кол-во вершин"
              << " | " << std::setw(16) << "Количество ребер"
              << " | " << std::setw(22) << "Кол-во Эйлеровых графов"
              << " | " << std::setw(20) << "Гамильтоновых графов"
              << " | " << std::setw(11) << "Всех графов"
              << " |\n";
    std::cout << "____________________________________________________________________________________________________\n";
}

void tenSecGen(int n, int last) {
    for (int m = n; m <= last; ++m) {
        int count_eil = 0, count_gam = 0, count_un = 0;
        auto start = std::chrono::high_resolution_clock::now();
        while (true) {
            Graph graph(n, m);
            graph.randGenerate();
            ++count_un;
            if (graph.isHamiltonian()) {
                ++count_gam;
            }
            if (graph.isEulerian()) {
                ++count_eil;
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            if (elapsed.count() >= 10.0) {
                break;
            }
        }
        std::cout << "| " << std::setw(13) << n
                  << " | " << std::setw(16) << m
                  << " | " << std::setw(22) << count_eil
                  << " | " << std::setw(20) << count_gam
                  << " | " << std::setw(11) << count_un << " |\n";
    }
}

int main() {
    std::cout << "\n\nТаблица для 8 вершин: \n";
    printHeader();
    tenSecGen(8, 28);
    std::cout << "\n\nТаблица для 9 вершин: \n";
    printHeader();
    tenSecGen(9, 36);
    std::cout << "\n\nТаблица для 10 вершин: \n";
    printHeader();
    tenSecGen(10, 45);
    return 0;
}
