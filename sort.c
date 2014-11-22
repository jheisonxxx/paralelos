#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <omp.h>


void merge(int *, int *, int, int, int);
void mergeSort(int *, int *, int, int);

int main(int argc, char** argv) {
	
	//Crear Vector y Llenar Aleatoriamente
	int tam_vector = 1000;
	//printf(atoi(argv[0]));
	int * vector_original = (int*)malloc(tam_vector * sizeof(int));
	
	
	int c;
	srand(time(NULL));
	//printf("Vector Desordenado:  ");
	for(c = 0; c < tam_vector; c++) {
		vector_original[c] = rand() % tam_vector;
		//printf("%d ", vector_original[c]);
	}

	//printf("\n");
	//printf("\n");
	
	
	//Iniciar MPI
	int miRank;
	int procs;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &miRank);
	MPI_Comm_size(MPI_COMM_WORLD, &procs);
	
	
	if(miRank==0){
		printf("Vector Desordenado:  ");
		for(c = 0; c < tam_vector; c++) {
			printf("%d ", vector_original[c]);
		}
		printf("\n");
		printf("\n");
	}
		
	//Dividir el vector en partes iguales 
	int tam = tam_vector/procs;
	//Enviar cada sub_vector a cada proceso 
	int *sub_vector = (int*)malloc(tam * sizeof(int));
	MPI_Scatter(vector_original, tam, MPI_INT, sub_vector, tam, MPI_INT, 0, MPI_COMM_WORLD);
	

	
	//Merge Sort para cada proceso y subvector
	
	int *tmp_vector = (int*)malloc(tam * sizeof(int));
	mergeSort(sub_vector, tmp_vector, 0, (tam - 1));
	
	//Gather vectores ordenados en uno
	
	int * ordenado = NULL;
	if(miRank == 0) {
		
		ordenado  = (int*)malloc(tam_vector * sizeof(int));
		
		}
	
	MPI_Gather(sub_vector, tam, MPI_INT, ordenado , tam, MPI_INT, 0, MPI_COMM_WORLD);
	
	//Ultima llamada a funcion merge
	if(miRank == 0) {
		
		int *other_vector = (int*)malloc(tam_vector * sizeof(int));
		mergeSort(ordenado , other_vector, 0, (tam_vector - 1));
		
		//Imprimir Vector
		printf("Vector Ordenado: ");
		for(c = 0; c < tam_vector; c++) {
			
			printf("%d ", ordenado [c]);
			
			}
			
		printf("\n");
		printf("\n");
			
		}
	
	
	
	//Fin MPI
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	
	}

//Funcion Merge
void merge(int *a, int *b, int l, int m, int r) {
	
	int h, i, j, k;
	h = l;
	i = l;
	j = m + 1;
	
	#pragma omp parallel //schedule(dynamic,1)
	while((h <= m) && (j <= r)) {
		
		if(a[h] <= a[j]) {
			
			b[i] = a[h];
			h++;
			
			}
			
		else {
			
			b[i] = a[j];
			j++;
			
			}
			
		i++;
		
		}
		
	if(m < h) {
		#pragma omp parallel for schedule(dynamic,1)
		for(k = j; k <= r; k++) {
			
			b[i] = a[k];
			i++;
			
			}
			
		}
		
	else {
		#pragma omp parallel for schedule(dynamic,1)
		for(k = h; k <= m; k++) {
			
			b[i] = a[k];
			i++;
			
			}
			
		}
	
	#pragma omp parallel for schedule(dynamic,1)
	for(k = l; k <= r; k++) {
		
		a[k] = b[k];
		
		}
		
	}

//Funcion Recursiva
void mergeSort(int *a, int *b, int l, int r) {
		
	int m;
	
	if(l < r) {
		
		m = (l + r)/2;
		#pragma omp parallel sections
		{	
			#pragma omp section
				mergeSort(a, b, l, m);
			#pragma omp section
				mergeSort(a, b, (m + 1), r);
		}
		
		merge(a, b, l, m, r);
		
	}
		
}

