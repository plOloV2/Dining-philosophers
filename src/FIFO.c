#include"libs.h"
#include"FIFO.h"

struct FIFO_queue* FIFO_create(){

    struct FIFO_queue* q = calloc(1, sizeof(struct FIFO_queue));
    if(!q){

        fprintf(stderr, "ERROR: FIFO_create() alloc fail\n");
        return NULL;

    }

    return q;

}

uint8_t FIFO_is_empty(struct FIFO_queue* queue){

    return queue->first == NULL;

}

uint8_t FIFO_peek(struct FIFO_queue* queue, uint8_t node_num){

    if(FIFO_is_empty(queue)){

        fprintf(stderr, "ERROR: FIFO_peek() error, queue empty\n");
        return 0xff;

    }

    struct node* temp = queue->first;

    for(uint8_t i = 0; i < node_num; i++){

        if(!temp->next){

            fprintf(stderr, "ERROR: FIFO_peek() error, queue smaller than expected\n");
            return 0xff;

        }

        temp = temp->next;
    }

    return temp->data;

}

void FIFO_add_node(struct FIFO_queue* queue, uint8_t data){

    struct node* n = malloc(sizeof(struct node));
    if(!n){

        fprintf(stderr, "ERROR: FIFO_add_node() alloc fail\n");
        return;

    }

    n->data = data;
    n->next = NULL;

    if(FIFO_is_empty(queue)){

        queue->first = n;
        queue->last = n;

    } else{

        queue->last->next = n;
        queue->last = n;

    }

}

void FIFO_remove_node(struct FIFO_queue* queue){

    if(FIFO_is_empty(queue)){

        fprintf(stderr, "ERROR: Cannot delete from empty queue\n");
        return;

    } else{

        struct node* temp = queue->first;
        queue->first = temp->next;

        if(queue->first == NULL)
            queue->last = NULL;

        free(temp);

    }

}

void FIFO_destroy(struct FIFO_queue* queue){

    while(!FIFO_is_empty(queue))
        FIFO_remove_node(queue);

    free(queue);
    queue = NULL;

}

uint8_t FIFO_get_size(struct FIFO_queue* queue){

    return queue->size;

}
