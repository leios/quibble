#ifndef QUIBBLE_CONTEXT_H
#define QUIBBLE_CONTEXT_H

#include "errors.h"
#include "macros.h"
#include <CL/cl.h>

#include "quibble_buffers.h"
#include <stdbool.h>

typedef struct {
    // Quibble stuff
    quibble_buffer buffer;
    int res_x;
    int res_y;
    unsigned int **framebuffers;
    

    // CL stuff
    cl_platform_id *platform_ids;
    cl_device_id *device_ids;
    cl_uint num_devices;
    cl_uint num_platforms;
    int chosen_platform;
    int chosen_device;
    cl_context context;
    cl_command_queue command_queue;
    cl_program program;
    cl_kernel kernel;
    size_t global_item_size;
    size_t local_item_size;
    int stage;
} quibble_canvas;

char *get_device_name(cl_device_id device_id);
char *get_platform_name(cl_platform_id platform_id);
quibble_canvas create_blank_canvas();
void qb_find_platforms(quibble_canvas *qc, bool verbose);
void qb_list_devices();

// I'll need to add `char *kernel` and `res_x`, `res_y`
quibble_canvas create_canvas(char * kernel,
                             int platform,
                             int device,
                             bool verbose);
quibble_canvas create_default_canvas(char *kernel, bool verbose);
void free_quibble_canvas(quibble_canvas qc);

void qb_run(quibble_canvas qc,
            size_t global_item_size,
            size_t local_item_size);

void qb_copy_buffer_to_canvas(quibble_canvas qc, quibble_buffer qb);
void qb_copy_buffer_from_canvas(quibble_canvas qc, quibble_buffer qb);

#endif
