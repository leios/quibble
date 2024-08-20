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
#include "../include/quibble_verses.h"

int main(){

    /*------------------------------------------------------------------------//
        QUICK TESTS
    //------------------------------------------------------------------------*/
    char *kernel_string = DCOMPILE(
        __verse check(){
        }
    );

    // qb_is_dcompiled
    if (qb_is_dcompiled(kernel_string)){
        printf("Passed: qb_is_dcompiled\n");
    }
    else{
        printf("Failed: qb_is_dcompiled\n");
    }

    // qb_preprocess_verse
    qb_preprocess_verse(kernel_string);
    if (strcmp(kernel_string,
               "//\nDCOMPILE\nGENERATED\n__verse\ncheck(){\n}") == 0){
        printf("Passed: qb_preprocess_verse\n");
    }
    else{
        printf("Failed: qb_preprocess_verse\n");
    }

    // qb_replace_char_if_proceeding
    char check[21] = "This is a test string";
    qb_replace_char_if_proceeding(check, 100, "test", 4, ' ', '\n');
    qb_replace_char_if_proceeding(check, 100, "s", 1, ' ', '_');
    if (strcmp(check, "This_is_a test\nstring") == 0){
        printf("Passed: qb_replace_char_if_proceeding\n");
    }

    // qb_find_next_char
    if (qb_find_next_char(check, 21, 0, 't') == 10 &&
        qb_find_next_char(check, 21, 0, 'z') == -1 &&
        qb_find_next_char(check, 21, 11, 't') == 13){
        printf("Passed: qb_find_next_char\n");
    }
    else{
        printf("Failed: qb_find_next_char\n");
    }

    // qb_find_next_string
    if (qb_find_next_string(check, 21, 0, "t", 1) == 10 &&
        qb_find_next_string(check, 21, 0, "test", 4) == 10 &&
        qb_find_next_string(check, 21, 0, "z", 1) == -1 &&
        qb_find_next_string(check, 21, 11, "test", 4) == -1 &&
        qb_find_next_string(check, 21, 11, "string", 15) == -1){
        printf("Passed: qb_find_next_string\n");
    }
    else{
        printf("Failed: qb_find_next_string\n");
    }

    // qb_find_matching_char
    char check_2[10] = "([]()(()))";
        // qb_find_next_string
    if (qb_find_matching_char(check_2, 10, 0, '(', ')') == 9 &&
        qb_find_matching_char(check_2, 10, 1, '[', ']') == 2 &&
        qb_find_matching_char(check_2, 10, 5, '(', ')') == 8 &&
        qb_find_matching_char(check_2, 10, 7, '(', ')') == -1 &&
        qb_find_matching_char("(()((", 5, 0, '(', ')') == -1){
        printf("Passed: qb_find_matching_char\n");
    }
    else{
        printf("Failed: qb_find_matching_char\n");
        printf("%d\n", qb_find_matching_char(check_2, 10, 0, '(', ')'));
    }
 
    /*------------------------------------------------------------------------//
        DEMO
    //------------------------------------------------------------------------*/

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
    quibble_canvas qc = create_canvas(kernel_source, 1, 0, 1);

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
