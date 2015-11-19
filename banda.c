#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#define BUFSIZE 64

#define FILEPATH "./mmaped.bin"

#define tcruse 2
#define capacidad 3
time_t kuz[capacidad];

int fd;
int *data;

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
	if(forcestop || (derecha==0 && izquierda == 0))
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
	
	
	
	// argumentos--------------------
	int bandas=1, secciones=1, actual=0;
	//formula para sacar la fila derecha e izquierda de la banda
	// # de banda x2= izquierda    		izquierda+1=derecha
	
	if(argv[1]!=NULL)
		bandas=atoi(argv[1]);
	if(argv[2]!=NULL)
		actual=atoi(argv[2]);
	
	//printf("%d",actual);
	//printf("%s\n",argv[1]);
	//printf("%s\n",argv[2]);
	
	/// argumentos--------------------
	
	//---------shared mem
	fd = open(FILEPATH, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
	data=mmap(0, bandas*2*sizeof(int), PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, fd, 0);
	write(fd, "", 1);
	msync(FILEPATH, bandas*2*sizeof(int), MS_ASYNC);
	/*
	data[1]=modishnes;
			write(FILEPATH,&data,getpagesize());
			msync (FILEPATH, 10*sizeof(int), MS_ASYNC);
			data[1]
	*/
	//----- shared mem end
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
	while(derecha >0 || izquierda >0)
	{
		izquierda= data[actual]; 
		derecha = data[actual+1];
		printf("dejaron de cruzar, d=%d , i=%d\n",derecha,izquierda);
		cruzar();
		printf("d=%d , i=%d\n",derecha,izquierda);
		data[actual]= izquierda;
		data[actual+1] =  derecha;
		write(FILEPATH,&data,getpagesize());
		msync (FILEPATH, bandas*2*sizeof(int), MS_ASYNC);
	}
	printf("dejaron de cruzar, d=%d , i=%d\n",derecha,izquierda);
	return 0;
}
