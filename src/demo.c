/*-------------demo.c---------------------------------------------------------//

Purpose: This file is a quick example of how we might read function fragments in
         to construct an OpenCL kernel with Quibble.

//----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/macros.h"
#include <CL/cl.h>

#include "../include/demo_functions.h"
#include "../include/errors.h"

int main(){
    int array_size = 10;
    float *a = (float*)malloc(sizeof(float)*array_size);

    for (size_t i = 0; i < array_size; ++i){
        a[i] = 0;
    }

/*
    char filename[50] = "demo.cl";
    FILE *kernel_file = fopen(filename, "r");

    if (!kernel_file){
        fprintf(stderr, "No file %s found!\n", filename);
        exit(-1);
    }

    char *kernel_source = (char*)malloc(MAX_SOURCE_SIZE);
    size_t kernel_size = fread(kernel_source, 1, MAX_SOURCE_SIZE, kernel_file);
    fclose(kernel_file);
*/

    // Creating Kernel String
    char *kernel_source = (char*)malloc(MAX_SOURCE_SIZE);
    define_preamble(kernel_source);
    define_functions(kernel_source, 100);
    define_postamble(kernel_source);

    size_t kernel_size = strlen(kernel_source);
    //printf("%s \n %d\n", kernel_source, kernel_size);


    // OpenCL specific device info
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_uint num_devices;
    cl_uint num_platforms;

    // For error propagation
    cl_int err;
    cl_check(
        clGetPlatformIDs(1, &platform_id, &num_platforms)
    );
    cl_check(
        clGetDeviceIDs(platform_id,
                       CL_DEVICE_TYPE_DEFAULT,
                       1,
                       &device_id,
                       &num_devices)
    );

    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);
    cl_check(err);

    cl_command_queue command_queue = clCreateCommandQueueWithProperties(
        context,
        device_id,
        0,
        &err
    );
    cl_check(err);

    // creating d_a and copying to GPU
    cl_mem d_a = clCreateBuffer(context,
                                CL_MEM_READ_WRITE,
                                array_size * sizeof(float),
                                NULL,
                                &err);
    cl_check(err);

    cl_check(
        clEnqueueWriteBuffer(command_queue,
                             d_a,
                             CL_TRUE,
                             0,
                             array_size * sizeof(float),
                             a,
                             0,
                             NULL,
                             NULL)
    );

    // Create program
    cl_program program = clCreateProgramWithSource(context,
                                                   1,
                                                   (const char**)&kernel_source,
                                                   (const size_t *)&kernel_size,
                                                   &err);
    cl_check(err);

    err = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    cl_check_program(err, program, device_id);

    cl_kernel kernel = clCreateKernel(program, "demo", &err);
    cl_check(err);

    cl_check(
        clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&d_a)
    );
    cl_check(
        clSetKernelArg(kernel, 1, sizeof(int), &array_size)
    );

    size_t global_item_size = array_size;
    size_t local_item_size = array_size;

    cl_check(
        clEnqueueNDRangeKernel(command_queue,
                               kernel,
                               1,
                               NULL,
                               &global_item_size,
                               &local_item_size,
                               0,
                               NULL,
                               NULL)
    );

    cl_check(
        clEnqueueReadBuffer(command_queue,
                            d_a,
                            CL_TRUE,
                            0,
                            array_size * sizeof(float),
                            a,
                            0,
                            NULL,
                            NULL)
    );

    for (int i = 0; i < array_size; ++i){
        printf("%f\n", a[i]);
    }

    cl_check(clFlush(command_queue));
    cl_check(clFinish(command_queue));
    cl_check(clReleaseCommandQueue(command_queue));
    cl_check(clReleaseKernel(kernel));
    cl_check(clReleaseProgram(program));
    cl_check(clReleaseMemObject(d_a));
    cl_check(clReleaseContext(context));

    free(a);

    return 0;
}
