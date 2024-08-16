#ifndef QUIBBLE_BUFFERS_H
#define QUIBBLE_BUFFERS_H

#include <stdio.h>
#include "../include/errors.h"

typedef struct {
    size_t n;
    float *cpu;
    cl_mem *canvas;
} quibble_buffer;

quibble_buffer qb_create_buffer(cl_context context, int n);
void qb_free_buffer(quibble_buffer qb);

#endif
