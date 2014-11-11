#include "mpi.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>


using namespace std;


int main(int argc, char* argv[])
{
	int size, rank, tag, rc, N, nro_iteraciones, secuencia, s;
	MPI_Status Stat;
	ofstream output("output.txt"); 	//Botar en archivo
	rc=MPI_Init(&argc,&argv);
	if (rc!=0) {
		cout << "Error del MPI." << endl; 
		MPI_Abort(MPI_COMM_WORLD, rc);
	}
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank==0){
		// Procesador 0
		if (argc<2) {
			cout << "No hay archivo de entrada" << endl;
			MPI_Finalize();
			return -1;
		}
			
		ifstream file(argv[1]);	//Archivo de Entrada

		if (!file)
		{
			cout << "Error al abrir el archivo"<<endl;
			MPI_Finalize();
			return -1;
		}
	
		file >> N >> nro_iteraciones >> secuencia;	//Tres variables N->Tamano de la matriz secuencia->
		s=N/size;	//s -> Divisiones 
		int tablero[N][N];	
		
		
	
		for (int i=0;i<N;i++){	//Leer archivo en temp 
			string temp;	
			file>>temp;
			for (int j=0; j<N; j++) tablero[i][j]=temp[j]-'0';
			for(int u=0;u<N;u++) cout<<tablero[i][u];
			cout<<endl;
		}
		file.close();
		
		
		
		//Enviar informacion inicial (N, k, #nro_iteraciones, output points) a cada proceso
		int info[4];
		info[0]=N; info[1]=s; info[2]=nro_iteraciones; info[3]=secuencia;
		for (int dest=0; dest<size; dest++) MPI_Send(&info, 4, MPI_INT, dest, 1, MPI_COMM_WORLD); //enviar informacion
		int division[N/size][N];	
		for (int z=0; z<size; z++)
		{
			for (int k=0; k<s; k++) 
				for (int l=0; l<N; l++) 
					division[k][l]=tablero[k+(z*s)][l];	//hacer una division del tablero
			MPI_Send(&division, N*s, MPI_INT, z, 1, MPI_COMM_WORLD);	//enviar
		}
	} // fin del proceso 0

	//reciviendo informacion
	int localinfo[4];		// informacion local recibido del proceso 0
	MPI_Recv(&localinfo, 4, MPI_INT, 0, 1, MPI_COMM_WORLD, &Stat);
	int mydivision[localinfo[1]][localinfo[0]]; //sacando informacion para asignar la division a cada proceso
	MPI_Recv(&mydivision, localinfo[0]*localinfo[1], MPI_INT, 0, 1, MPI_COMM_WORLD, &Stat);	//recibir cada division
	N = localinfo[0];			//assignar variables
	s = localinfo[1];			
	nro_iteraciones=localinfo[2];	
	secuencia=localinfo[3];		
	
	int to_abajo[N];	int to_arriba[N]; int de_abajo[N]; int de_arriba[N]; //arrays para enviar y recibir
	for (int g=1; g<=nro_iteraciones; g++) //nro_iteraciones para el bucle
	{	

		if (rank!=size-1) // todos excepto para el ultimo que envia de abajo
		{
			for (int j=0; j<N; j++) to_abajo[j]=mydivision[s-1][j];
			MPI_Send(&to_abajo, N, MPI_INT, rank+1, 1, MPI_COMM_WORLD);

		} else {
			for (int k=0; k<N; k++) de_abajo[k]=0; } // ultimo en generar vacio de a a bajo

		if (rank!=0) // todo excepto para el primero que recibe de arriba
		{
			MPI_Recv(&de_arriba, N, MPI_INT, rank-1, 1, MPI_COMM_WORLD, &Stat);	

		} else { for (int k=0; k<N; k++) de_arriba[k]=0; } // primero que genera linea vacia  de arriba	
	
		if (rank!=0) // todos excepto para el primero que envia arriba
		{
			for (int j=0; j<N; j++) to_arriba[j]=mydivision[0][j];
			MPI_Send(&to_arriba, N, MPI_INT, rank-1, 1, MPI_COMM_WORLD);
		}
	
		if (rank!=size-1) // todos excepto para ultimo que recibe de abajo
		{
			MPI_Recv(&de_abajo, N, MPI_INT, rank+1, 1, MPI_COMM_WORLD, &Stat);
		}

		//contar celdas vecinas
		int sum=0; // suma de vecinos
		int mynewdivision[s][N];
		for (int x=0; x<s; x++) //filas
		{	
			for (int y=0; y<N; y++) //celdas de esquina
			{
				if (x==0 && y==0) 
					sum = mydivision[x+1][y]+mydivision[x+1][y+1]+mydivision[0][y+1]+de_arriba[0]+de_arriba[1];
				else if (x==0 && y==N-1)
					sum = mydivision[x][y-1]+mydivision[x+1][y-1]+mydivision[x+1][y]+de_arriba[N-1]+de_arriba[N-2];
				else if (x==s-1 && y==0) 
					sum = mydivision[x][y+1]+mydivision[x-1][y+1]+mydivision[x-1][y]+de_abajo[0]+de_abajo[1];
				else if (x==s-1 && y==N-1) 
					sum = mydivision[x-1][y]+mydivision[x-1][y-1]+mydivision[x][y-1]+de_abajo[N-1]+de_abajo[N-2];
				else // no celdas de esquina 
				{
					if (y==0) 
						sum=mydivision[x-1][y]+mydivision[x-1][y+1]+mydivision[x][y+1]+mydivision[x+1][y+1]+mydivision[x+1][y];
					else if (y==N-1) 
						sum=mydivision[x-1][y]+mydivision[x-1][y-1]+mydivision[x][y-1]+mydivision[x+1][y-1]+mydivision[x+1][y];
					else if (x==0) 
						sum=mydivision[x][y-1]+mydivision[x+1][y-1]+mydivision[x+1][y]+mydivision[x+1][y+1]+mydivision[x][y+1]+de_arriba[y-1]+de_arriba[y]+de_arriba[y+1];
					else if (x==s-1) 
						sum=mydivision[x-1][y-1]+mydivision[x-1][y]+mydivision[x-1][y+1]+mydivision[x][y+1]+mydivision[x][y-1]+de_abajo[y-1]+de_abajo[y]+de_abajo[y+1];
					else 
						sum=mydivision[x-1][y-1]+mydivision[x-1][y]+mydivision[x-1][y+1]+mydivision[x][y+1]+mydivision[x+1][y+1]+mydivision[x+1][y]+mydivision[x+1][y-1]+mydivision[x][y-1];
				}
				
				//actualizar new celda de acuerdo a los vecinos
				if (mydivision[x][y]==1 && (sum==2 || sum==3)) mynewdivision[x][y]=1;
				else if (mydivision[x][y]==1 && sum>3) mynewdivision[x][y]=0;
				else if (mydivision[x][y]==1 && sum<1) mynewdivision[x][y]=0;
				else if (mydivision[x][y]==0 && sum==3) mynewdivision[x][y]=1;
		 		else mynewdivision[x][y]=0;
			
			}
		}
	
		// copiar nueva division en mydivision
		for (int x=0; x<s; x++)
			for (int y=0; y<N; y++)
				mydivision[x][y]=mynewdivision[x][y];

		//Imprimir resultado en archivo
		if (g%secuencia==0) //imprimir interaciones de acuerdo a la secuencia
		{
			if (rank==0) 
			{
				int aTablero[s][N];
				output << "Iteracion" << g << ":" << endl;
				for (int x=0; x<s; x++) //poner mi propia division
				{
					for (int y=0; y<N; y++)	output << mydivision[x][y];
					output << endl;
				}
				for (int i=1; i<size; i++)
				{
					MPI_Recv(&aTablero, N*s, MPI_INT, i, 1, MPI_COMM_WORLD, &Stat); //recibe de los demas
					for (int x=0; x<s; x++)
					{
						for (int y=0; y<N; y++) output << aTablero[x][y];
						output << endl;
					}
				}
				output << endl << endl;
			}
			else MPI_Send(&mydivision, N*s, MPI_INT, 0,1, MPI_COMM_WORLD);

		
		}	
	} // fin del bucle iteraciones 

output.close();
MPI_Finalize();
}
