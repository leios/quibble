#ifndef QUIBBLE_CONTEXT_H
#define QUIBBLE_CONTEXT_H

#include "errors.h"
#include "macros.h"
#include <CL/cl.h>

struct quibble_context{
    cl_platform_id platform_id;
    cl_device_id device_id;
    cl_uint num_devices;
    cl_uint num_platforms;
    cl_context context;
    cl_command_queue command_queue;
    cl_program program;
    cl_kernel kernel;
    size_t global_item_size;
    size_t local_item_size;
};

struct quibble_context create_default_context();
void free_quibble_context(struct quibble_context qc);
/*
void list_devices(...);
void choose_device(...);
struct quibble_context create_context(int device, ...);
*/

#endif
