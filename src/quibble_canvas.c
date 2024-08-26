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
             3: Ready to launch:
                CL context created
                CL command_queue generated
                CL program created
                CL Program built
                CL Kernel Created
                CL Args set
         There is also a separate launch function for Quibble Contexts
         "demo" is hardcoded as kernel name.
             remember that most of this will be determined by quibble, not
             by the user.

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

quibble_canvas create_blank_canvas(){

    quibble_canvas qc;

    qc.stage = 0;

    return qc;
}

void qb_find_platforms(quibble_canvas *qc, bool verbose){
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
        for (size_t i = 0; i < qc->num_platforms; ++i){
            char *platform_name = get_platform_name(qc->platform_ids[i]);
            printf("        Platform %lu: %s\n", i, platform_name);
            free(platform_name);
        }
        printf("    Choosing platform %d\n", qc->chosen_platform);
        printf("Stage 1 complete!\n");
        printf("\n");
    }
}

void qb_find_devices(quibble_canvas *qc, bool verbose){

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
        for (size_t j = 0; j < qc->num_devices; ++j){
            char *device_name = get_device_name(qc->device_ids[j]);
            printf("        device %lu: %s\n", j, device_name);
            free(device_name);
        }

        printf("    Choosing device %d\n", qc->chosen_device);

        printf("Stage 2 complete!\n");
        printf("\n");
        free(platform_name);
    }

}

void qb_list_devices(){
    quibble_canvas qc = create_blank_canvas();
    qb_find_platforms(&qc, 0);

    cl_uint num_devices;
    for (size_t i = 0; i < qc.num_platforms; ++i){
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

        printf("Platform %lu: %s. %d known device(s)...\n", i, platform_name,
               num_devices);

        for (size_t j = 0; j < num_devices; ++j){
            char *device_name = get_device_name(device_ids[j]);
            printf("\tDevice %lu: %s\n", j, device_name);
            free(device_name);
        }
        free(platform_name);
        free(device_ids);
    }
}

quibble_canvas create_default_canvas(char *kernel, bool verbose){
    return create_canvas(kernel, 0,0, verbose);
}

quibble_canvas create_canvas(char *kernel,
                                    int platform,
                                    int device,
                                    bool verbose){
    quibble_canvas qc = create_blank_canvas();

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

    // Create program
    size_t kernel_size = strlen(kernel);
    qc.program = clCreateProgramWithSource(qc.context,
                                           1,
                                           (const char**)&kernel,
                                           (const size_t *)&kernel_size,
                                           &err);
    cl_check(err);

    err = clBuildProgram(qc.program,
                         1,
                         &qc.device_ids[qc.chosen_device],
                         NULL,
                         NULL,
                         NULL);
    cl_check_program(err, qc.program, qc.device_ids[0]);

    qc.kernel = clCreateKernel(qc.program, "demo", &err);
    cl_check(err);

    qc.stage = 3;

    return qc;
}

void free_quibble_canvas(quibble_canvas qc){
    free(qc.platform_ids);
    free(qc.device_ids);
    //qb_free_buffer(qc.buffer);
    cl_check(clFlush(qc.command_queue));
    cl_check(clFinish(qc.command_queue));
    cl_check(clReleaseCommandQueue(qc.command_queue));
    cl_check(clReleaseKernel(qc.kernel));
    cl_check(clReleaseProgram(qc.program));
    cl_check(clReleaseContext(qc.context));

}

void qb_run(quibble_canvas qc,
            size_t global_item_size,
            size_t local_item_size){

    cl_check(
        clEnqueueNDRangeKernel(qc.command_queue,
                               qc.kernel,
                               1,
                               NULL,
                               &global_item_size,
                               &local_item_size,
                               0,
                               NULL,
                               NULL)
    );

}

void qb_copy_buffer_to_canvas(quibble_canvas qc, quibble_buffer qb){
    cl_check(
        clEnqueueWriteBuffer(qc.command_queue,
                             *qb.canvas,
                             CL_TRUE,
                             0,
                             qb.n * sizeof(float),
                             qb.cpu,
                             0,
                             NULL,
                             NULL)
    );

}
void qb_copy_buffer_from_canvas(quibble_canvas qc, quibble_buffer qb){
    cl_check(
        clEnqueueReadBuffer(qc.command_queue,
                            *qb.canvas,
                            CL_TRUE,
                            0,
                            qb.n * sizeof(float),
                            qb.cpu,
                            0,
                            NULL,
                            NULL)
    );

}

