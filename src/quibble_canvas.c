/*-------------quibble_canvas.c-----------------------------------------------//

Purpose: This file defines the quibble_canvas and associate functions.
         A quibble_canvas holds all the necessary information for quibble
         and OpenCL

  Notes: There are a few random numbers and NULLs in the OpenCL API that I 
             don't fully understand at this point
         The Quibble Context is initialized in stages:
             0: Nothing
             1: platforms found
             2: device selected
             3: CL context created
             4: CL command_queue generated
             5: CL program created
             6: CL Program built
             7: CL Kernel Created
             8: CL Args set
         There is also a separate launch function for Quibble Contexts
         ALL != DEFAULT Listing

  Plans: It might be nice to create different functions to more easily create
             quibble canvases, like `create_gpu_canvas` or `create_cpu_canvas`.
             This could be done by:
             1. Defining the appropriate functions and splitting the `create`
                function into an initialization phase and then kernel creation.
                These functions would go through all platforms and devices and
                then find the first default CPU or GPU, depending on the call
             2. Defining a set of properties like CL's CL_DEFAULT, CL_GPU, which
                can be used to specify the type of device wanted.

//----------------------------------------------------------------------------*/

#include "../include/quibble_canvas.h"

/*----------------------------------------------------------------------------//
    HEADER
//----------------------------------------------------------------------------*/

// I'll need to add `char *kernel` and `res_x`, `res_y`
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

struct quibble_canvas create_blank_canvas(){

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
        printf("Stage 1:\n");
        printf("    %d platforms found:\n", qc->num_platforms);
        for (int i = 0; i < qc->num_platforms; ++i){
            char *platform_name = get_platform_name(qc->platform_ids[i]);
            printf("        Platform %d: %s\n", i, platform_name);
            free(platform_name);
        }
        printf("    Choosing platform %d\n", qc->chosen_platform);
        printf("Stage 1 complete!\n");
        printf("\n");
    }
}

void qb_find_devices(struct quibble_canvas *qc, bool verbose){

    // As in `qb_find_platforms` and `qb_list devices`, 10 is arbitrary
    cl_check(
        clGetDeviceIDs(qc->platform_ids[qc->chosen_platform],
                       CL_DEVICE_TYPE_ALL,
                       0,
                       NULL,
                       &qc->num_devices)
    );

    qc->device_ids = (cl_device_id *)malloc(qc->num_devices *
                                            sizeof(cl_device_id));
    cl_check(
        clGetDeviceIDs(qc->platform_ids[qc->chosen_platform],
                       CL_DEVICE_TYPE_ALL,
                       qc->num_devices,
                       qc->device_ids,
                       &qc->num_devices)
    );

    if (verbose){
        char *platform_name =
            get_platform_name(qc->platform_ids[qc->chosen_platform]);
        printf("Stage 2:\n");
        printf("    %d devices found on platform %s:\n",
               qc->num_devices,
               platform_name);
        for (int j = 0; j < qc->num_devices; ++j){
            char *device_name = get_device_name(qc->device_ids[j]);
            printf("        device %d: %s\n", j, device_name);
            free(device_name);
        }

        printf("    Choosing device %d\n", qc->chosen_device);

        printf("Stage 2 complete!\n");
        printf("\n");
        free(platform_name);
    }

}

void qb_list_devices(){
    struct quibble_canvas qc = create_blank_canvas();
    qb_find_platforms(&qc, 0);

    cl_uint num_devices;
    for (int i = 0; i < qc.num_platforms; ++i){
        char *platform_name = get_platform_name(qc.platform_ids[i]);

        // As in `qb_find_platforms` and `qb_list devices`, 10 is arbitrary
        cl_check(
            clGetDeviceIDs(qc.platform_ids[i],
                           CL_DEVICE_TYPE_ALL,
                           0,
                           NULL,
                           &num_devices)
        );

        cl_device_id *device_ids = (cl_device_id *)malloc(num_devices *
             sizeof(cl_device_id));
        cl_check(
            clGetDeviceIDs(qc.platform_ids[i],
                           CL_DEVICE_TYPE_ALL,
                           num_devices,
                           device_ids,
                           &num_devices)
        );

        printf("Platform %d: %s. %d known device(s)...\n", i, platform_name,
               num_devices);

        for (int j = 0; j < num_devices; ++j){
            char *device_name = get_device_name(device_ids[j]);
            printf("\tDevice %d: %s\n", j, device_name);
            free(device_name);
        }
        free(platform_name);
        free(device_ids);
    }
}

struct quibble_canvas create_default_canvas(bool verbose){
    return create_canvas(0,0,verbose);
}
struct quibble_canvas create_canvas(int platform, int device, bool verbose){
    struct quibble_canvas qc = create_blank_canvas();

    qc.chosen_platform = platform;
    qc.chosen_device = device;
    qb_find_platforms(&qc, verbose);
    qb_find_devices(&qc, verbose);

    cl_int err;

    qc.context = clCreateContext(NULL,
                                 1,
                                 &qc.device_ids[qc.chosen_device],
                                 NULL,
                                 NULL,
                                 &err);
    cl_check(err);

    qc.command_queue = clCreateCommandQueueWithProperties(
        qc.context,
        qc.device_ids[qc.chosen_device],
        0,
        &err
    );
    cl_check(err);

    return qc;
}

void free_quibble_canvas(struct quibble_canvas qc){
    free(qc.platform_ids);
    free(qc.device_ids);
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
