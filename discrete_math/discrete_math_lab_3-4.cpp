#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

template <typename T>
class BinMatrix {

public:
    unsigned short size;
    std::vector<std::vector<T>> matrix;

    explicit BinMatrix(unsigned short size) {
        if (size <= 0) throw std::invalid_argument("Недопустимый размер матрицы");

        this->size = size;
        matrix.resize(size);

        for (auto& row : matrix) row.resize(size);
    }

    ~BinMatrix() {};

    template <typename RelationFunction>
    static void MakeRelationMatrix(RelationFunction relation, BinMatrix& result) {
        for (size_t i = 0; i < result.size; ++i) {
            for (size_t j = 0; j < result.size; ++j) {

                if (relation(i, j)) result.matrix[i][j] = 1;

                else result.matrix[i][j] = 0;

            }
        }
    }

    bool isReflection(const BinMatrix& m) {
        for (int i = 0; i < m.size; i++) {
            if (!m.matrix[i][i]) {
                std::cout << "Отношение не является рефлексивным Отсутствует пара (" << ++i << ", " << ++i << ")\n";
                return false;
            }
        }

        std::cout << "Отношение является рефлексивным" << std::endl;

        return true;
    }

    bool isAntiReflection(const BinMatrix& m) {
        for (int i = 0; i < m.size; i++) {
            if (m.matrix[i][i]) {
                std::cout << "Отношение не является антирефлексивным. Присутсвует пара (" << ++i << ", " << ++i << ")\n";
                return false;
            }
        }

        std::cout << "Отношение является антирефлексивным\n";

        return true;
    }

    bool isSymmetric(const BinMatrix& m) {
        for (int i = 0; i < m.size; i++) {
            for (int j = 0; j < m.size; j++) {
                if (m.matrix[i][j] != m.matrix[j][i]) {
                    std::cout << "Отношение не является симметричным. Есть такая пара (" << ++i << ", " << ++j << "), что принадлежит отношению, a (" << ++j << ", " << ++i << ") нет\n";
                    return false;
                }
            }
        }
        std::cout << "Отношение является симметричным\n";
        return true;
    }

    bool isAntiSymmetric(const BinMatrix& m) {
        for (int i = 0; i < m.size; i++) {
            for (int j = 0; j < m.size; j++) {
                if (m.matrix[i][j] == m.matrix[j][i] && (m.matrix[i][j] && m.matrix[j][i])) {
                    std::cout << "Отношение не является антисимметричным. Есть такая пара (" << ++i << ", " << ++j << "), что принадлежит отношению и (" << ++j << ", " << ++i << ") тоже принадлежит \n";
                    return false;
                }
            }
        }
        std::cout << "Отношение является антисимметричным\n";

        return true;
    }

    bool isTransitive(const BinMatrix& m) {
        for (int i = 0; i < m.size; i++) {
            for (int j = 0; j < m.size; j++) {
                if (m.matrix[i][j]) {
                    for (int k = 0; k < m.size; k++) {
                        if (m.matrix[j][k] && !m.matrix[i][k]) {
                            std::cout << "Отношение не является транзитивным, т.к (" << ++i << ", " << ++j << ") принадлежат R и (" \
                                << ++j << ", " << ++k << ") принадлежат R, а (" << ++i << ", " << ++k << ") не принадлежат R, что противоречит усл транзитивности" \
                                << std::endl;

                            return false;
                        }
                    }
                }
            }
        }

        std::cout << "Отношение является транзитивным" << std::endl;

        return true;
    }

    bool isAntiTransitive(const BinMatrix& m) {
        for (int i = 0; i < m.size; i++) {
            for (int j = 0; j < m.size; j++) {
                if (m.matrix[i][j]) {
                    for (int k = 0; k < m.size; k++) {
                        if (m.matrix[i][k] && m.matrix[j][k]) {
                            std::cout << "Отношение не является антитранзитивным, т.к (" << ++i << ", " << ++j << ") принадлежат R и (" \
                                << ++j << ", " << ++k << ") принадлежат R и (" << ++i << ", " << ++k << ") тоже принадлежат R, что противоречит усл антитранзитивности" \
                                << std::endl;
                            return false;
                        }
                    }
                }
            }
        }
        std::cout << "Отношение является антитранзитивным\n";

        return true;
    }

    bool isFull(const BinMatrix& m) {
        for (int i = 0; i < m.size; i++) {
            for (int j = 0; j < m.size; j++) {
                if (i != j) {
                    if (!m.matrix[i][j] && !m.matrix[j][i]) {
                        std::cout << "Отношение не является полным. Отсутствует пара (" << ++i << ", " << ++j << ") и пара (" << ++j << ", " << ++i << ") \n";
                        return false;
                    }
                }

            }
        }

        std::cout << "Отношение является полным\n";

        return true;
    }

    inline bool isOrder(const BinMatrix& m) { 
        if (isAntiSymmetric(m) && isTransitive(m)) {
            std::cout << std::endl;
            std::cout << "Отношение обладает свойством порядка" << std::endl;
            return true;
        } else {
            std::cout << std::endl;
            std::cout << "Отношение не обладает свойством порядка" << std::endl;
            return false;
        }
    }

    inline bool isLooseOrder(const BinMatrix& m) { 
        if (isOrder(m) && isReflection(m)) {
            std::cout << std::endl;
            std::cout << "Отношение обладает свойством нестрогого порядка" << std::endl;
            return true;
        } else {
            std::cout << std::endl;
            std::cout << "Отношение не обладает свойством нестрогого порядка" << std::endl;
            return false;
        }
    
    }

    inline bool isStrictOrder(const BinMatrix& m) { 
        if (isOrder(m) && isAntiReflection(m)) {
            std::cout << std::endl;
            std::cout << "Отношение обладает свойством строгого порядка" << std::endl;
            return true;
        } else {
            std::cout << std::endl;
            std::cout << "Отношение не обладает свойством строгого порядка" << std::endl;
            return false;
        }
    }

    inline bool isLinearOrder(const BinMatrix& m) { 
        if (isOrder(m) && isFull(m)) {
            std::cout << std::endl;
            std::cout << "Отношение обладает свойством линейного порядка" << std::endl;
            return true;
        } else {
            std::cout << std::endl;
            std::cout << "Отношение не обладает свойством линейного порядка" << std::endl;
            return false;
        }
    }

    inline bool isLooseLinearOrder(const BinMatrix& m) { 
        if (isLooseOrder(m) && isFull(m)) {
            std::cout << std::endl;
            std::cout << "Отношение обладает свойством нестрогого линейного порядка" << std::endl;
            return true;
        } else {
            std::cout << std::endl;
            std::cout << "Отношение не обладает свойством нестрогого линейного порядка" << std::endl;
            return false;
        }
    }

    inline bool isStrictLinearOrder(const BinMatrix& m) { 
        if (isStrictOrder(m) && isFull(m)) {
            std::cout << std::endl;
            std::cout << "Отношение обладает свойством строгого линейного порядка" << std::endl;
            return true;
        } else {
            std::cout << std::endl;
            std::cout << "Отношение не обладает свойством строгого линейного порядка" << std::endl;
            return false;
        }    
    }

    void printMatrix() {
        for (const auto& row : matrix) {
            for (int val : row) {
                std::cout << val << " ";
            }
            std::cout << std::endl;
        }
    }

    inline void makeZeroDiagonal() {
        for (size_t i = 0; i < size; i++) matrix[i][i] = 0;
    }

    BinMatrix& operator=(const BinMatrix& other) {
        BinMatrix res(other.size);

        for (size_t i = 0; i < other.size; i++) {
            for (size_t j = 0; j < other.size; j++)
                res.matrix[i][j] = other.matrix[i][j];
        }

        return res;
    }

    BinMatrix makeDominationMatrix(const BinMatrix& other) {
        BinMatrix domMatrix = other;

        domMatrix.makeZeroDiagonal();

        for (size_t i = 0; i < size; i++) {
            for (size_t j = 0; j < size; j++) {
                for (size_t k = 0; k < size; k++) {
                    if (matrix[j][i] && matrix[i][k])
                        domMatrix.matrix[j][k] = 0;
                }
            }
        }

        return domMatrix;
    }

};

typedef struct Point {
public:
    int x;
    int y;
} Point;

template<typename T>
static void MakeRelationPointsMatrix(BinMatrix<T>& result, Point* M) {
    for (size_t a = 0; a < result.size; ++a) {
        for (size_t b = 0; b < result.size; ++b) {

            if (M[a].x * M[a].x + M[a].y * M[a].y < M[b].x * M[b].x + M[b].y * M[b].y)
                result.matrix[a][b] = 1;

            else result.matrix[a][b] = 0;

        }
    }
}

void task1() {
    try {
        Point M1[] = { {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 0}, {0, 1}, {1, -1}, {1, 0}, {1, 1} };

        BinMatrix<bool> mat1(9);

        MakeRelationPointsMatrix(mat1, M1);

        std::cout << "M1 матрица:" << std::endl;
        mat1.printMatrix();

        std::cout << std::endl;

        std::cout << "Отношение M1:\n";
        std::cout << "Рефлексивность: "; mat1.isReflection(mat1);
        std::cout << "Антирефлексивность: "; mat1.isAntiReflection(mat1);
        std::cout << "Симметричность: "; mat1.isSymmetric(mat1);
        std::cout << "Антитранзитивность: "; mat1.isAntiTransitive(mat1);
        std::cout << "Полнота: "; mat1.isFull(mat1);
        std::cout << "Порядок: "; mat1.isOrder(mat1);

        std::cout << std::endl;

        Point M2[] = { {-2, 0}, {-1, -1}, {-1, 0}, {-1, 1}, {0, -2}, \
        {0, -1}, {0, 0}, {0, 1}, {0, 2}, {1, -1}, {1, 0}, \
        {1, 1}, {2, 0} };

        BinMatrix<bool> mat2(13);
        MakeRelationPointsMatrix(mat2, M2);

        std::cout << "M2 матрица:" << std::endl;
        mat2.printMatrix();

        std::cout << std::endl;

        std::cout << "Отношение M2:\n";
        std::cout << "Рефлексивность: "; mat2.isReflection(mat2);
        std::cout << "Антирефлексивность: "; mat2.isAntiReflection(mat2);
        std::cout << "Симметричность: "; mat2.isSymmetric(mat2);
        std::cout << "Антитранзитивность: "; mat2.isAntiTransitive(mat2);
        std::cout << "Полнота: "; mat2.isFull(mat2);
        std::cout << "Порядок: " << std::endl; mat2.isOrder(mat2);

        std::cout << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}

void task2() {
    try {
        Point M1[] = { {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 0}, {0, 1}, {1, -1}, {1, 0}, {1, 1} };

        BinMatrix<bool> mat1(9);

        MakeRelationPointsMatrix(mat1, M1);

        std::cout << "Матрица отношения доминирования на множестве М1:" << std::endl;
        BinMatrix<bool> domM1 = mat1.makeDominationMatrix(mat1);
        domM1.printMatrix();

        std::cout << std::endl;

        Point M2[] = { {-2, 0}, {-1, -1}, {-1, 0}, {-1, 1}, {0, -2}, \
        {0, -1}, {0, 0}, {0, 1}, {0, 2}, {1, -1}, {1, 0}, \
        {1, 1}, {2, 0} };

        BinMatrix<bool> mat2(13);
        MakeRelationPointsMatrix(mat2, M2);

        std::cout << "Матрица отношения доминирования на множестве М2:" << std::endl;
        BinMatrix<bool> domM2 = mat2.makeDominationMatrix(mat2);
        domM2.printMatrix();

        std::cout << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}

inline bool isNegative(const std::vector<int> W) {
    for (size_t i = 0; i < W.size(); i++)
        if (W[i] >= 0) return false;

    return true;
}

template<typename T>
static void topologicSort(BinMatrix<T>& m) {
    std::vector<int> W;

    for (size_t i = 0; i < m.size; i++) {
        W.push_back(0);
        for (size_t j = 0; j < m.size; j++)
            W[i] += m.matrix[j][i];
    }

    int k = 0;
    while (!isNegative(W)) {
        std::cout << k << " уровень -> ";

        for (int i = 0; i < m.size; i++) {
            if (W[i] == 0) {
                printf("%i ", i + 1);
                W[i] = -(k + 1);
            }
        }
        for (int i = 0; i < m.size; i++) {
            if (W[i] == -(k + 1)) {
                for (int j = 0; j < m.size; j++) {
                    W[j] -= m.matrix[i][j];
                }
            }
        }
        k++;
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Конечные элементы массива\n";

    for (size_t a = 0; a < m.size; a++)
        std::cout << W[a] << " ";
}   

void task3() {
    try {
        Point M1[] = { {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 0}, {0, 1}, {1, -1}, {1, 0}, {1, 1} };

        BinMatrix<bool> mat1(9);

        MakeRelationPointsMatrix(mat1, M1);

        std::cout << "Топологическая сортировка по матрице на множестве М1 отношения доминирования:" << std::endl;
        BinMatrix<bool> domM1 = mat1.makeDominationMatrix(mat1);

        topologicSort(domM1);

        std::cout << std::endl;
        std::cout << std::endl;

        Point M2[] = { {-2, 0}, {-1, -1}, {-1, 0}, {-1, 1}, {0, -2}, \
        {0, -1}, {0, 0}, {0, 1}, {0, 2}, {1, -1}, {1, 0}, \
        {1, 1}, {2, 0} };

        BinMatrix<bool> mat2(13);
        MakeRelationPointsMatrix(mat2, M2);

        std::cout << "Топологическая сортировка по матрице на множестве М2 отношения доминирования:" << std::endl;
        BinMatrix<bool> domM2 = mat2.makeDominationMatrix(mat2);

        topologicSort(domM2);

        std::cout << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
   //task1();
   //task2();
   //task3();

    return 0;
}