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


int main(int argc, const char * argv[])
{
    bandas = atoi(argv[1]);
    
    int myid, numprocs;
    //int data[MAXSIZE];
    //int rbuf[MAXSIZE];
    //int i, n, x, low, high, myresult = 0, result = 0;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    int  longitud;
    MPI_Init(&argc,&argv);
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
        #pragma omp parallel num_threads(bandas)
        {     
            //pid_t tid = syscall(SYS_gettid);
            int id = omp_get_thread_num();
            //*(tids+id) = tid;
            //printf("%d %d\n", id, *(tids+id));


            
            //signal(SIGTSTP, gestor_usrsig1);
            start(bandas, id);
        }
    }
    //free (tids);
    MPI_Finalize();
    
    return 0;
}

void master()
{
    printf("Hola soy el master\n");        
}
