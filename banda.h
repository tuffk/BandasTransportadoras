/*
 * para llamar a este programa nesesitas 2 argumentos
 * el primer argumento es la cantidad de bandas que hay en la seccion
 * el segundo argumento es la banda actual
 * */

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

typedef enum { PARADO, IZQUIERDA, DERECHA} SENTIDO;

time_t kuz[capacidad];

int fd;
int *data;
int derecha;
int izquierda;
int onBoard;
int bandas, secciones, actual;
SENTIDO sentido;//1: ->; 2: <-; 0: parado;
time_t timer;
bool forcestop=false;


void gestor_usrsig1(int sig)
{
    printf("TEST\n\n\n");
	if(forcestop)
		forcestop=!forcestop;
	else
		forcestop=!forcestop;
}

double ElapsedTime(time_t x)
{
	return ((double)(time(NULL)-x));
}

void aguantamela()
{
	if(onBoard !=0)
	{
		sleep(3);
		//printf("espero\n");
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
		timer=time(NULL);
	}else if(izquierda==0){
		pararranca(DERECHA);
		timer=time(NULL);
	}
	else if(sentido ==IZQUIERDA)
	{
		if(izquierda<(derecha -4))
		{
			cambioSentido();
			timer=time(NULL);
		}
			
	}else if(sentido ==DERECHA){
		if(derecha<(izquierda-4))
		{	
			cambioSentido();
			timer=time(NULL);
		}
	}
	timeout();
	
}

void timeout()
{
	if(ElapsedTime(timer)>60)
		pararranca(PARADO);
}

void cruzar()
{
	if(forcestop || (derecha==0 && izquierda == 0))
		return;
	
	if(sentido==IZQUIERDA && capacidad>onBoard && izquierda >0)
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
		if(decide())
		{
			if(actual <(bandas-1))
			{
				data[((actual+1)*2)]+=1;
				write(FILEPATH,&data,getpagesize());
				msync (FILEPATH, bandas*2*sizeof(int), MS_ASYNC);
			}
		}
		check();
	}else if(sentido == DERECHA && capacidad>=onBoard && derecha > 0){
		derecha--;
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
		
		if(decide())
		{
			if(actual >(0))
			{
				data[(((actual-1)*2)+1)]+=1;
				write(FILEPATH,&data,getpagesize());
				msync (FILEPATH, bandas*2*sizeof(int), MS_ASYNC);
				
			}
		}
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

int decide()
{
	return rand()%2;
}

void generamela()
{
  int r=0;
  if(derecha< 7)
  {
    r=rand()%300;
    if(r ==5)
      derecha++;
  }
  if(izquierda < 7)
  {
    r=rand()%300;
    if(r ==5)
      izquierda++;
  }
}

void start(int a, int b)
{

	//signal(SIGINT, SIG_IGN);
	//signal(SIGUSR1, gestor_usrsig1);
	// argumentos--------------------
	bandas=1; secciones=1; actual=0;
	//formula para sacar la fila derecha e izquierda de la banda
	// # de banda x2= izquierda    		izquierda+1=derecha
	
		bandas=a;
		actual=b;
	
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
	msync (FILEPATH, bandas*2*sizeof(int), MS_ASYNC);
	izquierda= data[actual]; 
	derecha = data[actual+1];
	izquierda = derecha = 15;
	if(izquierda>derecha)
	{
		pararranca(IZQUIERDA);
	}else{
		pararranca(DERECHA);
	}
	printf("sentido: %d\n",sentido);
	sleep(3);
	timer = time(NULL);
	while(true)
	{
		//izquierda= data[actual]; 
		//derecha = data[actual+1];
		if(!forcestop)
		{
		    cruzar();
		    printf("d=%d , i=%d\n",derecha,izquierda);
		    generamela();
		    data[actual]= izquierda;
		    data[actual+1] =  derecha;
		    write(FILEPATH,&data,getpagesize());
		    msync (FILEPATH, bandas*2*sizeof(int), MS_ASYNC);
		}
	}
	printf("dejaron de cruzar, d=%d , i=%d\n",derecha,izquierda);
}
