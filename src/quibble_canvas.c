/*-------------quibble_canvas.c-----------------------------------------------//

Purpose: This file defines the quibble_canvas and associate functions.
         A quibble_canvas holds all the necessary information for quibble
         and OpenCL

  Notes: There are a few random numbers and NULLs in the OpenCL API that I 
             don't fully understand at this point
         The Quibble Context is initialized in stages:
             0: Nothing
             1: platforms found
             2: devices found
             3: device selected
             4: CL context created
             5: CL command_queue generated
             6: CL program created
             7: CL Program built
             8: CL Kernel Created
             9: CL Args set
         There is also a separate launch function for Quibble Contexts

//----------------------------------------------------------------------------*/

#include "../include/quibble_canvas.h"

struct quibble_canvas create_simple_context(){

    struct quibble_canvas qc;

    qc.stage = 0;

    return qc;
}

void qb_find_platforms(struct quibble_canvas *qc){
}

void qb_find_devices(struct quibble_canvas *qc){
}

// This selects and then sets the device in the context
void qb_select_device(struct quibble_canvas *qc,
                      int platform_id,
                      int device_id){
}

void qb_create_command_queue(struct quibble_canvas *qc){
}

// This creates and builds the program
void qb_build_program(struct quibble_canvas *qc,
                      const char **kernel_source,
                      const size_t *kernel_size){
}

void qb_create_kernel(struct quibble_canvas *qc, char *kernel_name){
}

/*
void qb_set_kernel_args(struct quibble_canvas *qc, int arg_num, ...){
} 
*/

void qb_launch(struct quibble_canvas *qc){
}


char *get_device_name(cl_device_id device_id){
    size_t size;
    clGetDeviceInfo(device_id, CL_DEVICE_NAME, 0, NULL, &size);
    char *str = (char *)malloc(size);
    clGetDeviceInfo(device_id, CL_DEVICE_NAME, size, str, NULL);
    return str;
}

char *get_platform_name(cl_platform_id platform_id){
    size_t size;
    clGetPlatformInfo(platform_id, CL_PLATFORM_NAME, 0, NULL, &size);
    char *str = (char *)malloc(size);
    clGetPlatformInfo(platform_id, CL_PLATFORM_NAME, size, str, NULL);
    return str;
}

struct quibble_canvas create_default_canvas(){
    struct quibble_canvas qbc;
    qbc.platform_ids = (cl_platform_id *)malloc(2*sizeof(cl_platform_id));
    qbc.device_ids = (cl_device_id *)malloc(sizeof(cl_device_id));

    cl_int err;
    cl_check(
        clGetPlatformIDs(2, qbc.platform_ids, &qbc.num_platforms)
    );
    cl_check(
        clGetDeviceIDs(qbc.platform_ids[1],
                       CL_DEVICE_TYPE_DEFAULT,
                       //CL_DEVICE_TYPE_GPU,
                       1,
                       &qbc.device_ids[0],
                       &qbc.num_devices)
    );

    char *platform_name = get_platform_name(qbc.platform_ids[1]);
    printf("CL_PLATFORM_NAME: %s\n", platform_name);
    free(platform_name);
    char *dev_name = get_device_name(qbc.device_ids[0]);
    printf("CL_DEVICE_NAME: %s\n", dev_name);
    free(dev_name);

    qbc.context = clCreateContext(NULL, 1, &qbc.device_ids[0], NULL, NULL, &err);
    cl_check(err);

    qbc.command_queue = clCreateCommandQueueWithProperties(
        qbc.context,
        qbc.device_ids[0],
        0,
        &err
    );
    cl_check(err);

    return qbc;
}

void free_quibble_canvas(struct quibble_canvas qc){
    cl_check(clFlush(qc.command_queue));
    cl_check(clFinish(qc.command_queue));
    cl_check(clReleaseCommandQueue(qc.command_queue));
    cl_check(clReleaseKernel(qc.kernel));
    cl_check(clReleaseProgram(qc.program));
    cl_check(clReleaseContext(qc.context));

}

void create_kernel(struct quibble_canvas *qc){
}

/*
void list_devices(...){
    something with clGetDeviceInfo
};
void choose_device(...);
struct quibble_canvas create_context(int device, ...);
*/
