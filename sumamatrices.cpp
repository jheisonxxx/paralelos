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

//Crear una  matriz
  vals = (double *) malloc (matrizTam * matrizTam * sizeof(double));


  temp = (double **) malloc (matrizTam * sizeof(double*));

  for(i=0; i < matrizTam; i++)
    temp[i] = &(vals[i * matrizTam]);

  return temp;
}


//Imprimir una matriz
void printMatrix(double **mat) {
  int i,j;

  printf(" %d * %d es\n\n", matrizTam, matrizTam);
  for(i=0; i < matrizTam; i++){
    for(j=0; j < matrizTam; j++)
      printf("%lf ",  mat[i][j]);
    printf("\n");
  }
}


//funcion del worker o hilo para sumar por filas
void sumar(int myId) {
  int i,j,k;
  double sum;

  // Limites para los hilos
  int inicio_fila = myId * matrizTam/numThreads;
  int fin_fila = (myId+1) * (matrizTam/numThreads) - 1;

  // Multiplicacion de Matrices
  for (i = inicio_fila; i <= fin_fila; i++) {
    for (j = 0; j < matrizTam; j++) {
      sum = 0.0;
      //for (k = 0; k < matrizTam; k++) {
	c[i][j] =  a[i][j] + b[i][j];

      //}

    }
  }
}

//hilo
void *worker(void *arg)
{
  int id = *((int *) arg);
  sumar(id);
  return NULL;
}


int main(int argc, char *argv[]) {
  int i, j;
  int *p;
  pthread_t *threads;

  //Aca se define el tamano de la matriz y el numero de hilos
  matrizTam = 8;
  numThreads = 4;


  //Creando la matrices
  a = allocateMatrix();
  b = allocateMatrix();
  c = allocateMatrix();



//llenando la matriz
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
    cout<<"Matriz A";
    printMatrix(a);
    cout<<endl;
    cout<<"Matriz A";
    printMatrix(b);
    cout<<endl;
    cout<<"Matriz Resultado";
    printMatrix(c);
}
