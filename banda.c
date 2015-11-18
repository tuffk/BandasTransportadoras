#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>

#define tcruse 2
#define capacidad 3
time_t kuz[capacidad];
double ElapsedTime(time_t x)
{
	return ((double)(time(NULL)-x));
}

typedef enum { PARADO, IZQUIERDA, DERECHA} SENTIDO;

int derecha;
int izquierda;
int onBoard;
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

void aguantamela()
{
	if(onBoard !=0)
	{
		sleep(3);
	}
}

void cambioSentido()
{
	if(forcestop)
		return;
	if(sentido==IZQUIERDA)
	{
		aguantamela();
		sentido = DERECHA;
	}
	else if(sentido == DERECHA)
	{
		aguantamela();
		sentido = IZQUIERDA;
	}
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
	
	if(sentido==IZQUIERDA && capacidad>onBoard)
	{
		izquierda--;
		onBoard++;
		if(kuz[0]== (time_t)0)
		{
			kuz[0] = time(NULL);
		}else if(kuz[1]== (time_t)0)
		{
			kuz[1] = time(NULL);
		}else if(kuz[2]== (time_t)0){
			kuz[2] = time(NULL);
		}
		check();
	}else if(sentido == DERECHA && capacidad>=onBoard){
		derecha--;
		check();
	}
	
	if(ElapsedTime(kuz[0]) > tcruse)
	{
		kuz[0] = (time_t)0;
		onBoard--;
	}
	if(ElapsedTime(kuz[1]) > tcruse)
	{
		kuz[1] = (time_t)0;
		onBoard--;
	}
	if(ElapsedTime(kuz[2]) > tcruse)
	{
		kuz[2] = (time_t)0;
		onBoard--;
	}
}

int main(int argc, const char * argv[])
{
	//seccion de test---------------------
	
	time_t pito=time(NULL);
	printf("%g\n",pito);
	pito = (time_t) 0;
	printf("%g\n",pito);
	if(pito == (time_t)0)
	{
		printf("es comparable\n");
	}else{
		printf("no es\n");
	}
	
	/// fin seccion test --------------------
	int i=0;
	for(;i<capacidad;++i)
	{
		kuz[i]= (time_t) 0;
	}
	srand((int)time(NULL));
	derecha = rand()%50;
	izquierda = rand()%50;
	onBoard=0;
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
