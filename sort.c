#include <stdio.h>
#include <mpi.h>
#include <omp.h>

#define TAM 1000

int main(int argc, char** argv){
  int i;
  int procs, miRank;
  int parte, ind=0;
  MPI_Status st;

  int vet[TAM];//vector inicial
  int sol[TAM];//vector respuesta

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &miRank);

  parte = (int)(TAM/(procs-1));

  int res=(TAM%(procs-1));//Por ejemplo, si tam=100 y procs=28 => parte=3; 
                          //Me faltan 19 datos que procesar (100-(3*27))
                            
    
  if(miRank==0){//proc master
    for(i=0;i<TAM;i++){//inicializo el vector => TAM...1
      vet[i]=TAM-i;
    }
    for(i=1;i<procs;i++){//envio las partes para ser ordenadas            
      MPI_Send(&vet[ind],parte,MPI_INT,i,i,MPI_COMM_WORLD);
      ind=ind+parte;
    }
    
    if(res){//si hay res, entonces necesito un proceso extra para ordenar estos datos.
            //como no puedo crear un proc extra, el master ordenara las partes que faltan

  //   #pragma omp parallel private(i,sol) shared(vet)
     //{      
	#pragma omp for schedule(dynamic)
	for(i=0;i<res;i++){//ordena datos que faltan
	sol[i]=vet[TAM-1-i];
	}
//     }
    }

	
    for(i=1;i<procs;i++){//recive datos ordenados y los almacena en sol
      MPI_Recv(&sol[TAM-i*parte], parte, MPI_INT,i, MPI_ANY_TAG, MPI_COMM_WORLD, &st);            
    }
    
    for(i=0;i<TAM;i++){//imprime vector sol
      printf("%d ", sol[i]);
    }
    printf("\n");
  }
  else{//proc esclavos
    MPI_Recv(vet, parte, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &st);//recivo datos de master
    //#pragma omp parallel private(i,sol) shared(vet)
    //{      
	   #pragma omp for schedule(dynamic)
	    for(i=0;i<parte;i++){//ordeno master y los guardo en sol
	      sol[i]=vet[parte-1-i];
	    }
	    //ind=st.MPI_TAG-1*parte;
	    MPI_Send(sol,parte,MPI_INT,0,miRank,MPI_COMM_WORLD);//envio a master datos ordenados
     // }
    }

  MPI_Finalize();
  
  return 0;
}
