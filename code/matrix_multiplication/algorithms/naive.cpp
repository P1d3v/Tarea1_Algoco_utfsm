#include "naive.h"
using namespace std;
//https://en.wikipedia.org/wiki/Matrix_multiplication_algorithm

Matrix naiveMultiply(const Matrix& A, const Matrix& B) {
    int n = static_cast<int>(A.size());
    int p = static_cast<int>(B.size());
    int m = static_cast<int>(B[0].size());

    Matrix C(n, std::vector<int>(m, 0));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            int sum = 0;
            for (int k = 0; k < p; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
    return C;
}
