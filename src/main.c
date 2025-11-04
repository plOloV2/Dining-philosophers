#include"libs.h"
#include"dinning.h"

int main(int argc, char** argv){

    switch(argc){
        case 2:
            let_them_eat_locks((uint8_t)(atoi(argv[1]) % (omp_get_max_threads() + 1)));
            return 0;

        default:
            fprintf(stderr, "ERROR: Wrong number of arguments\n");
            return 1;
    }
    
    fprintf(stderr, "ERROR: How did we get here?\n");
    return 0x45;

}