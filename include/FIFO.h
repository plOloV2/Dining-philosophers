#ifndef FIFO_H
#define FIFO_H

#include"libs.h"

struct node{

    uint8_t data;
    struct node* next;

};

struct FIFO_queue{

    struct node* first;
    struct node* last;
    uint8_t size;

};

#endif
