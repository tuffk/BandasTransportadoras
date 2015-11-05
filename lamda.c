#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <stdbool.h>
#include<math.h>
#include<unistd.h>
#define lambda 10.000000000

//double log(double x)


double numAleatorio(void)
{
double otra=0, aux;
srand((double) time(NULL));
otra=rand();
aux=otra/10000000000;
//printf("%f\n",aux);

return aux;
}

double  equiz(void)
{
double x=0;
x=(-1/lambda) * log(numAleatorio());
//x=(-1/lambda);
//printf("X fun: %f\n", x);
return x;
}//Cierre de equiz


int main(int argc, const char * argv[])
{
clock_t begin, end;
double time_spent;
while(true)
{
	sleep(equiz()*7);
	printf("espere %f segundos para crear %f\n",time_spent++,lambda);
}

return 0;
}//Cierred e main
