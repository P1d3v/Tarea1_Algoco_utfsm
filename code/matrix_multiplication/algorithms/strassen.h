#ifndef STRASSEN_H
#define STRASSEN_H

#include <vector>

using namespace std;
using Matrix = vector<vector<int>>;

Matrix strassenMultiply(const Matrix& A, const Matrix& B);

#endif