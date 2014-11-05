#include <stdio.h>
#include <stdlib.h>

void srandom (unsigned seed);
double dboard (int darts);
#define DARTS 50000     
#define ROUNDS 10       
#define MASTER 0       

int main (int argc, char *argv[]) {

double	homepi,       
	pi,             
	avepi,        
	pirecv,         
	pisum;   
int	i, n;


 srandom (0);

 avepi = 0;
 for (i = 0; i < ROUNDS; i++) {
   pi = dboard(DARTS);
   
  
   avepi = ((avepi * i) + pi)/(i + 1); 
   printf("   Despues %8d , valor promedio de pi = %10.8f\n",
	  (DARTS * (i + 1)),avepi);
        
 } 
 
 return 0;
}

#include <stdio.h>
#include <stdlib.h>
#define sqr(x)	((x)*(x))
long random(void);

double dboard(int darts)
  {
  double x_coord, y_coord, pi, r; 
  int score, n;
  unsigned int cconst;  
  if (sizeof(cconst) != 4) {
    printf("error tamano de la data para variable constante en rutina del dboard\n");
    exit(1);
    }
  cconst = 2 << (31 - 1);
  score = 0;


  for (n = 1; n <= darts; n++)  {

    r = (double)random()/cconst;
    x_coord = (2.0 * r) - 1.0;
    r = (double)random()/cconst;
    y_coord = (2.0 * r) - 1.0;

   
    if ((sqr(x_coord) + sqr(y_coord)) <= 1.0)
         score++;
    }

  /* calcular pi */
  pi = 4.0 * (double)score/(double)darts;
  return(pi);
  } 
