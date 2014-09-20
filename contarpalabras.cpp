#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
using namespace std;


pthread_mutex_t mutex;

int numThreads=10;
char cad[100];


void dividir_texto(int myId){
    int longitud=strlen(cad);
    longitud=longitud/numThreads;

}


void *worker(void *arg)
{
  int id = *((int *) arg);
  dividir_texto(id);
  return NULL;
}




int main()
{

    pthread_t threads[numThreads];

    int * p;
     for (int i = 0; i < numThreads; i++) {
        p = (int *) malloc(sizeof(int));
        *p = i;
        pthread_create(&threads[i], NULL, worker, (void *)(p));
     }

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_destroy(&mutex);

    gets(cad);
    int espacios = 0;

    for(int i=0;i<strlen(cad); i++)
    {
        if(cad[i] == 32)
        {
            espacios++;
            while(cad[i+1] == 32) i++;
        }

    }

    printf("El numero de palabras es %d", (espacios+1));

return 0;
}



