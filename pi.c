#include "mpi.h"
#include <math.h>

int main(argc,argv)
int argc;
char *argv[];
{
    int cero = 0, n, myid, numprocs, i;
    double valor_de_pi = 3.141592653589793238462643;
    double mypi, pi, h, sum, x;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    while (!cero)
    {
	if (myid == 0) {
	    printf("Ingresar numero de intervalos:");
	    scanf("%d",&n);
	}
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (n == 0) break;
  
	h   = 1.0 / (double) n;
	sum = 0.0;
	for (i = myid + 1; i <= n; i += numprocs) {
	    x = h * ((double)i - 0.5);
	    sum += 4.0 / (1.0 + x*x);
	}
	mypi = h * sum;
    
	MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0,
		   MPI_COMM_WORLD);
    
	if (myid == 0)
	    printf("pi es es %.16f, Error es %.16f\n",
		   pi, fabs(pi - valor_de_pi));
    }
    MPI_Finalize();
    return 0;
}
