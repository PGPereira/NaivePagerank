#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "matrix.h"
#include "vector.h"

int main(int argc, char const *argv[]) {
  unsigned D;             // Numero de paginas. (Dimensão da matriz)
  double dampingFactor;   // Fator de Damping
  double **matriz = NULL; // Matriz a ser alocada.
  double **saida = NULL;  // Matriz de saida.
  unsigned i, j;          // Posições onde a matriz sera setada para 1

  scanf("%u %lf", &D, &dampingFactor);
  matrixAlloc(&matriz, D, D);

  while (scanf("%u %u", &i, &j) == 2) {
    matriz[i][j] = 1;
  }

  stochasticMatrix(matriz, D, D);
  dampMatrix(matriz, dampingFactor, D, D);

  copyMatrix(&saida, matriz, D, D);
  expontiationUntilConverge(&saida, matriz, D, 0);
  printIndexedVector(saida[0], D);

  matrixFree(saida, D);
  matrixFree(matriz, D);

  return 0;
}
