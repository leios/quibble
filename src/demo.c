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

    // Creating Kernel String
    char *kernel_source = (char*)malloc(MAX_SOURCE_SIZE);
    define_preamble(kernel_source);
    define_functions(kernel_source, 100);
    define_postamble(kernel_source);

    size_t kernel_size = strlen(kernel_source);

    // Array creation
    int array_size = 10;
    float *a = (float*)malloc(sizeof(float)*array_size);

    for (size_t i = 0; i < array_size; ++i){
        a[i] = 0;
    }

    cl_int err;
    struct quibble_canvas qc = create_canvas(kernel_source, 1, 0, 1);

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

    cl_check(
        clSetKernelArg(qc.kernel, 0, sizeof(cl_mem), (void *)&d_a)
    );
    cl_check(
        clSetKernelArg(qc.kernel, 1, sizeof(int), &array_size)
    );

    size_t global_item_size = array_size;
    size_t local_item_size = array_size;

    qb_run(qc, global_item_size, local_item_size);

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
