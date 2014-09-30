#include<stdio.h>
#include<windows.h>
#include<ctype.h>
#include<tchar.h>
#include <sys/types.h>
#include <pthread.h>
#include<conio.h>
#include <iostream>

#include<stdio.h>
#include<windows.h>
#include<ctype.h>
#include<tchar.h>
#include <sys/types.h>
#include <pthread.h>
#include<conio.h>
#include <iostream>

#define nrothreads 8
#define MAX 100

using namespace std;


//guarda la informacion del buffer e informacion del hilo
struct bloque {
    int inicio;
    int ID;
    int result;
    char buffer2[MAX];


};
//vector de arrays para cada thread
struct bloque particiones[nrothreads];




//el contador de threads
void * ThreadFun(void * param){
    int contador = 0;
    //set working struct and part of buffer where search for words starts
    struct bloque *temp = (struct bloque *)param;
    contador = particiones[temp->ID].inicio;
    //algorithm for finding words based on spaces
    while (temp->buffer2[contador] != '!' && temp->buffer2[contador] != '\0') {

        if (temp->buffer2[contador] == ' ' || temp->buffer2[contador] == '\t' || temp->buffer2[contador] == '\n'){
            particiones[temp->ID].result += 1;
        }
        contador++;

    }

  //  cout<<"result"<<particiones[temp->ID].result<<"id"<<temp->ID<<endl;


}




int main(int argc, char *argv[]) {

    int rc;
    FILE *open;
    open = fopen("jheison1.txt", "r");

    // ThreadId[nrothreads];


    pthread_t threads[nrothreads];



    char buffer[MAX] = { 0 };
    buffer[MAX-1] = '!';


    for (int i = 0; i < nrothreads; i++){
        particiones[i].inicio = 0;
        particiones[i].result = 0;
        particiones[i].ID = i;
    }


    int c;
    int contadorfinal = 0;
    //checkea  si el primer caracter es una palabra y lo incrementa
    c = getc(open);
    if (!isspace(c)){ contadorfinal = 1; }
    rewind(open);


    //llenando el buffer con el archivo
    while ((c = getc(open)) != EOF) {

        memset(buffer, 0, MAX);

       // cout<<"imprimiendo"<<c<<endl;

        for (int i = 0; i < MAX; i++){if (i == 49 || i == 99){ buffer[i] = '!'; }
            if (i != 49 && i != 99){ buffer[i] = c; }
            if ((c = getc(open)) != EOF){}
            //guarda una no palabra para que la threas pare

            else break;
        }
        //guarda el buffer dentro de la estructuras de particiones
        for (int i = 0; i < nrothreads; i++){
            for (int j = 0; j < MAX; j++){
                particiones[i].buffer2[j] = buffer[j];
            }
        }
        for (int i = 0; i < nrothreads; i++){
                                                                                                                                                                                                            //if (particiones[i + 1].inicio == 0){}
           rc=pthread_create(&threads[i], NULL, ThreadFun, (void *) &particiones[i]);
            particiones[i + 1].inicio += (MAX / nrothreads) * (i+1);
          // cout<<(MAX / nrothreads) * (i+1)<<endl;
        }

        for (int i = 0; i < nrothreads; i++){
            rc = pthread_join(threads[i], NULL);
        }


        for (int i = 0; i < nrothreads; i++){
            particiones[i].inicio = 0;
        }
    }

    for (int i = 0; i < nrothreads; i++){
        contadorfinal += particiones[i].result;
    }

    fclose(open);
    printf("%d", contadorfinal);


    return 0;
}
