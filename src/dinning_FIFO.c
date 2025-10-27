#include"libs.h"
#include"table.h"
#include"FIFO_lib.h"

void let_them_eat_FIFO(uint8_t num_philosophes){

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

    }

    struct FIFO_queue* queue = FIFO_create();
    if(!queue)
        return;  

    #pragma omp parallel num_threads(num_philosophes) shared(table, queue)
    {

        uint8_t id = omp_get_thread_num(), forks_owned = 0;

        unsigned int seed;

        #pragma omp critical
        {
            FILE* f = fopen("/dev/urandom", "rb");
            if(f && fread(&seed, sizeof(seed), 1, f) == 1){

                fclose(f);

            } else{

                if(f)
                    fclose(f);
                
                seed = (unsigned int)time(NULL);

            }

        }

        while(1){

            if(!FIFO_is_empty(queue)){

                uint8_t my_pos, left_pos, right_pos, queue_size;
                my_pos = 0xff;
                left_pos = 0xff; 
                right_pos = 0xff;

                #pragma omp critical(FIFO)
                    queue_size = FIFO_get_size(queue);

                for(int8_t i = 0; i < queue_size; i++){

                    uint8_t queue_data = 0xff;

                    #pragma omp critical(FIFO)
                        queue_data = FIFO_peek(queue, i);

                    if(queue_data == id){

                        my_pos = i;

                    } else if(queue_data == (id - 1) % num_philosophes){

                        left_pos = i;

                    } else if(queue_data == (id + 1) % num_philosophes){

                        right_pos = i;

                    }

                }

                #pragma omp critical(table)
                {
                    if(my_pos < left_pos && my_pos < right_pos && table[id]->user_id == 0xff && table[(id+1) % num_philosophes]->user_id == 0xff){

                        table[id]->user_id = id;
                        table[(id+1) % num_philosophes]->user_id = id;
                        forks_owned += 2;

                    }
                }

                if(my_pos == 0xff){

                    #pragma omp critical(FIFO)
                        FIFO_add_node(queue, id);

                }

            }

            if(forks_owned == 2){

                #pragma omp critical(print)
                    printf("Philosopher %d now eats\n", id);

                sleep(rand_r(&seed) % 5 + 1);

                #pragma omp critical(table)
                {

                    table[id]->user_id = 0xff;
                    table[(id+1) % num_philosophes]->user_id = 0xff;

                }


                forks_owned = 0;

            } else{

                #pragma omp critical(print)
                    printf("Philosopher %d now things\n", id);

            }

            usleep(100000);     //sleep for 100ms
            
        }

    }

    for(uint8_t i = 0; i < num_philosophes; i++){

        free(table[i]);

    }
    free(table);

}
