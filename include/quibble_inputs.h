#ifndef QUIBBLE_INPUTS_H
#define QUIBBLE_INPUTS_H

#include "quibble_buffers.h"

struct quibble_variable{
    struct quibble_buffer buffer;
    int index;
};

struct quibble_array{
    struct quibble_buffer buffer;
    int start_index;
    int end_index;
};

struct quibble_variable qb_create_variable(struct quibble_buffer qb,
                                           int index,
                                           float value);

struct quibble_array qb_create_array(struct quibble_buffer qb,
                                     int start_index,
                                     int n,
                                     float *a);

void qb_set_variable(struct quibble_variable qv, float value);
void qb_set_array(struct quibble_array qa, int n, float *a);

float qb_variable_value(struct quibble_variable qv);
void qb_inplace_array_value(float *a, int n, struct quibble_array qa);
float *qb_array_value(struct quibble_array qa);

#endif
