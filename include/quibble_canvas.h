#ifndef QUIBBLE_CONTEXT_H
#define QUIBBLE_CONTEXT_H

#include "errors.h"
#include "macros.h"
#include <CL/cl.h>

#include "quibble_buffers.h"
#include <stdbool.h>

struct quibble_canvas{
    // Quibble stuff
    struct quibble_buffer input_buffer;
    int res_x;
    int res_y;
    unsigned int **framebuffers;
    

    // CL stuff
    cl_platform_id *platform_ids;
    cl_device_id *device_ids;
    cl_uint num_devices;
    cl_uint num_platforms;
    cl_context context;
    cl_command_queue command_queue;
    cl_program program;
    cl_kernel kernel;
    size_t global_item_size;
    size_t local_item_size;
    int stage;
};

char *get_device_name(cl_device_id device_id);
char *get_platform_name(cl_platform_id platform_id);
struct quibble_canvas create_simple_canvas();
void qb_find_platforms(struct quibble_canvas *qc, bool verbose);
void qb_list_devices();

struct quibble_canvas create_default_canvas(bool verbose);
void free_quibble_canvas(struct quibble_canvas qc);

/*
void list_devices(...);
void choose_device(...);
struct quibble_context create_context(int device, ...);
*/

#endif
