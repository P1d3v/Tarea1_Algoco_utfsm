#include "strassen.h"
#include <algorithm>
using namespace std;
// https://en.wikipedia.org/wiki/Strassen_algorithm

static const int STRASSEN_THRESHOLD = 64;

static Matrix addMatrix(const Matrix& A, const Matrix& B) {
    int n = static_cast<int>(A.size());
    Matrix C(n, vector<int>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            C[i][j] = A[i][j] + B[i][j];
    return C;
}

static Matrix subMatrix(const Matrix& A, const Matrix& B) {
    int n = static_cast<int>(A.size());
    Matrix C(n, vector<int>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            C[i][j] = A[i][j] - B[i][j];
    return C;
}

static Matrix naiveMultiplySquare(const Matrix& A, const Matrix& B) {
    int n = static_cast<int>(A.size());
    Matrix C(n, vector<int>(n, 0));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            int sum = 0;
            for (int k = 0; k < n; ++k) sum += A[i][k] * B[k][j];
            C[i][j] = sum;
        }
    return C;
}

static void splitMatrix(const Matrix& M, Matrix& a11, Matrix& a12,
                         Matrix& a21, Matrix& a22) {
    int newSize = static_cast<int>(M.size()) / 2;
    for (int i = 0; i < newSize; ++i) {
        for (int j = 0; j < newSize; ++j) {
            a11[i][j] = M[i][j];
            a12[i][j] = M[i][j + newSize];
            a21[i][j] = M[i + newSize][j];
            a22[i][j] = M[i + newSize][j + newSize];
        }
    }
}

static Matrix strassenRec(const Matrix& A, const Matrix& B) {
    int n = static_cast<int>(A.size());

    if (n <= STRASSEN_THRESHOLD) {
        return naiveMultiplySquare(A, B);
    }

    int half = n / 2;
    Matrix a11(half, vector<int>(half)), a12(half, vector<int>(half)),
           a21(half, vector<int>(half)), a22(half, vector<int>(half));
    Matrix b11(half, vector<int>(half)), b12(half, vector<int>(half)),
           b21(half, vector<int>(half)), b22(half, vector<int>(half));

    splitMatrix(A, a11, a12, a21, a22);
    splitMatrix(B, b11, b12, b21, b22);

    Matrix m1 = strassenRec(addMatrix(a11, a22), addMatrix(b11, b22));
    Matrix m2 = strassenRec(addMatrix(a21, a22), b11);
    Matrix m3 = strassenRec(a11, subMatrix(b12, b22));
    Matrix m4 = strassenRec(a22, subMatrix(b21, b11));
    Matrix m5 = strassenRec(addMatrix(a11, a12), b22);
    Matrix m6 = strassenRec(subMatrix(a21, a11), addMatrix(b11, b12));
    Matrix m7 = strassenRec(subMatrix(a12, a22), addMatrix(b21, b22));

    Matrix c11 = addMatrix(subMatrix(addMatrix(m1, m4), m5), m7);
    Matrix c12 = addMatrix(m3, m5);
    Matrix c21 = addMatrix(m2, m4);
    Matrix c22 = addMatrix(subMatrix(addMatrix(m1, m3), m2), m6);

    Matrix C(n, vector<int>(n));
    for (int i = 0; i < half; ++i) {
        for (int j = 0; j < half; ++j) {
            C[i][j] = c11[i][j];
            C[i][j + half] = c12[i][j];
            C[i + half][j] = c21[i][j];
            C[i + half][j + half] = c22[i][j];
        }
    }
    return C;
}

Matrix strassenMultiply(const Matrix& A, const Matrix& B) {
    int rowsA = static_cast<int>(A.size());
    int colsA = static_cast<int>(A[0].size());
    int colsB = static_cast<int>(B[0].size());

    int maxDim = std::max({rowsA, colsA, colsB});
    int newSize = 1;
    while (newSize < maxDim) newSize *= 2;

    Matrix paddedA(newSize, vector<int>(newSize, 0));
    Matrix paddedB(newSize, vector<int>(newSize, 0));

    for (int i = 0; i < rowsA; ++i)
        for (int j = 0; j < colsA; ++j)
            paddedA[i][j] = A[i][j];

    for (int i = 0; i < static_cast<int>(B.size()); ++i)
        for (int j = 0; j < colsB; ++j)
            paddedB[i][j] = B[i][j];

    Matrix result = strassenRec(paddedA, paddedB);

    Matrix C(rowsA, vector<int>(colsB));
    for (int i = 0; i < rowsA; ++i)
        for (int j = 0; j < colsB; ++j)
            C[i][j] = result[i][j];

    return C;
}