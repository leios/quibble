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

/*----------------------------------------------------------------------------//
    HEADER
//----------------------------------------------------------------------------*/

char *get_device_name(cl_device_id device_id);
char *get_platform_name(cl_platform_id platform_id);

struct quibble_canvas create_simple_canvas(){

    struct quibble_canvas qc;

    qc.stage = 0;

    return qc;
}

void qb_find_platforms(struct quibble_canvas *qc, bool verbose){
    // 10 is arbitrary, but should be high enough for almost any task
    cl_check(
        clGetPlatformIDs(10, NULL, &qc->num_platforms)
    );

    qc->platform_ids = (cl_platform_id *)malloc(qc->num_platforms *
                                                sizeof(cl_platform_id));
    cl_check(
        clGetPlatformIDs(qc->num_platforms, qc->platform_ids, NULL)
    );

    qc->stage = 1;

    if (verbose){
        printf("Stage 1 complete: %d platforms found:\n", qc->num_platforms);
        for (int i = 0; i < qc->num_platforms; ++i){
            char *platform_name = get_platform_name(qc->platform_ids[i]);
            printf("%d\t%s\n", i, platform_name);
            free(platform_name);
        }
        printf("\n");
    }
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

/*----------------------------------------------------------------------------//
    FX DEFINITIONS
//----------------------------------------------------------------------------*/

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

struct quibble_canvas create_default_canvas(bool verbose){
    struct quibble_canvas qc = create_simple_canvas();
    qb_find_platforms(&qc, verbose);
    qc.device_ids = (cl_device_id *)malloc(sizeof(cl_device_id));

    cl_int err;
    cl_check(
        clGetPlatformIDs(2, qc.platform_ids, &qc.num_platforms)
    );
    cl_check(
        clGetDeviceIDs(qc.platform_ids[1],
                       CL_DEVICE_TYPE_DEFAULT,
                       //CL_DEVICE_TYPE_GPU,
                       1,
                       &qc.device_ids[0],
                       &qc.num_devices)
    );

    char *platform_name = get_platform_name(qc.platform_ids[1]);
    printf("CL_PLATFORM_NAME: %s\n", platform_name);
    free(platform_name);
    char *dev_name = get_device_name(qc.device_ids[0]);
    printf("CL_DEVICE_NAME: %s\n", dev_name);
    free(dev_name);

    qc.context = clCreateContext(NULL, 1, &qc.device_ids[0], NULL, NULL, &err);
    cl_check(err);

    qc.command_queue = clCreateCommandQueueWithProperties(
        qc.context,
        qc.device_ids[0],
        0,
        &err
    );
    cl_check(err);

    return qc;
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
