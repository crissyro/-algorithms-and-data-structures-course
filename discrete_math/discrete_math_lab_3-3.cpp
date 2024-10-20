#include <iostream>
#include <vector>
#include <algorithm>

template <typename T>
class BinMatrix {
private:
    unsigned short size;
    std::vector<std::vector<T>> matrix;

public:
    explicit BinMatrix(unsigned short size) {
        if (size <= 0) throw std::invalid_argument("Недопустимый размер матрицы");

        this->size = size;
        matrix.resize(size);

        for (auto& row : matrix) row.resize(size);
    }

    ~BinMatrix() {};

    short getSize(BinMatrix& M) {
        return M.size;
    }

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

    bool isEquivalent(const BinMatrix& m) {
        if (isReflection(m) && isSymmetric(m) && isTransitive(m)) {
            std::cout << "Отношение является эквивалентным - обладает свойствами рефлексивности, симметричности и транзитивности" << std::endl;
            return true;
        }
        else {
            std::cout << "Отношение не является эквивалентным" << std::endl;
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

    void formationOfPartition(const BinMatrix& m, const int* array, int* resArray) {
        int k = 0;

        for (size_t i = 0; i < 10; i++)
            resArray[i] = 0;

        for (size_t i = 0; i < 10; i++) {
            if (resArray[i] == 0) {
                k++;
                for (size_t j = 0; j < 10; j++) {
                    if (m.matrix[i][j])
                        resArray[j] = k;
                }
            }
        }
    }

};

void factorSet(const int* S, const size_t size) {
    int count_classes = *std::max_element(S, S + size);

    std::cout << "{";
    for (int i = 1; i <= count_classes; ++i) {
        if (i != 1) std::cout << ", ";

        std::cout << "{";

        bool isFirst = true;
        for (int j = 0; j < size; ++j) {
            if (S[j] == i) {
                if (!isFirst) std::cout << ",";

                else isFirst = false;
                
                std::cout << j + 1;
            }
        }
        
        std::cout << "}";
    }

    std::cout << "}" << std::endl;
}

const int baseArray[10] = { 1, 2, 3, 4, 5, 6 , 7, 8, 9, 10 };

bool myRelation(int x, int y) {
    return (x < 3 && y < 3) || (x > 3 && y > 3) || (x == y);
}

void task1() {
    try {
        BinMatrix<bool> mat(10);
        BinMatrix<bool>::MakeRelationMatrix(myRelation, mat);

        std::cout << "Входная матрица:" << std::endl;
        mat.printMatrix();

        std::cout << std::endl;


        std::cout << "Результирующая матрица:" << std::endl;

        std::cout << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}

void task2() {
    try {
        BinMatrix<bool> mat(10);
        BinMatrix<bool>::MakeRelationMatrix(myRelation, mat);

        std::cout << "Входная матрица:" << std::endl;
        mat.printMatrix();

        std::cout << std::endl;

        std::cout << "Отношение A:\n";
        std::cout << "Рефлексивность: "; mat.isReflection(mat);
        std::cout << "Антирефлексивность: "; mat.isAntiReflection(mat);
        std::cout << "Симметричность: "; mat.isSymmetric(mat);
        std::cout << "Антисимметричность: "; mat.isAntiSymmetric(mat);
        std::cout << "Транзитивность: "; mat.isTransitive(mat);
        std::cout << "Антитранзитивность: "; mat.isAntiTransitive(mat);
        std::cout << "Полнота: "; mat.isFull(mat);
        std::cout << "Эквивалентность: "; mat.isEquivalent(mat);

        std::cout << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}

void task4() {
    try {
        BinMatrix<bool> mat(10);
        BinMatrix<bool>::MakeRelationMatrix(myRelation, mat);

        std::cout << "Входная матрица:" << std::endl;
        mat.printMatrix();

        std::cout << std::endl;

        std::cout << "Эквивалентность: "; mat.isEquivalent(mat);

        std::cout << std::endl;

        int res[10];

        mat.formationOfPartition(mat, baseArray, res);

        std::cout << "Храние разбиения в одномерном массиве" << std::endl;

        for (size_t i = 0; i < 10; i++)
            std::cout << res[i] << " ";

        std::cout << std::endl;

        std::cout << "Фактормножество" << std::endl;

        factorSet(res, 10);

    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    task4();

    return 0;
}
