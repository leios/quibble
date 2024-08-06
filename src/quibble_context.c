/*-------------quibble_context.c----------------------------------------------//

Purpose: This file defines the quibble_context and associate functions.
         A quibble_context holds all the necessary information for quibble
         and OpenCL

  Notes: There are a few random numbers and NULLs in the OpenCL API that I 
         don't fully understand at this point

//----------------------------------------------------------------------------*/

#include "../include/quibble_context.h"

struct quibble_context create_default_context(){
    struct quibble_context qbc;
    qbc.platform_id = NULL;
    qbc.device_id = NULL;

    cl_int err;
    cl_check(
        clGetPlatformIDs(1, &qbc.platform_id, &qbc.num_platforms)
    );
    cl_check(
        clGetDeviceIDs(qbc.platform_id,
                       CL_DEVICE_TYPE_DEFAULT,
                       1,
                       &qbc.device_id,
                       &qbc.num_devices)
    );

    qbc.context = clCreateContext(NULL, 1, &qbc.device_id, NULL, NULL, &err);
    cl_check(err);

    qbc.command_queue = clCreateCommandQueueWithProperties(
        qbc.context,
        qbc.device_id,
        0,
        &err
    );
    cl_check(err);

    return qbc;
}

void free_quibble_context(struct quibble_context qc){
    cl_check(clFlush(qc.command_queue));
    cl_check(clFinish(qc.command_queue));
    cl_check(clReleaseCommandQueue(qc.command_queue));
    cl_check(clReleaseKernel(qc.kernel));
    cl_check(clReleaseProgram(qc.program));
    cl_check(clReleaseContext(qc.context));

}

void create_kernel(struct quibble_context *qc){
}

/*
void list_devices(...){
    something with clGetDeviceInfo
};
void choose_device(...);
struct quibble_context create_context(int device, ...);
*/
