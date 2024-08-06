/*-------------demo_functions.c-----------------------------------------------//

Purpose: This file defines a few functions to create an OpenCL kernel by 
         reading in a set of functions with the `define_functions` function.

  Notes: `define functions` should be made to read functions in at runtime:
    ```
    char filename[50] = "demo.cl";
    FILE *kernel_file = fopen(filename, "r");

    if (!kernel_file){
        fprintf(stderr, "No file %s found!\n", filename);
        exit(-1);
    }

    char *kernel_source = (char*)malloc(MAX_SOURCE_SIZE);
    size_t kernel_size = fread(kernel_source, 1, MAX_SOURCE_SIZE, kernel_file);
    fclose(kernel_file);
    ```

//----------------------------------------------------------------------------*/
#include "../include/macros.h"
#include "../include/demo_functions.h"

void define_add(char *final_fx){
    const char *kernel_str =                                              "\n" \
    "float add(float a, float b){                                          \n" \
    "    return a + b;                                                     \n" \
    "}                                                                     \n";
    strcat(final_fx, kernel_str);
}
float add(float a, float b){
    return a + b;
}

void define_preamble(char *final_fx){
    define_add(final_fx);
    const char *kernel_str =                                              "\n" \
    "__kernel void demo(__global float *a, int N){                         \n" \
    "    int idx = get_global_id(0);                                       \n" \
    "    int x = a[idx];                                                   \n";
    strcat(final_fx, kernel_str);
}

void define_postamble(char *final_fx){
    const char *kernel_str =                                              "\n" \
    "    a[idx] = x;                                                       \n" \
    "}                                                                     \n";
    strcat(final_fx, kernel_str);
}

void define_functions(char *final_fx, int n){
    char *all_fxs = (char*)malloc(MAX_SOURCE_SIZE);

    char f_1[50] = "    x = add(x, 1);\n";
    char f_2[50] = "    x += 2;\n";
    char f_3[50] = "    x += 3;\n";
    char f_4[50] = "    x += 4;\n";
    char f_5[50] = "    x += 5;\n";
    char f_6[50] = "    x += 6;\n";
    char f_7[50] = "    x += 7;\n";
    char f_8[50] = "    x += 8;\n";
    char f_9[50] = "    x += 9;\n";
    char f_10[50] = "    x += 10;\n";
    char f_11[50] = "    x += 11;\n";
    char f_12[50] = "    x += 12;\n";
    char f_13[50] = "    x += 13;\n";
    char f_14[50] = "    x += 14;\n";
    char f_15[50] = "    x += 15;\n";
    char f_16[50] = "    x += 16;\n";
    char f_17[50] = "    x += 17;\n";
    char f_18[50] = "    x += 18;\n";
    char f_19[50] = "    x += 19;\n";
    char f_20[50] = "    x += 20;\n";

    for (int i = 0; i < n; ++i){
        strcat(all_fxs, f_1);
        strcat(all_fxs, f_2);
        strcat(all_fxs, f_3);
        strcat(all_fxs, f_4);
        strcat(all_fxs, f_5);
        strcat(all_fxs, f_6);
        strcat(all_fxs, f_7);
        strcat(all_fxs, f_8);
        strcat(all_fxs, f_9);
        strcat(all_fxs, f_10);
        strcat(all_fxs, f_11);
        strcat(all_fxs, f_12);
        strcat(all_fxs, f_13);
        strcat(all_fxs, f_14);
        strcat(all_fxs, f_15);
        strcat(all_fxs, f_16);
        strcat(all_fxs, f_17);
        strcat(all_fxs, f_18);
        strcat(all_fxs, f_19);
        strcat(all_fxs, f_20);
    }

    strcat(final_fx, all_fxs);
}
