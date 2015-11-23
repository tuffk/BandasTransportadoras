#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <pthread.h>
#include "banda.h"
#include <mpi.h>


#define PATH "./banda"

//int bandas;
//pid_t * tids;
void master();
void modishness();


int main(int argc, const char * argv[])
{
    bandas = atoi(argv[1]);
    int provided;
    int myid, numprocs;
    //int data[MAXSIZE];
    //int rbuf[MAXSIZE];
    //int i, n, x, low, high, myresult = 0, result = 0;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    int  longitud;
    MPI_Init(&argc,&argv);
    //MPI_Init_thread(&argc, &argv, MPI_THREAD_SERIALIZED, &provided);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    
    
    MPI_Get_processor_name(hostname, &longitud);
    //tids = malloc(sizeof(pid_t) * bandas);
    //signal(SIGTSTP, gestor_ctrlz);
    
    if(myid == 0)
    {
        printf("Hostname: %s\n", hostname);
        
        master();
    }
    else
    {
    	printf("Hostname:%s\n", hostname);
        #pragma omp parallel num_threads(bandas + 1)
        {     
            int id = omp_get_thread_num();
	        if(id ==bandas)
	        {
	            sleep(3);
	            modishness();
	        }
            else
            {
            //signal(SIGTSTP, gestor_usrsig1);
            start(bandas, id, hostname);
            }
        }
    }
    //free (tids);
    MPI_Finalize();
    
    return 0;
}

void master()
{
    printf("Hola soy el master\n");  
    int r1,r2;
    MPI_Recv(&r1,1,MPI_INT,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    //MPI_Recv(&r2,1,MPI_INT,2,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    printf("en la terminal %d hay %d personas en las bandas\nen la terminal %d hay %d personas en las bandas",1,r1,2,r2);
    
    
}

void modishness()
{
    MPI_Request request = MPI_REQUEST_NULL;
    int fd;
int *data;
  int now=0,i,cont=0;
  printf("1\n");
  printf("%d", bandas);
  fd = open(FILEPATH, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
	data=mmap(0, bandas*2*sizeof(int), PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, fd, 0);
	printf("2\n");
	if(data == -1)
	    printf("error de mapeo");
	write(fd, "", 1);
	msync(FILEPATH, bandas*2*sizeof(int), MS_ASYNC);
  while(true)
  {

    now++;
    if(now ==100000)
    {
      msync(FILEPATH, bandas*2*sizeof(int), MS_ASYNC);
      for(i=0;i<bandas;++i)
      {
	cont += (int)data[i];
	//printf("%d!!\n", (int)data[i]);
      }
      //printf("%d!!!\n", cont);
      now=0;
      //printf("en la terminal x hay %d personas en las bandas",cont);
      MPI_Send(&cont,1,MPI_INT,0,0,MPI_COMM_WORLD);
      cont = 0;
      
    }
  }
}
