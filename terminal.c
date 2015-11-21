#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <pthread.h>

#define PATH "./banda"

int bandas;
pid_t * tids;
void gestor_usrsig1(int sig)
{
    for (int i = 0; i < bandas; i++)
    {
        pthread_kill(*tids+i, SIGUSR1);
    }
}

int main(int argc, const char * argv[])
{
    bandas = atoi(argv[1]);
    tids = malloc(sizeof(pid_t) * bandas);
    signal(SIGINT, gestor_usrsig1);
    #pragma omp parallel num_threads(bandas) shared(tids)
    {

        pid_t tid = syscall(SYS_gettid);

        //pid_t tid = gettid(); 
        int id;
        
        *(tids+id) = tid;

        id = omp_get_thread_num();
        char bandas_char[10];
        char id_char[10];
        sprintf(bandas_char, "%d", bandas);

        sprintf(id_char, "%d", id);
        printf("a %s %s\n", bandas_char, id_char);
        execl(PATH, bandas_char, id_char);
        
    }
    free (tids);
    return 0;
}
