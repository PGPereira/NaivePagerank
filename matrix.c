#include <stdbool.h>
#include <jemalloc/jemalloc.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "matrix.h"
#include "vector.h"

static double convergeValue = 1E-12;
static unsigned maximumExponent = 2000;

/**
 * Função que aloca uma matriz dinamicamente.
 * @param lines   um inteiro que representa a quantidade de linhas de uma matriz
 * @param columms um inteiro que representa a quantidade de colunas de uma
 *  matriz
 */
void matrixAlloc(double ***matriz, unsigned lines, unsigned columms) {
  unsigned i;
  double **matrix;

  matrix = (double **)calloc(lines, sizeof(double *));

  for (i = 0; i < lines; i++)
    matrix[i] = (double *)calloc(columms, sizeof(double));

  *matriz = matrix;
}

/**
 * Função para liberar uma matriz da memória.
 * @param matrix Ponteiro duplo (matriz é bidimensional)
 * @param lines Quantidade de linhas que existem na dada matriz.
 */
void matrixFree(double **matrix, unsigned lines) {
  unsigned i;

  for (i = 0; i < lines; i++)
    free(matrix[i]);

  free(matrix);
}

/**
 * Função que imprime o conteúdo de uma matriz.
 * @param matrix  Ponteiro para uma matriz.
 * @param lines   Quantidade de linhas da matriz
 * @param columms Quantidade de colunas da matriz
 */
void printMatrix(double **matrix, unsigned lines, unsigned columms) {
  unsigned i;

  printf("\n");

  for (i = 0; i < lines; i++)
    printVector(matrix[i], columms);

  printf("\n");
}

/**
 * Transforma a matriz de entrada em uma matriz estocástica:
 * a - Se uma linha da matriz é nula todos os elementos são substituidos por 1.
 * b - A soma de todos os elementos de uma linha deve dar 1.
 * @param matrix  Ponteiro para uma matriz.
 * @param lines   Quantidade de linhas na dada matriz.
 * @param columms Quantidade de colunas na dada matriz.
 */
void stochasticMatrix(double **matrix, unsigned lines, unsigned columms) {
  unsigned i;
  for (i = 0; i < lines; i++) {
    if (isNullVector(matrix[i], columms) == true)
      setVectorToOne(matrix[i], columms);

    normalizeVector(matrix[i], columms);
  }
}

/**
 * Modela a matriz com um fator conhecido por 'Damping Factor' pela seguinte
 * fórmula:
 * M[i][j] = (1 - damp)*S[i][j] + damp/N
 * @param matrix  Um ponteiro para uma matriz.
 * @param damp    O 'damping factor' a ser utilizado.
 * @param lines   Quantidade de linhas na matriz.
 * @param columms Quantidade de colunas na matriz.
 */
void dampMatrix(double **matrix, double damp, unsigned lines,
                unsigned columms) {
  unsigned i;

  for (i = 0; i < lines; i++)
    dampVector(matrix[i], damp, columms);
}

/**
 * memset extendido para uso em uma matriz bidimensional.
 * @param matrix  Ponteiro para uma matriz bidimensional.
 * @param lines   Quantidade de linhas na matriz.
 * @param columms Quantidade de colunas na matriz.
 * @param value   Valor a ser utilizado na operação de memset.
 */
void matrixMemorySet(double **matrix, unsigned lines, unsigned columms,
                     int value) {
  unsigned i;
  for (i = 0; i < lines; i++)
    memset(matrix[i], value, columms);
}

/**
 * Multiplicação de duas matrizes quadradas.
 * @param matrixA Matriz mais a esquerda.
 * @param matrixB Matriz mais a direita.
 * @param saida   Matriz de saída.
 * @param lado    Dimensão N das matrizes NxN.
 */
void squareMatrixMultiplication(double **matrixA, double **matrixB,
                                double **saida, unsigned lado) {
  unsigned i, j, k;

  matrixMemorySet(saida, lado, lado, 0);

  for (i = 0; i < lado; i++) {
    for (j = 0; j < lado; j++) {
      for (k = 0; k < lado; k++) {
        saida[i][j] += matrixA[i][k] * matrixB[k][j];
      }
    }
  }
}

/**
 * Essa função checa se a exponenciação de matriz convergiu à um valor
 * específico. Esse valor é definido como uma constante no cabeçalho da
 * biblioteca e aqui é definido por 1E-12.
 * Parte-se do pré-suposto que os expoentes seguem a seguinte regra M > N.
 * @param  matrixM Matriz exponenciada a M.
 * @param  matrixN Matriz exponenciada a N.
 * @param  size    Tamanho da matriz.
 * @return         Inteiro que indentifica se a matriz convergiu ou não.
 */
int matrixHasConverged(double **matrixM, double **matrixN, unsigned size) {
  double **matrixD = NULL, norm;

  matrixDifference(matrixM, matrixN, &matrixD, size, size);
  norm = matrixNorm(matrixD, size, size);
  matrixFree(matrixD, size);

  return (norm <= convergeValue);
}

/**
 * Calcula a diferença entre duas matrizes.
 * @param M          [description]
 * @param N          [description]
 * @param difference [description]
 * @param lines      [description]
 * @param columms    [description]
 */
void matrixDifference(double **M, double **N, double ***difference,
                      unsigned lines, unsigned columms) {
  unsigned i, j;
  double **D;
  matrixAlloc(&D, lines, columms);

  for (i = 0; i < lines; i++) {
    for (j = 0; j < columms; j++) {
      D[i][j] = M[i][j] - N[i][j];
    }
  }

  *difference = D;
}

/**
 * Calcula a norma de uma matriz.
 * @param  matrix  Um ponteiro para uma matriz.
 * @param  lines   Quantidade de linhas na matriz.
 * @param  columms Quantidade de colunas na matriz.
 * @return         A norma da matriz.
 */
double matrixNorm(double **matrix, unsigned lines, unsigned columms) {
  double norm = 0;
  unsigned i, j;

  for (i = 0; i < lines; i++) {
    for (j = 0; j < columms; j++) {
      norm += matrix[i][j] * matrix[i][j];
    }
  }

  return norm;
}

/**
 * Exponecia a matriz até a norma da matriz ^ m ser semelhante à norma
 * da matriz ^ n, determinado por um valor de convergência.
 * @param matrixExp  [description]
 * @param matrixBase [description]
 * @param size       [description]
 * @param iteration  [description]
 */
void expontiationUntilConverge(double ***matrixExp, double **matrixBase,
                               unsigned size, unsigned iteration) {
  double **matrixExpPlusOne = NULL;
  matrixAlloc(&matrixExpPlusOne, size, size);

  squareMatrixMultiplication(*matrixExp, matrixBase, matrixExpPlusOne, size);

  if (!matrixHasConverged(matrixExpPlusOne, *matrixExp, size) &&
      iteration <= maximumExponent) {
    expontiationUntilConverge(&matrixExpPlusOne, matrixBase, size, ++iteration);
  }

  matrixFree(*matrixExp, size);
  *matrixExp = matrixExpPlusOne;
}

/**
 * Faz uma cópia de uma matriz para uma segunda matriz.
 * @param output  Ponteiro para a matriz que vai receber a cópia.
 * @param input   Ponteiro para a matriz que vai ser copiada.
 * @param lines   Linhas da matriz principal.
 * @param columms Colunas da matriz principal.
 */
void copyMatrix(double ***output, double **input, unsigned lines,
                unsigned columms) {
  unsigned i, j;
  double **newPointer;
  if (*output != NULL)
    matrixFree(*output, lines);

  matrixAlloc(&newPointer, lines, columms);
  for (i = 0; i < lines; i++) {
    for (j = 0; j < columms; j++) {
      newPointer[i][j] = input[i][j];
    }
  }

  *output = newPointer;
}
