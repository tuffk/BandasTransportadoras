#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define PATH "./banda"
int main(int argc, const char * argv[])
{
    int bandas = atoi(argv[1]);
    
    #pragma omp parallel num_threads(bandas)
    {
        int id;
        id = omp_get_thread_num();
        char bandas_char[10];
        char id_char[10];
        sprintf(bandas_char, "%d", bandas);
        sprintf(id_char, "%d", id);
        execl(PATH, bandas_char, id_char);
    }
    return 0;
}
