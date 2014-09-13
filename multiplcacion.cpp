#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#define MAX_SIZE 4096
#define NO_PROCESS 8

using namespace std;


int N=100;
int A[100][100];
int B[100][100];
int C[100][100];
int D[100][100];

void * Init_MatrixA()
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

void * Init_MatrixB()
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

void * Init_MatrixC()
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
void  * Init_MatrixD()
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


int main()
{
    pthread_t threads[4];

    pthread_t Aid,Bid,Cid,Did;
    pthread_create(&Aid,NULL,Init_MatrixA,NULL);
    pthread_create(&Bid,NULL,Init_MatrixB,NULL);
    pthread_create(&Cid,NULL,Init_MatrixC,NULL);
    pthread_create(&Did,NULL,Init_MatrixD,NULL);

    pthread_join(Aid,NULL);
    pthread_join(Bid,NULL);
    pthread_join(Cid,NULL);
    pthread_join(Did,NULL);


    return 0;
}
