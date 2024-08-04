#ifndef ERROR_H
#define ERROR_H

#include <string.h>
#include <stdio.h>

#include "macros.h"
#include <CL/cl.h>

void cl_check(cl_int error);

const char *get_cl_error_string(cl_int error);
void cl_check_program(cl_int error, cl_program program, cl_device_id device_id);

#endif
