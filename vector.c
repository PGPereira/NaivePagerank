#include <stdbool.h>
#include <stdio.h>

/**
 * Verifica se todos os elementos do vetor dado são nulos.
 * @param  vector Ponteiro para uma matriz
 * @param  size   Tamanho da matriz
 * @return        Booleano que indica se o vetor é ou não, nulo.
 */
int isNullVector(double *vector, unsigned size) {
  unsigned i;
  int zero = 1;

  for (i = 0; i < size; i++) {
    if (vector[i] == 1) {
      zero = 0;
      break;
    }
  }

  return zero;
}

/**
 * Seta todos os elementos de um vetor para um
 * @param vector Ponteiro para um vetor
 * @param size   Tamanho do Vetor
 */
void setVectorToOne(double *vector, unsigned size) {
  unsigned i;
  for (i = 0; i < size; i++) {
    vector[i] = 1;
  }
}

/**
 * Normaliza o vetor (Torna-o unitário).
 * @param vector Ponteiro para um vetor.
 * @param size   Tamanho do vetor.
 */
void normalizeVector(double *vector, unsigned size) {
  unsigned i;
  double sum = 0;
  double multiplyFactor = 1;

  for (i = 0; i < size; i++)
    sum += vector[i];

  if (sum != 0)
    multiplyFactor /= sum;

  for (i = 0; i < size; i++)
    vector[i] *= multiplyFactor;
}

/**
 * Aplica o algoritmo de Damping a cada elemento do vetor
 * @param vector Ponteiro para vetor.
 * @param damp   "Dumbing Factor".
 * @param size   Tamanho do vetor.
 */
void dampVector(double *vector, double damp, unsigned size) {
  unsigned i;
  /*
    Ambos os valores são constantes, logo não faz sentido recalculá-los
    incessantemente
   */
  const double addFactor = damp / size;
  const double multiplyFactor = 1 - damp;

  for (i = 0; i < size; i++) {
    vector[i] = multiplyFactor * vector[i] + addFactor;
  }
}
/**
 * Imprime os elementos do vetor.
 * @param vector Ponteiro para vetor.
 * @param size   Tamanho do vetor.
 */
void printVector(double *vector, unsigned size) {
  unsigned i = 0;
  for (i = 0; i < size; i++)
    printf("%lf ", vector[i]);

  printf("\n");
}

void printIndexedVector(double *vector, unsigned size) {
  unsigned i = 0;
  for (i = 0; i < size; i++)
    printf("%d %.4lf\n", i, vector[i]);
}
