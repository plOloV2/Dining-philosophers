#ifndef FIFO_LIB_H
#define FIFO_LIB_H

#include"libs.h"
#include"FIFO.h"

struct FIFO_queue* FIFO_create();
uint8_t FIFO_is_empty(struct FIFO_queue* queue);
uint8_t FIFO_peek(struct FIFO_queue* queue, uint8_t node_num);
void FIFO_add_node(struct FIFO_queue* queue, uint8_t data);
void FIFO_remove_node(struct FIFO_queue* queue);
void FIFO_destroy(struct FIFO_queue* queue);
uint8_t FIFO_get_size(struct FIFO_queue* queue);

#endif
