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

#define PATH "./banda"

//int bandas;
//pid_t * tids;


int main(int argc, const char * argv[])
{
    bandas = atoi(argv[1]);
    //tids = malloc(sizeof(pid_t) * bandas);
    //signal(SIGTSTP, gestor_ctrlz);
    #pragma omp parallel num_threads(bandas)
    {     
        //pid_t tid = syscall(SYS_gettid);
        int id = omp_get_thread_num();
        //*(tids+id) = tid;
        //printf("%d %d\n", id, *(tids+id));


        
        //signal(SIGTSTP, gestor_usrsig1);
        start(bandas, id);
    }
    //free (tids);
    return 0;
}
