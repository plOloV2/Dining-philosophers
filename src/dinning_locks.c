#include"libs.h"
#include"table.h"

void let_them_eat_locks(uint8_t num_philosophes){

    struct fork** table = malloc(num_philosophes * sizeof(struct fork*));
    if(!table){

        fprintf(stderr, "ERROR: struct fork** table alloc failed");
        return;

    }

    for(uint8_t i = 0; i < num_philosophes; i++){

        table[i] = malloc(sizeof(struct fork));
        if(!table[i]){

            fprintf(stderr, "ERROR: struct fork* table[%i] alloc failed", (int)i);
            return;

        }

        table[i]->user_id = 0xff;

        omp_init_lock(&table[i]->fork_lock);

    }

    #pragma omp parallel num_threads(num_philosophes) shared(table)
    {

        uint8_t id = omp_get_thread_num(), forks_owned = 0;

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

            if(id % 2 == 0){

                if(omp_test_lock(&table[id]->fork_lock)){

                    table[id]->user_id = id;
                    forks_owned++;

                    if(omp_test_lock(&table[(id+1) % num_philosophes]->fork_lock)){

                        table[(id+1) % num_philosophes]->user_id = id;
                        forks_owned++;

                    } else{

                        table[id]->user_id = 0xff;
                        forks_owned--;

                        omp_unset_lock(&table[id]->fork_lock);

                    }

                }

            }else {

                if(omp_test_lock(&table[(id+1) % num_philosophes]->fork_lock)){

                    table[(id+1) % num_philosophes]->user_id = id;
                    forks_owned++;

                    if(omp_test_lock(&table[id]->fork_lock)){

                        table[id]->user_id = id;
                        forks_owned++;

                    } else{

                        table[(id+1) % num_philosophes]->user_id = 0xff;
                        forks_owned--;

                        omp_unset_lock(&table[(id+1) % num_philosophes]->fork_lock);

                    }

                }

            }

            if(forks_owned == 2){

                #pragma omp critical(print)
                    printf("Philosopher %d now eats\n", id);

                sleep(rand_r(&seed) % 5 + 1);

                table[id]->user_id = 0xff;
                omp_unset_lock(&table[id]->fork_lock);
                table[(id+1) % num_philosophes]->user_id = 0xff;
                omp_unset_lock(&table[(id+1) % num_philosophes]->fork_lock);

                forks_owned = 0;

            } else{

                #pragma omp critical(print)
                    printf("Philosopher %d now things\n", id);

            }

            usleep(100000);
            
        }

    }

    for(uint8_t i = 0; i < num_philosophes; i++){
        
        omp_destroy_lock(&table[i]->fork_lock);
        free(table[i]);

    }
    free(table);

}
