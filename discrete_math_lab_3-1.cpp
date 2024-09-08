#include <iostream>
#include <vector>
#include <cmath>

constexpr int N = 10;
using matrix_t = std::vector<std::vector<int>>;

// Функция для инициализации матрицы нулями
matrix_t createMatrix(int rows, int cols) {
    return matrix_t(rows, std::vector<int>(cols, 0));
}

// Функция для проверки на подмножество
bool isSubset(const matrix_t& matrix1, const matrix_t& matrix2) {
    for (int i = 0; i < matrix1.size(); ++i) {
        for (int j = 0; j < matrix1[i].size(); ++j) {
            if (matrix1[i][j] == 1 && matrix2[i][j] == 0) {
                return false;
            }
        }
    }
    return true;
}

// Функция для проверки на равенство
bool isEqual(const matrix_t& matrix1, const matrix_t& matrix2) {
    for (int i = 0; i < matrix1.size(); ++i) {
        for (int j = 0; j < matrix1[i].size(); ++j) {
            if (matrix1[i][j] != matrix2[i][j]) {
                return false;
            }
        }
    }
    return true;
}

// Функция для проверки на строгое подмножество
bool isProperSubset(const matrix_t& matrix1, const matrix_t& matrix2) {
    bool subset = true;
    bool equal = true;
    for (int i = 0; i < matrix1.size(); ++i) {
        for (int j = 0; j < matrix1[i].size(); ++j) {
            if (matrix1[i][j] == 1 && matrix2[i][j] == 0) {
                subset = false;
            }
            if (matrix1[i][j] != matrix2[i][j]) {
                equal = false;
            }
        }
    }
    return subset && !equal;
}

// Функция для операции объединения
matrix_t unionMatrices(const matrix_t& matrix1, const matrix_t& matrix2) {
    matrix_t result = createMatrix(N, N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            result[i][j] = matrix1[i][j] | matrix2[i][j];
        }
    }
    return result;
}

// Функция для операции пересечения
matrix_t intersectionMatrices(const matrix_t& matrix1, const matrix_t& matrix2) {
    matrix_t result = createMatrix(N, N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            result[i][j] = matrix1[i][j] & matrix2[i][j];
        }
    }
    return result;
}

// Функция для операции разности двух матриц-отношений
matrix_t differenceMatrices(const matrix_t& matrix1, const matrix_t& matrix2) {
    matrix_t result = createMatrix(N, N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            result[i][j] = matrix1[i][j] & ~matrix2[i][j];
        }
    }
    return result;
}

// Функция для операции симметрической разности двух матриц-отношений
matrix_t symDifferenceMatrices(const matrix_t& matrix1, const matrix_t& matrix2) {
    matrix_t result = createMatrix(N, N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            result[i][j] = (matrix1[i][j] || matrix2[i][j]) && !(matrix1[i][j] && matrix2[i][j]);
        }
    }
    return result;
}

// Функция для операции дополнения
matrix_t complementMatrix(const matrix_t& matrix) {
    matrix_t result = createMatrix(N, N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            result[i][j] = ~matrix[i][j];
        }
    }
    return result;
}

// Функция для операции обращения
matrix_t inverseMatrix(const matrix_t& matrix) {
    matrix_t result = createMatrix(N, N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            result[j][i] = matrix[i][j];
        }
    }
    return result;
}

// Функция для операции композиции двух матриц-отношений
matrix_t compositeMatrices(const matrix_t& matrix1, const matrix_t& matrix2) {
    matrix_t result = createMatrix(N, N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                if (matrix1[i][k] && matrix2[k][j]) {
                    result[i][j] = 1;
                    break;
                }
            }
        }
    }
    return result;
}

// Функция для создания матрицы отношений A, B и C
matrix_t makeRelationMatrix(bool (*relation)(int, int)) {
    matrix_t matrix = createMatrix(N, N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (relation(i, j)) {
                matrix[i][j] = 1;
            }
        }
    }
    return matrix;
}

bool A_relation(int x, int y) {
    return y > x + 5 || x > y + 5;
}

bool B_relation(int x, int y) {
    return x % 2 == 0 && y % 2 == 0;
}

bool C_relation(int x, int y) {
    return std::abs(x - y) > 5;
}

void printMatrix(const matrix_t& matrix) {
    for (const auto& row : matrix) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
}

bool IsSubset(const matrix_t& m1, const matrix_t& m2) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (m1[i][j] == 1 && m2[i][j] != 1) {
                return false;
            }
        }
    }
    return true;
}

bool IsEqual(const matrix_t& m1, const matrix_t& m2) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (m1[i][j] != m2[i][j]) {
                return false;
            }
        }
    }
    return true;
}

bool IsProperSubset(const matrix_t& m1, const matrix_t& m2) {
    return !IsEqual(m1, m2) && IsSubset(m1, m2);
}

void UnionMatrix(const matrix_t& m1, const matrix_t& m2, matrix_t& res) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            res[i][j] = (m1[i][j] || m2[i][j]) ? 1 : 0;
        }
    }
}

void IntersectionMatrix(const matrix_t& m1, const matrix_t& m2, matrix_t& res) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            res[i][j] = (m1[i][j] && m2[i][j]) ? 1 : 0;
        }
    }
}

void DiffMatrix(const matrix_t& m1, const matrix_t& m2, matrix_t& res) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            res[i][j] = (m2[i][j] ? 0 : m1[i][j]);
        }
    }
}

void SymDiffMatrix(const matrix_t& m1, const matrix_t& m2, matrix_t& res) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            res[i][j] = (m1[i][j] + m2[i][j] == 1) ? 1 : 0;
        }
    }
}

void ComplMatrix(const matrix_t& m, matrix_t& res) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            res[i][j] = !m[i][j];
        }
    }
}

void InverseMatrix(const matrix_t& m, matrix_t& res) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            res[i][j] = m[j][i];
        }
    }
}

void CompositeMatrix(const matrix_t& m1, const matrix_t& m2, matrix_t& res) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            res[i][j] = 0;
            for (int k = 0; k < N; k++) {
                if (m1[i][k] && m2[k][j]) {
                    res[i][j] = 1;
                }
            }
        }
    }
}

bool isReflection(const matrix_t& m) {
    for (int i = 0; i < N; i++) {
        if (!m[i][i]) {
            std::cout << "Отношение не является рефлексивным. Отсутствует пара (" << i << ", " << i << ")\n";
            return false;
        }
    }
    std::cout << "Отношение является рефлексивным\n";
    return true;
}

bool isAntiReflection(const matrix_t& m) {
    for (int i = 0; i < N; i++) {
        if (m[i][i]) {
            std::cout << "Отношение не является антирефлексивным. Присутсвует пара (" << i << ", " << i << ")\n";
            return false;
        }
    }
    std::cout << "Отношение является антирефлексивным\n";
    return true;
}

bool isSymmetric(const matrix_t& m) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (m[i][j] != m[j][i]) {
                std::cout << "Отношение не является симметричным. Есть такая пара (" << i << ", " << j << "), что R[" << i << "][" << j << "] != R[" << j << "][" << i << "]\n";
                return false;
            }
        }
    }
    std::cout << "Отношение является симметричным\n";
    return true;
}

bool isAntiSymmetric(const matrix_t& m) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (m[i][j] == m[j][i] && (m[i][j] && m[j][i])) {
                std::cout << "Отношение не является антисимметричным. Есть такая пара (" << i << ", " << j << "), что R[" << i << "][" << j << "] = R[" << j << "][" << i << "]\n";
                return false;
            }
        }
    }
    std::cout << "Отношение является антисимметричным\n";
    return true;
}

bool isTransitive(const matrix_t& m) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            //bool isTransitive = false;
            for (int k = 0; k < N; k++) {
                if (m[i][k] && m[k][j]) {
                    if (!m[i][j]) {
                        std::cout << "Отношение не является транзитивным. Для данного набора (" << i << ", " << j << ", " << k << ") не выполняется правило xRz && zRy && xRy\n";
                        return false;
                    }
                    //isTransitive = true;
                }
            }
           /* if (!isTransitive) {
                std::cout << "Отношение не является транзитивным. Не выполняется правило xRz && zRy && xRy\n";
                return false;
            }*/
        }
    }
    std::cout << "Отношение является транзитивным\n";
    return true;
}

bool isAntiTransitive(const matrix_t& m) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                if (m[i][k] && m[k][j] && m[i][j]) {
                    std::cout << "Отношение не является антитранзитивным. Для данного набора (" << i << ", " << j << ", " << k << ") выполняется правило xRz && zRy && xRy\n";
                    return false;
                }
            }
        }
    }
    std::cout << "Отношение является антитранзитивным\n";
    return true;
}

bool isFull(const matrix_t& m) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (!(m[i][j] || m[j][i])) {
                std::cout << "Отношение не является полным. Отсутствует пара (" << i << ", " << j << ")\n";
                return false;
            }
        }
    }
    std::cout << "Отношение является полным\n";
    return true;
}

//int main() {
//    matrix_t matrix_A = makeRelationMatrix(A_relation);
//    matrix_t matrix_B = makeRelationMatrix(B_relation);
//    matrix_t matrix_C = makeRelationMatrix(C_relation);
//
//    // Шаг 1: Найти инверсию матрицы B (B^{-1})
//    matrix_t matrixB_inv = inverseMatrix(matrix_B);
//
//    // Шаг 2: Композиция A o B^{-1}
//    matrix_t A_o_B_inv = compositeMatrices(matrix_A, matrixB_inv);
//
//    // Шаг 3: Пересечение A o B^{-1} с A
//    matrix_t intersection_A_o_B_inv_and_A = intersectionMatrices(A_o_B_inv, matrix_A);
//
//    // Шаг 4: Вычитание C из пересечения
//    matrix_t D_matrix = differenceMatrices(intersection_A_o_B_inv_and_A, matrix_C);
//
//    std::cout << "Matrix D = (A o B^{-1}) ∩ A - C:" << std::endl;
//    printMatrix(D_matrix);
//
//    return 0;
//}

int main() {
    setlocale(LC_ALL, "Russian");


    auto matrix_A = makeRelationMatrix(A_relation);
    auto matrix_B = makeRelationMatrix(B_relation);
    auto matrix_C = makeRelationMatrix(C_relation);

    std::cout << "Матрица A:\n";
    std::cout << "Рефлексивность: "; isReflection(matrix_A);
    std::cout << "Антирефлексивность: "; isAntiReflection(matrix_A);
    std::cout << "Симметричность: "; isSymmetric(matrix_A);
    std::cout << "Антисимметричность: "; isAntiSymmetric(matrix_A);
    std::cout << "Транзитивность: "; isTransitive(matrix_A);
    std::cout << "Антитранзитивность: "; isAntiTransitive(matrix_A);
    std::cout << "Полнота: "; isFull(matrix_A);

    std::cout << "\nМатрица B:\n";
    std::cout << "Рефлексивность: "; isReflection(matrix_B);
    std::cout << "Антирефлексивность: "; isAntiReflection(matrix_B);
    std::cout << "Симметричность: "; isSymmetric(matrix_B);
    std::cout << "Антисимметричность: "; isAntiSymmetric(matrix_B);
    std::cout << "Транзитивность: "; isTransitive(matrix_B);
    std::cout << "Антитранзитивность: "; isAntiTransitive(matrix_B);
    std::cout << "Полнота: "; isFull(matrix_B);

    std::cout << "\nМатрица C:\n";
    std::cout << "Рефлексивность: "; isReflection(matrix_C);
    std::cout << "Антирефлексивность: "; isAntiReflection(matrix_C);
    std::cout << "Симметричность: "; isSymmetric(matrix_C);
    std::cout << "Антисимметричность: "; isAntiSymmetric(matrix_C);
    std::cout << "Транзитивность: "; isTransitive(matrix_C);
    std::cout << "Антитранзитивность: "; isAntiTransitive(matrix_C);
    std::cout << "Полнота: "; isFull(matrix_C);

    return 0;
}
