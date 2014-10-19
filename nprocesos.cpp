#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

	MPI_Init(NULL, NULL);
	int miRank, procs;

	MPI_Comm_rank(MPI_COMM_WORLD, &miRank);

	MPI_Comm_size(MPI_COMM_WORLD, &procs);
	int number, MPI_TAG = 0, sendTo = (miRank + 1)% procs, recieveFrom = (miRank - 1 + procs) % procs;

	while(number<20){
		if(miRank == 0)
		{
			number++;
			MPI_Send(&number, 1, MPI_INT, sendTo, MPI_TAG, MPI_COMM_WORLD);
			printf("proceso: %d envio numero: %d a : %d \n", miRank, number,sendTo);

		}
			MPI_Recv(&number, 1, MPI_INT, recieveFrom, MPI_TAG, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
			printf("proceso: %d recibio numero: %d de : %d \n", miRank, number,recieveFrom);
		if(miRank != 0)
			number++;
			MPI_Send(&number, 1, MPI_INT, sendTo, MPI_TAG, MPI_COMM_WORLD);
			printf("proceso: %d envio numero: %d a : %d \n", miRank, number,sendTo);
		}

	MPI::Finalize();
}
