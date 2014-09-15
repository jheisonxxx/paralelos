#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <iostream>
#define MAX_SIZE 4096
#define NO_PROCESS 8

using namespace std;


int N=100;
int A[100][100];
int B[100][100];
int C[100][100];
int D[100][100];

//Matriz de la Multiplicacion de AB
int X[100][100];
//Matriz de la Multiplicacion de CD
int Y[100][100];
//Matriz de la suma de X+Y
int R[100][100];


//Inicializar Matriz A
void * Init_MatrixA(void * p)
{


    int i, j;
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                    A[i][j] =i+j;
            }
        }
}


//Inicializar Matriz B
void * Init_MatrixB(void * p)
{
    int i, j;
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                    B[i][j] = i+2*j;
            }
        }
}
//Inicializar Matriz C
void * Init_MatrixC(void * p)
{
    int i, j;
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                    C[i][j] = 2*i+3*j;
            }
        }
}

//Inicializar Matriz D
void * Init_MatrixD(void * p)
{
    int i, j;
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                    D[i][j] =2*i+j;
            }
        }
}



//Imprimir una matriz
void printMatrix(int mat[100][100]) {
  int i,j;

  printf(" %d * %d es\n\n", N, N);
  for(i=0; i < N; i++){
    for(j=0; j < N; j++)
      printf("%d ",  mat[i][j]);
    printf("\n");
  }
}


//Multiplicacion X=A*B
void * multAB(void * myId) {
  int i,j,k;
  int sum;


  // Multiplicacion de Matrices
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      sum = 0;
      for (k = 0; k < N; k++) {
        X[i][j] = X[i][j]+A[i][k] + B[k][j];
      }
    }
  }
}


//Multiplicacion Y=C*D
void * multCD(void * myId) {
  int i,j,k;
  int sum;
  // Multiplicacion de Matrices
  for (i = 0; i <= N; i++) {
    for (j = 0; j < N; j++) {
      sum = 0;
      for (k = 0; k < N; k++) {
        Y[i][j] = Y[i][j]+C[i][k] + D[k][j];
      }
    }
  }
}


//Sumar en paralelo agrupado por filas R=X+Y
void sumar(int myId) {
  int i,j,k;
  int sum;

  // Limites para los hilos
  int inicio_fila = myId * N/4;
  int fin_fila = (myId+1) * (N/4) - 1;

  // Multiplicacion de Matrices
  for (i = inicio_fila; i <= fin_fila; i++) {
    for (j = 0; j < N; j++) {
      sum = 0;
      //for (k = 0; k < matrizTam; k++) {
	R[i][j] =  X[i][j] + Y[i][j];

      //}
    }
  }
}

//hilo para funcion sumar
void *worker(void *arg)
{
  int id = *((int *) arg);
  sumar(id);
  return NULL;
}



int main()
{
    //Inicializacion de las matrices en paralelo
    pthread_t Aid,Bid,Cid,Did;
    int numThreads=4;
    int p;
    pthread_create(&Cid,NULL,Init_MatrixA,&p);
    pthread_create(&Cid,NULL,Init_MatrixB,&p);
    pthread_create(&Cid,NULL,Init_MatrixC,&p);
    pthread_create(&Cid,NULL,Init_MatrixD,&p);


    pthread_join(Aid, NULL);
    pthread_join(Bid, NULL);
    pthread_join(Cid, NULL);
    pthread_join(Did, NULL);

     //Multiplicacion de las matrices en paralelo X=A*B y Y=C*D
    pthread_t ABid,CDid;
    pthread_create(&ABid,NULL,multAB,&p);
    pthread_create(&CDid,NULL,multCD,&p);

    pthread_join(ABid, NULL);
    pthread_join(CDid, NULL);


    //Suma de las matrices X+Y en paralelo

    pthread_t *threads;
      // creando threads
    threads = (pthread_t *) malloc(numThreads * sizeof(pthread_t));
    int * q;
    for (int i = 0; i < numThreads; i++) {
        q = (int *) malloc(sizeof(int));
        *q = i;
        pthread_create(&threads[i], NULL, worker, (void *)(q));
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }


    //Impresiones
    cout<<"Matriz A"<<endl;
    printMatrix(X);
    cout<<endl<<endl;
    cout<<"Matriz B"<<endl;
    printMatrix(B);
    cout<<endl<<endl;
    cout<<"Matriz C"<<endl;
    printMatrix(C);
    cout<<endl<<endl;
    cout<<"Matriz D"<<endl;
    printMatrix(D);
    cout<<endl<<endl;
    cout<<"Matriz AB"<<endl;
    printMatrix(X);
    cout<<endl<<endl;
    cout<<"Matriz CD"<<endl;
    printMatrix(Y);
    cout<<endl<<endl;

    cout<<"Matriz Resultado"<<endl;
    printMatrix(R);



    return 0;
}
