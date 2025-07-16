#ifndef ERROR_H
#define ERROR_H

#include <string.h>
#include <stdio.h>

#include "macros.h"
#include <CL/cl.h>

#include <stdio.h>
#include <stdlib.h>

// QBT -> Quibble Text
#define QBT_RED     "\x1b[31m"
#define QBT_GREEN   "\x1b[32m"
#define QBT_YELLOW  "\x1b[33m"
#define QBT_BLUE    "\x1b[34m"
#define QBT_MAGENTA "\x1b[35m"
#define QBT_CYAN    "\x1b[36m"
#define QBT_RESET   "\x1b[0m"

void qb_check(bool value, char *err_string);
void cl_check(cl_int error);

const char *get_cl_error_string(cl_int error);
void cl_check_program(cl_int error, cl_program program, cl_device_id device_id);

#endif
