#include"libs.h"
#include"dinning.h"

int main(int argc, char** argv){

    switch(argc){
        case 3:
            switch(atoi(argv[2])){
                case 0:
                    let_them_eat_locks((uint8_t)(atoi(argv[1]) % omp_get_max_threads()));
                    break;

                case 1:
                    let_them_eat_FIFO((uint8_t)(atoi(argv[1]) % omp_get_max_threads()));
                    break;

            }

            return 0;

        default:
            fprintf(stderr, "ERROR: Wrong number of arguments");
            return 1;
    }
    
    fprintf(stderr, "ERROR: How did we get here?");
    return 0x45;

}