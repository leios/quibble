/*-------------qbinfo.c-------------------------------------------------------//

Purpose: qb info is just an easy way to create and set OpenCL variables

//----------------------------------------------------------------------------*/

#include "../include/qbinfo.h"

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

void qb_list_devices(void){

    cl_platform_id *platform_ids;
    cl_uint num_platforms;

    // Platforms
        clGetPlatformIDs(10, NULL, &num_platforms);
    cl_check(
        clGetPlatformIDs(10, NULL, &num_platforms)
    );

    platform_ids = (cl_platform_id *)malloc(num_platforms *
                                                sizeof(cl_platform_id));
    cl_check(
        clGetPlatformIDs(num_platforms, platform_ids, NULL)
    );

    cl_uint num_devices;
    for (size_t i = 0; i < num_platforms; ++i){
        char *platform_name = get_platform_name(platform_ids[i]);

        // As in `qb_find_platforms` and `qb_list devices`, 10 is arbitrary
        cl_check(
            clGetDeviceIDs(platform_ids[i],
                           CL_DEVICE_TYPE_ALL,
                           0,
                           NULL,
                           &num_devices)
        );

        cl_device_id *device_ids = (cl_device_id *)malloc(num_devices *
             sizeof(cl_device_id));
        cl_check(
            clGetDeviceIDs(platform_ids[i],
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
    free(platform_ids);
}

int main(void){
    qb_list_devices();

    return 0;
}

