#ifndef TABLE_H
#define TABLE_H

#include"libs.h"

struct fork{

    uint8_t user_id;
    omp_lock_t fork_lock;

};

#endif
