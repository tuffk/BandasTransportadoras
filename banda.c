#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <stdbool.h>
#include<math.h>
#include<unistd.h>

#define tcruse 2
#define capacidad 3

int derecha;
int izquierda;
int sentido;//1: ->; 2: <-; 0: parado;
double timer;

void cambioSentido()
{
	if(sentido==1)
		sentido = 2;
	else if(sentido == 2)
		sentido = 1;
}

void pararranca(int x)
{
	if(x==3)// parar la banda
		sentido =0;
	else
		sentido = x;//arranca la banda con el sentido dado
}

int check(bool s)
{
	if(derecha==0 && izquierda == 0){
		pararranca(3);
	}else if(derecha==0){
		pararranca(1);
		timer=0;
	}else if(izquierda==0){
		pararranca(2);
		timer=0;
	}
	else if(sentido ==1)
	{
		if(izquierda<(derecha -4))
		{
			cambioSentido();
			timer=0;
		}
			
	}else if(sentido ==2){
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
		pararranca(3);
}

int main(argc, const char * argv[])
{

	return 0;
}
