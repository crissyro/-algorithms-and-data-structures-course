#include <cstdio>
#include <vector>
#include <algorithm> // For std::min
#include <iostream>  // For std::cout
#include <functional>

using namespace std;

struct Graph {
    vector<vector<int>> values;
    int nVertices;
};

Graph getMemGraph(int n) {
    Graph graph;
    graph.nVertices = n;
    graph.values.resize(n, vector<int>(n, 0));
    return graph;
}

Graph kraskalAlgorithm(Graph graph) {
    vector<int> flower(graph.nVertices);
    for (int j = 0; j < graph.nVertices; j++) {
        flower[j] = j;
    }
    Graph forest = getMemGraph(graph.nVertices);
    for (int i = 0; i < graph.nVertices; i++) {
        for (int j = i; j < graph.nVertices; j++) {
            if (graph.values[i][j]) {
                if (flower[i] != flower[j]) {
                    forest.values[i][j] = 1;
                    forest.values[j][i] = 1;
                    for (int k = 0; k < graph.nVertices; k++) {
                        if (flower[k] == flower[j]) 
                            flower[k] = flower[i];
                    }
                }
            }
        }
    }

    cout << "\nБукеты: ";

    for (int i = 0; i < flower.size(); i++) 
        cout << (flower[i] + 1) << " ";
    
    cout << endl;
    cout << "\nЛес:\n";

    for (int i = 0; i < forest.nVertices; i++) {
        for (int j = 0; j < forest.nVertices; j++) 
            cout << forest.values[i][j] << " ";
        

        cout << endl;
    }

    cout << endl;

    return forest;
}

void dfs(const Graph& graph, vector<int>& visited, int vertex) {
    if (visited[vertex]) return;
    
    visited[vertex] = 1;
    for (int i = 0; i < graph.nVertices; i++) {
        if (graph.values[vertex][i]) 
            dfs(graph, visited, i);
    }
}

Graph copyGraph(const Graph& graph) {
    Graph graph_;

    graph_.nVertices = graph.nVertices;
    graph_.values = graph.values;

    return graph_;
}

void recursionPartForFindingBridges(Graph& graph, Graph& bridges,
    vector<int>& visited, vector<int>& timerIn, vector<int>& timerUp,
    int vertex, int parentVertex) {
    static int timer = 0;
    visited[vertex] = 1;
    timerIn[vertex] = timerUp[vertex] = timer++;
    for (int to = 0; to < graph.nVertices; to++) {
        if (graph.values[vertex][to]) {
            if (to == parentVertex) {
                continue;
            }
            if (visited[to]) {
                timerUp[vertex] = min(timerUp[vertex], timerIn[to]);
            } else {
                recursionPartForFindingBridges(graph, bridges, visited, timerIn, timerUp, to, vertex);
                timerUp[vertex] = min(timerUp[vertex], timerIn[to]);
                if (timerUp[to] > timerIn[vertex]) {
                    bridges.values[vertex][to] = 1;
                    bridges.values[to][vertex] = 1;
                    cout << "Мост: (" << vertex + 1 << ", " << to + 1 << ")\n";
                }
            }
        }
    }
}

Graph tarjanAlgorithmForFindingBridges(Graph graph) {
    vector<int> timerIn(graph.nVertices, 0);
    vector<int> timerUp(graph.nVertices, 0);
    vector<int> visited(graph.nVertices, 0);
    Graph bridges = getMemGraph(graph.nVertices);
    for (int i = 0; i < graph.nVertices; i++) {
        if (!visited[i]) 
            recursionPartForFindingBridges(graph, bridges, visited, timerIn, timerUp, i, -1);
    }

    return bridges;
}

bool isConnect(const Graph& graph) {
    std::vector<bool> visited(graph.nVertices, false);
    std::function<void(int)> dfs = [&](int vertex) {
        visited[vertex] = true;
        for (int neighbor = 0; neighbor < graph.nVertices; ++neighbor) {
            if (graph.values[vertex][neighbor] && !visited[neighbor]) {
                dfs(neighbor);
            }
        }
    };
    dfs(0);
    for (bool visitedVertex : visited) {
        if (!visitedVertex) {
            return false;
        }
    }
    return true;
}

Graph splitGraphTwoConnectedComponents(Graph graph, Graph deletedEdges) {
    Graph forestWithTwoComponents = copyGraph(graph);
    for (int i = 0; i < graph.nVertices; i++) {
        for (int j = 0; j < graph.nVertices; j++) {
            if (graph.values[i][j] && deletedEdges.values[i][j]) {
                forestWithTwoComponents.values[i][j] = 0;
                forestWithTwoComponents.values[j][i] = 0;

                if (!isConnect(forestWithTwoComponents)) return forestWithTwoComponents;
            }
        }
    }

    return forestWithTwoComponents;
}

void showGraphDifference(const Graph& graph1, const Graph& graph2) {
    for (int i = 0; i < graph1.nVertices; i++) {
        for (int j = i; j < graph1.nVertices; j++) {
            if (graph1.values[i][j] != graph2.values[i][j]) 
                cout << "Пара (" << i + 1 << ", " << j + 1 << ")\n";
        }
    }
}

int main() {
    int n;

    cout << "Введите кол-во вершин графа: ";

    scanf("%d", &n);

    Graph graph = getMemGraph(n);

    cout << "Введите граф:\n";

    for (int i = 0; i < graph.nVertices; i++) 
        for (int j = 0; j < graph.nVertices; j++) scanf("%d", &graph.values[i][j]);
    

    if (isConnect(graph)) {

        Graph forest = kraskalAlgorithm(graph);
        Graph bridges = tarjanAlgorithmForFindingBridges(graph);
        Graph forestWithTwoComponents = splitGraphTwoConnectedComponents(forest, bridges);

        cout << "\nМаксимальный набор для разделения на два связанных компонента:\n";

        showGraphDifference(graph, forestWithTwoComponents);
    } else cout << "\nГраф несвязный\n";
    
    return 0;
}