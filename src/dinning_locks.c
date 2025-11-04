#include"libs.h"

void let_them_eat_locks(uint8_t num_philosophes){

    if(num_philosophes < 3){

        fprintf(stderr, "ERROR: entered number of philosophes is to low, min is 3.\n");
        return;

    }

    omp_lock_t* table = malloc(sizeof(omp_lock_t) * (num_philosophes - 1));
    if(!table){

        fprintf(stderr, "ERROR: omp_lock_t* table alloc failed\n");
        return;

    }

    uint8_t* what_do_i_do = calloc((num_philosophes - 1), sizeof(uint8_t));
    if(!what_do_i_do){

        fprintf(stderr, "ERROR: uint8_t* what_do_i_do alloc failed\n");
        return;

    }

    uint64_t* times_eaten = calloc((num_philosophes - 1), sizeof(uint64_t));
    if(!times_eaten){

        fprintf(stderr, "ERROR: uint64_t* times_eaten alloc failed\n");
        return;

    }

    for(uint8_t i = 0; i < (num_philosophes - 1); i++)
        omp_init_lock(&table[i]);

    #pragma omp parallel num_threads(num_philosophes) shared(table, what_do_i_do, times_eaten)
    {

        uint8_t id, forks_owned, left, right, left_eat_id, right_eat_id;

        id = omp_get_thread_num();

        if(id != num_philosophes - 1){

            forks_owned = 0;
            left = id;
            right = (id + 1) % (num_philosophes - 1);
            left_eat_id = (id == 0) ? (num_philosophes - 2) : (id - 1);
            right_eat_id = (id == (num_philosophes - 2)) ? 0 : (id + 1);

            unsigned int seed;
            FILE* f = fopen("/dev/urandom", "rb");
            if(f && fread(&seed, sizeof(seed), 1, f) == 1){

                fclose(f);

            } else{

                if(f)
                    fclose(f);
                
                seed = (unsigned int)time(NULL);

            }

            while(1){

                uint64_t I_eat, left_eat, right_eat;
                #pragma omp atomic read
                    I_eat = times_eaten[id];
                #pragma omp atomic read
                    left_eat = times_eaten[left_eat_id];
                #pragma omp atomic read
                    right_eat = times_eaten[right_eat_id];

                if(I_eat <= left_eat && I_eat <= right_eat){

                    if(id % 2 == 0){

                        if(omp_test_lock(&table[left])){
                            forks_owned++;

                            if(omp_test_lock(&table[right])){

                                forks_owned++;

                            } else{

                                forks_owned--;

                                omp_unset_lock(&table[left]);

                            }

                        }

                    }else {

                        if(omp_test_lock(&table[right])){

                            forks_owned++;

                            if(omp_test_lock(&table[left])){

                                forks_owned++;

                            } else{

                                forks_owned--;

                                omp_unset_lock(&table[right]);

                            }

                        }

                    }

                }

                if(forks_owned == 2){

                    #pragma omp atomic write
                        what_do_i_do[id] = 1;

                    sleep(rand_r(&seed) % 5 + 1);

                    omp_unset_lock(&table[left]);
                    omp_unset_lock(&table[right]);

                    forks_owned = 0;

                    #pragma omp atomic write
                        what_do_i_do[id] = 0;
                    
                    #pragma omp atomic
                        times_eaten[id] += 1;

                }

                usleep(100000);
                
            }

        } else{

            uint8_t first_run = 1;
            
            while(1){

                if(first_run){

                    printf("\n=========== Philosopher States ===========\n");

                    for(uint8_t i = 0; i < (num_philosophes - 1); i++)
                        printf("Philosopher %d: THINKING\n", i + 1);

                    printf("==========================================\n");
                    first_run = 0;

                }
                
                printf("\033[%dA", (num_philosophes - 1) + 2);
                printf("\r");
                
                printf("=========== Philosopher States ===========\n");
                
                for(uint8_t i = 0; i < (num_philosophes - 1); i++){

                    uint8_t state;
                    #pragma omp atomic read
                        state = what_do_i_do[i];

                    uint64_t eaten_times;
                    #pragma omp atomic read
                        eaten_times = times_eaten[i];

                    printf("Philosopher %d: %s, times he eaten: %ld\n", i + 1, state ? "EATING  " : "THINKING", eaten_times);

                }
                
                printf("==========================================\n");
                
                usleep(100000);

            }

        }

    }

    for(uint8_t i = 0; i < (num_philosophes - 1); i++)
        omp_destroy_lock(&table[i]);

    free(times_eaten);
    free(table);
    free(what_do_i_do);

}
