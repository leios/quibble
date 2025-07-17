/*-------------config---------------------------------------------------------//

 Purpose: config and run quibble programs

//----------------------------------------------------------------------------*/

#include "../include/config.h"

/*----------------------------------------------------------------------------//
CONFIGURATION
//----------------------------------------------------------------------------*/

void qb_rebuild_program(quibble_program *qp){
    free(qp->body);
    qb_build_program(qp);
}

void qb_build_program(quibble_program *qp){
    char *body = (char *)calloc(MAX_SOURCE_SIZE, sizeof(char));
    if (qp->everything_else != NULL){
        strcat(body, qp->everything_else);
        strcat(body, "\n\n");
    }

    for (int i = 0; i < qp->num_poems; ++i){
        char *temp_poem = qb_expand_poem(qp[0], i);
        if (temp_poem != NULL){
            strcat(body, temp_poem);
            strcat(body, "\n");
        }
        free(temp_poem);
    }

    int len = strlen(body)+1;
    char *final_body = (char *)calloc(len, sizeof(char));
    for (int i = 0; i < len; ++i){
        final_body[i] = body[i];
    }
    qp->body = final_body;
    free(body);
}

/*----------------------------------------------------------------------------//
    OpenCL Interface
//----------------------------------------------------------------------------*/

bool qb_find_keyword(quibble_program qp, char *keyword){

    if(qb_find_keyword_in_poems(qp, keyword)){
        return true;
    }
    if(qb_find_keyword_in_stanzas(qp, keyword)){
        return true;
    }
    if(qb_find_keyword_in_verses(qp, keyword)){
        return true;
    }

    return false;
}

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

void print_cl_info(quibble_program qp){
    char *platform_name =
        get_platform_name(qp.platform_ids[qp.chosen_platform]);
    char *device_name = get_device_name(qp.device_ids[qp.chosen_device]);

    printf("Platform: %s\nDevice: %s\n\n", platform_name, device_name);
    free(platform_name);
    free(device_name);
}

void qb_find_platforms(quibble_program *qp){
    // 10 is arbitrary, but should be high enough for almost any task
    // This finds &qp->num_platforms
    cl_check(
        clGetPlatformIDs(10, NULL, &qp->num_platforms)
    );

    qp->platform_ids = (cl_platform_id *)malloc(qp->num_platforms *
                                                sizeof(cl_platform_id));

    // This finds qp->platform_ids,
    cl_check(
        clGetPlatformIDs(qp->num_platforms, qp->platform_ids, NULL)
    );
}

void qb_find_devices(quibble_program *qp){

    // finds &qp->num_devices
    cl_check(
        clGetDeviceIDs(qp->platform_ids[qp->chosen_platform],
                       CL_DEVICE_TYPE_ALL,
                       0,
                       NULL,
                       &qp->num_devices)
    );

    qp->device_ids = (cl_device_id *)malloc(qp->num_devices *
                                            sizeof(cl_device_id));

    // finds &qp->num_devices
    cl_check(
        clGetDeviceIDs(qp->platform_ids[qp->chosen_platform],
                       CL_DEVICE_TYPE_ALL,
                       qp->num_devices,
                       qp->device_ids,
                       &qp->num_devices)
    );

}

void qb_configure_program(quibble_program *qp, int platform, int device){
    qp->chosen_platform = platform;
    qp->chosen_device = device;

    qb_find_platforms(qp);
    qb_find_devices(qp);

    cl_int err;

    qp->context = clCreateContext(NULL,
                                  1,
                                  &qp->device_ids[qp->chosen_device],
                                  NULL,
                                  NULL,
                                  &err);
    cl_check(err);

    qp->command_queue = clCreateCommandQueueWithProperties(
         qp->context,
         qp->device_ids[qp->chosen_device],
         0,
         &err
    );
    cl_check(err);

    // Create program
    size_t kernel_size = strlen(qp->body);
    qp->program = clCreateProgramWithSource(
        qp->context,
        1,
        (const char**)&qp->body,
        (const size_t *)&kernel_size,
        &err
    );
    cl_check(err);

    err = clBuildProgram(qp->program,
                         1,
                         &qp->device_ids[qp->chosen_device],
                         NULL,
                         NULL,
                         NULL);
    cl_check_program(err, qp->program,
                     qp->device_ids[qp->chosen_device]);

    cl_kernel *kernels = (cl_kernel *)malloc(sizeof(cl_kernel)*qp->num_poems);
    for (int i = 0; i < qp->num_poems; ++i){

        kernels[i] = clCreateKernel(qp->program, qp->poem_list[i].name, &err);
        cl_check(err);
    }

    qp->kernels = kernels;
    qp->configured = true;
}

void qb_run(quibble_program qp, char *kernel_name,
            size_t global_item_size,
            size_t local_item_size){

    int kernel_num = qb_find_poem_index(qp, kernel_name);

    cl_check(
        clEnqueueNDRangeKernel(qp.command_queue,
                               qp.kernels[kernel_num],
                               1,
                               NULL,
                               &global_item_size,
                               &local_item_size,
                               0,
                               NULL,
                               NULL)
    );

}

void qb_set_arg(quibble_program *qp, char *poem, char *arg, void *data){
    int poem_index = qb_find_poem_index(*qp, poem);

    char *type;
    char *variable;

    qb_find_type_arg(arg, &type, &variable);

    int arg_index = qb_find_arg_index(qp->poem_list[poem_index].args,
                                      qp->poem_list[poem_index].num_args,
                                      variable);

    size_t object_size = qb_find_type_size(type);

    cl_check(
        clSetKernelArg(qp->kernels[poem_index],
                       arg_index,
                       object_size,
                       (void *)data)
    );

    free(type);
    free(variable);
}

void qb_set_args_variadic(quibble_program *qp, char *poem, int n, va_list args){
    for (int i = 0; i < n; ++i){
        char *curr_arg_str = va_arg(args, char *);
        char *type;
        char *variable;
        qb_find_type_arg(curr_arg_str, &type, &variable);
        if (strcmp(type, "quibble_pixels") == 0){
            quibble_pixels curr_data = va_arg(args, quibble_pixels);
            qb_set_pixel_args(qp, poem, curr_data, variable);
        }
        else{
            void *curr_data = va_arg(args, void *);
            qb_set_arg(qp, poem, curr_arg_str, curr_data);
        }
        free(type);
        free(variable);
    }
}

void qb_set_args(quibble_program *qp, char *poem, int n, ...){
    va_list args;
    va_start(args, 2*n);

    qb_set_args_variadic(qp, poem, n, args);
 
    va_end(args);
}

char *qb_create_pixel_args(char *type, char *variable){
    char tmp[1000];

    sprintf(tmp, "__global %s *%s, %s_color_type, %s_width, %s_height,",
            type, variable, variable, variable, variable);

    int len = strlen(tmp);

    char *ret = (char *)calloc(len+1, sizeof(char));

    strcat(ret, tmp);

    return ret;
}

void qb_set_pixel_args(quibble_program *qp,
                       char *poem,
                       quibble_pixels qps,
                       char *variable){

    qb_set_arg(qp, poem, variable, qps.device_data);
    char curr_var[128];
    sprintf(curr_var, "%s_color_type", variable);
    qb_set_arg(qp, poem, curr_var, &qps.color_type);

    sprintf(curr_var, "%s_width", variable);
    qb_set_arg(qp, poem, curr_var, &qps.width);

    sprintf(curr_var, "%s_height", variable);
    qb_set_arg(qp, poem, curr_var, &qps.height);
}
