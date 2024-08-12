#ifndef QUIBBLE_CONTEXT_H
#define QUIBBLE_CONTEXT_H

#include "errors.h"
#include "macros.h"
#include <CL/cl.h>

#include "quibble_buffers.h"
#include <stdbool.h>

struct quibble_canvas{
    // Quibble stuff
    struct quibble_buffer buffer;
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
};

char *get_device_name(cl_device_id device_id);
char *get_platform_name(cl_platform_id platform_id);
struct quibble_canvas create_blank_canvas();
void qb_find_platforms(struct quibble_canvas *qc, bool verbose);
void qb_list_devices();

// I'll need to add `char *kernel` and `res_x`, `res_y`
struct quibble_canvas create_canvas(char * kernel,
                                    int platform,
                                    int device,
                                    bool verbose);
struct quibble_canvas create_default_canvas(char *kernel, bool verbose);
void free_quibble_canvas(struct quibble_canvas qc);

void qb_run(struct quibble_canvas qc,
            size_t global_item_size,
            size_t local_item_size);

void qb_copy_buffer_to_canvas(struct quibble_canvas qc,
                              struct quibble_buffer qb);
void qb_copy_buffer_from_canvas(struct quibble_canvas qc,
                                struct quibble_buffer qb);

#endif
