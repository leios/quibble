/*------------quibble_buffers.c-----------------------------------------------//

Purpose: This file defines quibble buffers, which are CPU / GPU array pairs
         meant for the storage of variables so users can avoid recompilation

  Notes: Improvements:
         1. Make the buffer always UInt and then carry the types around
            so you can recast the fis in the kernel to the appropriate type.
            Right now, we are just defaulting to floats
         2. Think about textures!

//----------------------------------------------------------------------------*/

#include "../include/quibble_buffers.h"

struct quibble_buffer create_quibble_buffer(cl_context context, int n){
    struct quibble_buffer qb;
    qb.n = n;

    float *cpu_array = (float*)malloc(sizeof(float)*n);
    for (int i = 0; i < n; ++i){
        cpu_array[i] = 0;
    }

    qb.cpu = cpu_array;

    cl_int err;
    cl_mem gpu_array = clCreateBuffer(context,
                                      CL_MEM_READ_WRITE,
                                      n * sizeof(float),
                                      NULL,
                                      &err);
    cl_check(err);
}

void free_quibble_buffer(struct quibble_buffer qb){
    free(qb.cpu);
    cl_check(clReleaseMemObject(qb.gpu));
}

/*
int main(){

    n = 10;
    quibble_buffer qb = create_quibble_buffer(n);
    free_quibble_buffer(qb);
    return 0;
}
*/
