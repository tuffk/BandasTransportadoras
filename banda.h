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
int bandas;

void start(int a, int b, char * hostname, int fd, int* data)
{
	
	time_t kuz[capacidad];

//int fd;
//int *data;

double ElapsedTime(time_t x)
{
	return ((double)(time(NULL)-x));
}

typedef enum { PARADO, IZQUIERDA, DERECHA} SENTIDO;

//int data[actual*2+1];
//int data[actual*2];
int onBoard;
int /*bandas, */secciones, actual;
SENTIDO sentido;//1: ->; 2: <-; 0: parado;
time_t timer;
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
  time_t yaharadina;
	if(onBoard !=0)
	{
	   yaharadina = time(NULL);
	   while(ElapsedTime(yaharadina)<3)
	   {
	     
	   }
		//sleep(3);
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

void timeout()
{
	if(ElapsedTime(timer)>60)
		pararranca(PARADO);
}

int check()
{
    //printf("checando\n");
	if(forcestop)
		return;
	if(data[actual*2+1]==0 && data[actual*2] == 0){
		pararranca(PARADO);
	}else if(data[actual*2+1]==0){
		pararranca(IZQUIERDA);
		timer=time(NULL);
	}else if(data[actual*2]==0){
		pararranca(DERECHA);
		timer=time(NULL);
	}
	else if(sentido ==IZQUIERDA)
	{
		if(data[actual*2]<(data[actual*2+1] -4))
		{
			cambioSentido();
			timer=time(NULL);
		}
			
	}else if(sentido ==DERECHA){
		if(data[actual*2+1]<(data[actual*2]-4))
		{	
			cambioSentido();
			timer=time(NULL);
		}
	}
	timeout();
	
}

int decide()
{
	return rand()%2;
}


void cruzar()
{
	if(forcestop || (data[actual*2+1]==0 && data[actual*2] == 0))
		return;
	
	
	if(sentido==IZQUIERDA && capacidad>onBoard && data[actual*2] >0)
	{
	  //printf("EN LA IZQ %d\n",data[actual*2]);
		data[actual*2]--;
		//printf("EN LA IZQ  post cambio %d\n",data[actual*2]);
		onBoard++;
		//sleep(3);
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
		    //printf("decide\n");
			if(actual <(bandas-1))
			{
			    //printf("voy a mandar persona a la izquierda de %d, previa %d\n", actual+1,data[((actual+1)*2)]);
			    msync (FILEPATH, bandas*2*sizeof(int), MS_ASYNC);
				data[((actual+1)*2)]+=1;
				write(FILEPATH,&data,getpagesize());
				msync (FILEPATH, bandas*2*sizeof(int), MS_ASYNC);
				//printf("ya mande !!!!!-----------%d\n", data[((actual+1)*2)]);
				
			}
		}
		//check();
	}else if(sentido == DERECHA && capacidad>=onBoard && data[actual*2+1] > 0){
		//printf("EN LA DER %d\n",data[actual*2+1]);
		data[actual*2+1]--;
		//printf("EN LA DER %d\n",data[actual*2+1]);
		onBoard++;
		//sleep(3);
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
		    //printf("decide\n");
			if(actual >(0))
			{
			    //printf("voy a mandar una persona a la derecha de %d, previa: %d\n", actual-1,data[(((actual-1)*2)+1)]);
    			msync (FILEPATH, bandas*2*sizeof(int), MS_ASYNC);
				data[(((actual-1)*2)+1)]+=1;
				write(FILEPATH,&data,getpagesize());
				msync (FILEPATH, bandas*2*sizeof(int), MS_ASYNC);
				//printf("ya mande !!!!!-----------%d\n", data[(((actual-1)*2)+1)]);
			}
		}
		//check();
	}
	
	if(ElapsedTime(kuz[0]) > tcruse)
	{
		kuz[0] = (time_t)0;
		onBoard--;
		//printf("reste gente a la banda\n");
	}
	if(ElapsedTime(kuz[1]) > tcruse)
	{
		kuz[1] = (time_t)0;
		onBoard--;
				//printf("reste gente a la banda\n");
	}
	if(ElapsedTime(kuz[2]) > tcruse)
	{
		kuz[2] = (time_t)0;
		onBoard--;
				//printf("reste gente a la banda\n");
	}
	check();
}

void generamela()
{
  int r=0;
  if(data[actual*2+1]< 7)
  {
    r=rand()%300;
    if(r ==5)
      data[actual*2+1]++;
  }
  if(data[actual*2] < 7)
  {
    r=rand()%300;
    if(r ==5)
      data[actual*2]++;
  }
}

void miramela()
{
  printf("hostname= %s, thread =%d, d=%d , i=%d\n",hostname, actual,data[actual*2 + 1],data[actual*2]);
}
	
	// argumentos--------------------
	bandas=1; secciones=1; actual=0;
	//formula para sacar la fila data[actual*2+1] e data[actual*2] de la banda
	// # de banda x2= data[actual*2]    		data[actual*2]+1=data[actual*2+1]
	
		bandas=a;
		actual=b;
	
	//printf("%d",actual);
	//printf("%s\n",argv[1]);
	//printf("%s\n",argv[2]);
	
	/// argumentos--------------------
	
	//---------shared mem
	//fd = open(FILEPATH, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
	printf("hola soy %d, y fd: %d, data: %p\n",actual, fd, data);
	//data=mmap(0, bandas*2*sizeof(int), PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, fd, 0);
	//write(fd, "", 1);
	//msync(FILEPATH, bandas*2*sizeof(int), MS_ASYNC);
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
	data[actual*2+1] = rand()%50;
	data[actual*2] = rand()%50;
	onBoard=0;
	msync (FILEPATH, bandas*2*sizeof(int), MS_ASYNC);
	//data[actual*2]= data[actual*2]; 
	//data[actual*2+1] = data[actual*2+1];
	data[actual*2] = 15;
	data[actual*2+1] = 15;
	//data[actual*2]= data[actual*2];
	//data[actual*2+1] =  data[actual*2+1];
	write(FILEPATH,&data,getpagesize());
	msync (FILEPATH, bandas*2*sizeof(int), MS_ASYNC);
	//printf("pito (der)id:%d, : %d\n", actual, data[actual*2 + 1]);
	//printf("pito (izq)id:%d, : %d\n", actual, data[actual*2]);
	if(data[actual*2]>data[actual*2+1])
	{
		pararranca(IZQUIERDA);
	}else{
		pararranca(DERECHA);
	}
	printf("sentido: %d\n",sentido);
	//sleep(2);
	timer = time(NULL);
	int tajat=0;
	while(true)
	{
	tajat++;
	    msync (FILEPATH, bandas*2*sizeof(int), MS_ASYNC);
		//data[actual*2]= data[actual*2]; 
		//data[actual*2+1] = data[actual*2+1];
			//printf("data (der) id: %d, : %d\n", actual, data[actual*2 + 1]);
			//printf("data (izq) id: %d, : %d\n", actual, data[actual*2]);
			//sleep(3);
		cruzar();
		//msync (FILEPATH, bandas*2*sizeof(int), MS_ASYNC);
		//msync (FILEPATH, bandas*2*sizeof(int), MS_ASYNC);
		//data[actual*2]= data[actual*2]; 
		//data[actual*2+1] = data[actual*2+1];
		
		
		generamela();
		//data[actual*2]= data[actual*2];
		//data[actual*2+1] =  data[actual*2+1];
		write(FILEPATH,&data,getpagesize());
		msync (FILEPATH, bandas*2*sizeof(int), MS_ASYNC);
		if(tajat>1000000)
		{
		 tajat=0;
		 miramela();
		}
	}
	printf("dejaron de cruzar, d=%d , i=%d\n",data[actual*2+1],data[actual*2]);
}
