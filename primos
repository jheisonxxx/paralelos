#include <stdio.h>
#include <omp.h>
#include <iostream>    
 
using namespace std;  
  
bool esPrimo(int n);  
  
int main()  
{  
    int num=2, n, sum=0;  
    float prom=0;  
    printf("Ingrese n: ");  
    scanf("%d",&n);  
    printf("\n=> ");
int cont;
#pragma omp parallel shared(sum) 
#pragma omp for schedule(static,1) reduction(+:sum) 
   for(cont=0;cont<n;cont++){ 

        if(esPrimo(num)){  
            printf("%d ",num); 

            sum = sum + num;  
            cont++;  
        }  
        num++;  
    }  
    printf("\n\nSuma: %d \n",sum);  

 
    return 0;  
}  
  
bool esPrimo(int n){  
    int i;  
    if(n == 2)  
        return true;  
    if(n%2 == 0 || n==1 )  
        return false;  
    for(i=2; i<=n/2; i++){  
        if(n%i == 0)  
            return false;  
    }  
    return true;  
}  
