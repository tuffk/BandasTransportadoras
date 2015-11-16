#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>

#define tcruse 2
#define capacidad 3

typedef enum { PARADO, IZQUIERDA, DERECHA} SENTIDO;

int derecha;
int izquierda;
SENTIDO sentido;//1: ->; 2: <-; 0: parado;
double timer;
bool forcestop=false;

void gestor_usrsig1(int sig)
{
	if(forcestop)
		forcestop=!forcestop;
	else
		forcestop=!forcestop;
}

void cambioSentido()
{
	if(forcestop)
		return;
	if(sentido==IZQUIERDA)
		sentido = DERECHA;
	else if(sentido == DERECHA)
		sentido = IZQUIERDA;
}

void pararranca(SENTIDO x)
{
	if(forcestop)
		return;
	if(x==PARADO)// parar la banda
		sentido =PARADO;
	else
		sentido = x;//arranca la banda con el sentido dado
}

int check()
{
	if(forcestop)
		return;
	if(derecha==0 && izquierda == 0){
		pararranca(PARADO);
	}else if(derecha==0){
		pararranca(IZQUIERDA);
		timer=0;
	}else if(izquierda==0){
		pararranca(DERECHA);
		timer=0;
	}
	else if(sentido ==IZQUIERDA)
	{
		if(izquierda<(derecha -4))
		{
			cambioSentido();
			timer=0;
		}
			
	}else if(sentido ==DERECHA){
		if(derecha<(izquierda-4))
		{	
			cambioSentido();
			timer=0;
		}
	}
	timeout();
	
}

void timeout()
{
	if(timer>60)
		pararranca(PARADO);
}

void cruzar()
{
	if(forcestop)
		return;
	
	if(sentido==IZQUIERDA)
	{
		izquierda--;
		check();
	}else if(sentido == DERECHA){
		derecha--;
		check();
	}
}

int main(int argc, const char * argv[])
{
	srand((int)time(NULL));
	derecha = rand()%50;
	izquierda = rand()%50;
	pararranca(DERECHA);
	printf("sentido: %d\n",sentido);
	sleep(3);
	while(derecha !=0 || izquierda !=0)
	{
		cruzar();
		printf("d=%d , i=%d\n",derecha,izquierda);
	}
	printf("dejaron de cruzar, d=%d , i=%d\n",derecha,izquierda);
	return 0;
}
