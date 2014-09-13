#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

double ** a, ** b, ** c;
int numThreads, matrizTam;

double **allocateMatrix() {
  int i;
  double *vals, **temp;

  // allocate values
  vals = (double *) malloc (matrizTam * matrizTam * sizeof(double));

  // allocate vector of pointers
  temp = (double **) malloc (matrizTam * sizeof(double*));

  for(i=0; i < matrizTam; i++)
    temp[i] = &(vals[i * matrizTam]);

  return temp;
}

void printMatrix(double **mat) {
  int i,j;

  printf("La %d * %d matriz es\n", matrizTam, matrizTam);
  for(i=0; i < matrizTam; i++){
    for(j=0; j < matrizTam; j++)
      printf("%lf ",  mat[i][j]);
    printf("\n");
  }
}

void mm(int myId) {
  int i,j,k;
  double sum;

  // Limites para los hilos
  int inicio_fila = myId * matrizTam/numThreads;
  int fin_fila = (myId+1) * (matrizTam/numThreads) - 1;

  // Multiplicaion de Matrices Sobre la Division
  for (i = inicio_fila; i <= fin_fila; i++) {
    for (j = 0; j < matrizTam; j++) {
      sum = 0.0;
      for (k = 0; k < matrizTam; k++) {
	sum = sum + a[i][k] * b[k][j];
      }
      c[i][j] = sum;
    }
  }
}

void *worker(void *arg)
{
  int id = *((int *) arg);
  mm(id);
  return NULL;
}


int main(int argc, char *argv[]) {
  int i, j;
  int *p;
  pthread_t *threads;
  matrizTam = 9;
  numThreads = 3;

  a = allocateMatrix();
  b = allocateMatrix();
  c = allocateMatrix();

//imprimir

  for (i = 0; i < matrizTam; i++)
    for (j = 0; j < matrizTam; j++) {
      a[i][j] = i + j;
      b[i][j] = i + j;
    }


  // creando threads
  threads = (pthread_t *) malloc(numThreads * sizeof(pthread_t));

  // Creando threads
  for (i = 0; i < numThreads; i++) {
    p = (int *) malloc(sizeof(int));
    *p = i;
    pthread_create(&threads[i], NULL, worker, (void *)(p));
  }

  for (i = 0; i < numThreads; i++) {
    pthread_join(threads[i], NULL);
  }

    printMatrix(a);
    printMatrix(b);
    cout<<endl;
    printMatrix(c);
}
