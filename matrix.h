#ifndef _MATRIX
#define _MATRIX

// Matrix Basic Operations
void matrixAlloc(double ***, unsigned, unsigned);
void matrixFree(double **, unsigned);
void printMatrix(double **, unsigned, unsigned);
void squareMatrixMultiplication(double **, double **, double **, unsigned);
void matrixMemorySet(double **, unsigned, unsigned, int);
void copyMatrix(double ***, double **, unsigned, unsigned);
void matrixDifference(double **, double **, double ***, unsigned, unsigned);

// Non-basic Matrix Operations
int matrixHasConverged(double **, double **, unsigned);
void stochasticMatrix(double **, unsigned, unsigned);
void dampMatrix(double **, double, unsigned, unsigned);
void expontiationUntilConverge(double ***, double **, unsigned, unsigned);
double matrixNorm(double **, unsigned, unsigned);

#endif
