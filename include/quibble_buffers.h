#ifndef QUIBBLE_BUFFERS_H
#define QUIBBLE_BUFFERS_H

#include <stdio.h>
#include "../include/errors.h"

struct quibble_buffer{
    size_t n;
    float *cpu;
    cl_mem gpu;
};

struct quibble_buffer create_quibble_buffer(cl_context context, int n);
void free_quibble_buffer(struct quibble_buffer qb);

#endif
