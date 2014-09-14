#include <iostream>
#include <pthread.h>
#include <stdio.h>

using namespace std;

#define MAX_elem_prod_cons  50        /* tamaño del elem_prod_cons */

void* Productor(void*);
void* Consumidor(void*);

pthread_mutex_t mutex;		/* mutex para controlar el acceso al "elem_prod_cons" compartido */
pthread_cond_t no_lleno;	/* esperar si no está lleno */
pthread_cond_t no_vacio;	/* esperar si no está vacío */

int contador=0;				/* número de elementos en el "elem_prod_cons" */
int elem_prod_cons[MAX_elem_prod_cons];	/* "elem_prod_cons" común */
int DATOS_A_PRODUCIR;

int main(int argc, char *argv[])
{
    int status;
	cout<<endl<<"---------------------------------"<<endl;
	cout<<" Cantidad de DATOS_A_PRODUCIR : ";
	cin>>DATOS_A_PRODUCIR;
	cout<<"---------------------------------"<<endl;

	pthread_t th_consumidor1, th_productor2;

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&no_lleno, NULL);
	pthread_cond_init(&no_vacio, NULL);

	pthread_create(&th_consumidor1, NULL, Productor, (void *)0);
	pthread_create(&th_productor2, NULL, Consumidor, (void *)0);

	pthread_join(th_consumidor1, NULL);
	pthread_join(th_productor2, NULL);

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&no_lleno);
	pthread_cond_destroy(&no_vacio);

    system("PAUSE");
    exit(0);
    return EXIT_SUCCESS;
}


void * Productor(void * data) {
	int dato_prod,pos = 0;

	for(int i=0; i<DATOS_A_PRODUCIR; i++ )
    {
		dato_prod = i;							 	/* producir dato */
		pthread_mutex_lock(&mutex);			 		/* acceder al elem_prod_cons */
			while (contador == MAX_elem_prod_cons)
				pthread_cond_wait(&no_lleno, &mutex);	/* se bloquea  si elem_prod_cons lleno */

			elem_prod_cons[pos] = i;
			pos = (pos + 1) % MAX_elem_prod_cons;		/* convertir necesario para no exceder el tamaño*/
			contador = contador + 1;

			if (contador == 1)
				pthread_cond_signal(&no_vacio);			/* elem_prod_cons no vacío */

		pthread_mutex_unlock(&mutex);
		cout<<"Produce: ["<<dato_prod<<"]"<<endl;
	}
	pthread_exit(0);
}

void * Consumidor(void * data)
{
	int dato_cons,pos = 0;
	for(int i=0; i<DATOS_A_PRODUCIR; i++ )
    {
		pthread_mutex_lock(&mutex);						/* acceder al elem_prod_cons */
			while (contador == 0)
				pthread_cond_wait(&no_vacio, &mutex); 	/* se bloquea si elem_prod_cons vacío*/

			dato_cons = elem_prod_cons[pos];
			pos = (pos + 1) % MAX_elem_prod_cons;
			contador = contador - 1 ;

			if (contador == MAX_elem_prod_cons - 1);
				pthread_cond_signal(&no_lleno);			/* elem_prod_cons no lleno */

		pthread_mutex_unlock(&mutex);
		cout<<"Consume: ["<<dato_cons<<"]"<<endl;
	}
	pthread_exit(0);
}
