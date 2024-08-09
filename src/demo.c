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
#include "../include/quibble_canvas.h"

int main(){
    int array_size = 10;
    float *a = (float*)malloc(sizeof(float)*array_size);

    for (size_t i = 0; i < array_size; ++i){
        a[i] = 0;
    }

    // Creating Kernel String
    char *kernel_source = (char*)malloc(MAX_SOURCE_SIZE);
    define_preamble(kernel_source);
    define_functions(kernel_source, 100);
    define_postamble(kernel_source);

    size_t kernel_size = strlen(kernel_source);

    cl_int err;
    struct quibble_canvas qc = create_default_canvas();

    // creating d_a and copying to GPU
    cl_mem d_a = clCreateBuffer(qc.context,
                                CL_MEM_READ_WRITE,
                                array_size * sizeof(float),
                                NULL,
                                &err);
    cl_check(err);

    cl_check(
        clEnqueueWriteBuffer(qc.command_queue,
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
    qc.program = clCreateProgramWithSource(qc.context,
                                           1,
                                           (const char**)&kernel_source,
                                           (const size_t *)&kernel_size,
                                           &err);
    cl_check(err);

    err = clBuildProgram(qc.program, 1, &qc.device_ids[0], NULL, NULL, NULL);
    cl_check_program(err, qc.program, qc.device_ids[0]);

    qc.kernel = clCreateKernel(qc.program, "demo", &err);
    cl_check(err);

    cl_check(
        clSetKernelArg(qc.kernel, 0, sizeof(cl_mem), (void *)&d_a)
    );
    cl_check(
        clSetKernelArg(qc.kernel, 1, sizeof(int), &array_size)
    );

    size_t global_item_size = array_size;
    size_t local_item_size = array_size;

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

    cl_check(
        clEnqueueReadBuffer(qc.command_queue,
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

    free_quibble_canvas(qc);
    cl_check(clReleaseMemObject(d_a));

    free(a);

    return 0;
}
