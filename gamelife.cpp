#include "mpi.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>


using namespace std;
int main(int argc, char* argv[])
{
int size, rank, tag, rc, N, iteraciones, outPoints, s;
MPI_Status Stat;
ofstream output("output.txt"); //Archivo de Salida
rc=MPI_Init(&argc,&argv);
if (rc!=0) {cout << "Error starting MPI." << endl; MPI_Abort(MPI_COMM_WORLD, rc);}
MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
if (rank==0){
// Proceso 0
if (argc<2) {
cout << "Mal Salida" << endl;
MPI_Finalize();
return -1;
}
ifstream file(argv[1]); //archivo de entrada
if (!file)
{
cout << "Error por Abrir Archivo"<<endl;
MPI_Finalize();
return -1;
}
file >> N >> iteraciones >> outPoints; //primeras tres variables del archivo
s=N/size; //diviciones
int thematr[N][N];
for (int i=0;i<N;i++){ //leer archivo en un array
string temp;
file >> temp;
for (int j=0; j<N; j++) thematr[i][j]=temp[j]-'0';
}
file.close();
//enviar informacion inicial (N, k, #iteraciones, output points) 
int info[4];
info[0]=N; info[1]=s; info[2]=iteraciones; info[3]=outPoints;
for (int dest=0; dest<size; dest++) MPI_Send(&info, 4, MPI_INT, dest, 1, MPI_COMM_WORLD); //enviar info
int division[N/size][N];
for (int z=0; z<size; z++)
{
for (int k=0; k<s; k++)
for (int l=0; l<N; l++)
division[k][l]=thematr[k+(z*s)][l]; //dividir una particion de la matriz
MPI_Send(&division, N*s, MPI_INT, z, 1, MPI_COMM_WORLD); //enviar
}
} // fin del proceso 0 
//recibir informacion inicial
int localinfo[4]; // información local para obtener información inicial
MPI_Recv(&localinfo, 4, MPI_INT, 0, 1, MPI_COMM_WORLD, &Stat); //recibir info
int mydivision[localinfo[1]][localinfo[0]]; //propia divicion de la matriz
MPI_Recv(&mydivision, localinfo[0]*localinfo[1], MPI_INT, 0, 1, MPI_COMM_WORLD, &Stat); //recibir particion
N = localinfo[0]; //asignar variables
s = localinfo[1]; 
iteraciones=localinfo[2]; 
outPoints=localinfo[3]; 
int todown[N]; int toup[N]; int fromdown[N]; int fromup[N]; //arrays para enviar y recibir
for (int g=1; g<=iteraciones; g++) //iteraciones 
{
if (rank!=size-1) // todos, excepto para el último envío de abajo
{
for (int j=0; j<N; j++) todown[j]=mydivision[s-1][j];
MPI_Send(&todown, N, MPI_INT, rank+1, 1, MPI_COMM_WORLD);
} else {
for (int k=0; k<N; k++) fromdown[k]=0; } // ultimo genera vacio de abajo
if (rank!=0) // todos, excepto para el primero q recibio de arriba 
{
MPI_Recv(&fromup, N, MPI_INT, rank-1, 1, MPI_COMM_WORLD, &Stat);
} else { for (int k=0; k<N; k++) fromup[k]=0; } // el primero genera una linea vacia de arriba
if (rank!=0) //  todos, excepto para el primero q envio para arriba 
{
for (int j=0; j<N; j++) toup[j]=mydivision[0][j];
MPI_Send(&toup, N, MPI_INT, rank-1, 1, MPI_COMM_WORLD);
}
if (rank!=size-1) // todos, excepto para el ultimo q recibio de arriba
{
MPI_Recv(&fromdown, N, MPI_INT, rank+1, 1, MPI_COMM_WORLD, &Stat);
}
//contando vecinos
int sum=0; // suma de vecinos
int mynewdivision[s][N];
for (int x=0; x<s; x++) //para cada fila
{
for (int y=0; y<N; y++) //para cada columna
{
if (x==0 && y==0) //celdas de esquina
sum = mydivision[x+1][y]+mydivision[x+1][y+1]+mydivision[0][y+1]+fromup[0]+fromup[1];
else if (x==0 && y==N-1) 
sum = mydivision[x][y-1]+mydivision[x+1][y-1]+mydivision[x+1][y]+fromup[N-1]+fromup[N-2];
else if (x==s-1 && y==0) 
sum = mydivision[x][y+1]+mydivision[x-1][y+1]+mydivision[x-1][y]+fromdown[0]+fromdown[1];
else if (x==s-1 && y==N-1) 
sum = mydivision[x-1][y]+mydivision[x-1][y-1]+mydivision[x][y-1]+fromdown[N-1]+fromdown[N-2];
else // no celdas de esquina
{
if (y==0) // 
sum=mydivision[x-1][y]+mydivision[x-1][y+1]+mydivision[x][y+1]+mydivision[x+1][y+1]+mydivision[x+1][y];
else if (y==N-1) 
sum=mydivision[x-1][y]+mydivision[x-1][y-1]+mydivision[x][y-1]+mydivision[x+1][y-1]+mydivision[x+1][y];
else if (x==0) 
sum=mydivision[x][y-1]+mydivision[x+1][y-1]+mydivision[x+1][y]+mydivision[x+1][y+1]+mydivision[x][y+1]+fromup[y-1]+fromup[y]+fromup[y+1];
else if (x==s-1) 
sum=mydivision[x-1][y-1]+mydivision[x-1][y]+mydivision[x-1][y+1]+mydivision[x][y+1]+mydivision[x][y-1]+fromdown[y-1]+fromdown[y]+fromdown[y+1];
else 
sum=mydivision[x-1][y-1]+mydivision[x-1][y]+mydivision[x-1][y+1]+mydivision[x][y+1]+mydivision[x+1][y+1]+mydivision[x+1][y]+mydivision[x+1][y-1]+mydivision[x][y-1];
}
//nuevo valor de la celda
if (mydivision[x][y]==1 && (sum==2 || sum==3)) mynewdivision[x][y]=1;
else if (mydivision[x][y]==1 && sum>3) mynewdivision[x][y]=0;
else if (mydivision[x][y]==1 && sum<1) mynewdivision[x][y]=0;
else if (mydivision[x][y]==0 && sum==3) mynewdivision[x][y]=1;
else mynewdivision[x][y]=0;
}
}
// copiar nueva divicion en midivicion
for (int x=0; x<s; x++)
for (int y=0; y<N; y++)
mydivision[x][y]=mynewdivision[x][y];
//imprimir el resultado en archivo
if (g%outPoints==0) 
if (rank==0)
{
int amatr[s][N];
output << "Iteraciones " << g << ":" << endl;
for (int x=0; x<s; x++) 
{
for (int y=0; y<N; y++) output << midivision[x][y];
output << endl;
}
for (int i=1; i<size; i++)
{
MPI_Recv(&amatr, N*s, MPI_INT, i, 1, MPI_COMM_WORLD, &Stat); //recibir de otros
for (int x=0; x<s; x++)
{
for (int y=0; y<N; y++) output << amatr[x][y];
output << endl;
}
}
output << endl << endl;
}
else MPI_Send(&midivision, N*s, MPI_INT, 0,1, MPI_COMM_WORLD);
}
} //fin del bucle
output.close();
MPI_Finalize();
}
