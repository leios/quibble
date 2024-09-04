#ifndef QUIBBLE_INPUTS_H
#define QUIBBLE_INPUTS_H

#include "errors.h"
#include "macros.h"
#include <CL/cl.h>

typedef struct {
    size_t n;
    float *cpu;
    cl_mem *canvas;
} quibble_buffer;

typedef struct{
    quibble_buffer buffer;
    int index;
} quibble_variable;

typedef struct{
    quibble_buffer buffer;
    int start_index;
    int end_index;
} quibble_array;

quibble_variable qb_create_variable(quibble_buffer qb, int index, float value);

quibble_array qb_create_array(quibble_buffer qb,
                              int start_index,
                              int n,
                              float *a);

void qb_set_variable(quibble_variable qv, float value);
void qb_set_array(quibble_array qa, int n, float *a);

float qb_variable_value(quibble_variable qv);
void qb_inplace_array_value(float *a, int n, quibble_array qa);
float *qb_array_value(quibble_array qa);

char *qb_variable_to_string(quibble_variable qv);
char *qb_array_to_string(quibble_array qa);

char *qb_int_to_string(int i);
char *qb_float_to_string(float f);
char *qb_double_to_string(double d);

void qb_free_buffer(quibble_buffer qb);

#endif
