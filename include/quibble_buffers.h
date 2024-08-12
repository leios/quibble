#ifndef QUIBBLE_BUFFERS_H
#define QUIBBLE_BUFFERS_H

#include <stdio.h>
#include "../include/errors.h"

struct quibble_buffer{
    size_t n;
    float *cpu;
    cl_mem *canvas;
};

struct quibble_buffer qb_create_buffer(cl_context context, int n);
void qb_free_buffer(struct quibble_buffer qb);

#endif
