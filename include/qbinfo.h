#ifndef QB_INFO_H
#define QB_INFO_H

#include "errors.h"
#include "macros.h"
#include <CL/cl.h>

char *get_device_name(cl_device_id device_id);
char *get_platform_name(cl_platform_id platform_id);
void qb_find_platforms(void);
void qb_find_devices(void);
void qb_list_devices(void);

#endif
