/*-------------quibble_inputs.c-----------------------------------------------//

Purpose: This file defines quibble inputs, which are variables stored in a 
         quibble buffer so users do not need to regularly recompile their 
         kernels

  Notes: Because the inputs are all stored in a quibble buffer, it should be 
         possible to also store textures in a similar way

//----------------------------------------------------------------------------*/

#include "../include/quibble_inputs.h"

quibble_variable qb_create_variable(quibble_buffer qb, int index, float value){
    quibble_variable qv;
    qv.index = index;
    qv.buffer = qb;
    qv.buffer.cpu[index] = value;

    return qv;
}

quibble_array qb_create_array(quibble_buffer qb,
                              int start_index,
                              int n,
                              float *a){
    quibble_array qa;
    qa.start_index = start_index;
    qa.end_index = start_index + n;
    qa.buffer = qb;
    for (int i = start_index; i < start_index + n; ++i){
        qa.buffer.cpu[i] = a[i-start_index];
    }

    return qa;
}

void qb_set_variable(quibble_variable qv, float value){
    qv.buffer.cpu[qv.index] = value;
}

void qb_set_array(quibble_array qa, int n, float *a){
    if (n != qa.end_index - qa.start_index){
        fprintf(stderr,
                "Quibble array is not the same size as the input array!\n    Quibble array size: %d\n    Input array size: %d",
                qa.end_index - qa.start_index,
                n);
        exit(1);
    }

    for (int i = qa.start_index; i < qa.end_index; ++i){
        qa.buffer.cpu[i] = a[i-qa.start_index];
    }

}

float qb_variable_value(quibble_variable qv){
    return qv.buffer.cpu[qv.index];
}

void qb_inplace_array_value(float *a, int n, quibble_array qa){
    if (n != qa.end_index - qa.start_index){
        fprintf(stderr,
                "Quibble array is not the same size as the output array!\n    Quibble array size: %d\n    Output array size: %d",
                qa.end_index - qa.start_index,
                n);
        exit(1);
    }

    for (int i = qa.start_index; i < qa.end_index; ++i){
        a[i-qa.start_index] = qa.buffer.cpu[i];
    }
}

float *qb_array_value(quibble_array qa){
    int size = qa.end_index - qa.start_index;
    float *a = (float *)malloc(size*sizeof(float));
    for (int i = qa.start_index; i < qa.end_index; ++i){
        a[i-qa.start_index] = qa.buffer.cpu[i];
    }
    return a;
}

char *qb_variable_to_string(quibble_variable qv){
    int len = snprintf(NULL, 0, "%d", qv.index);
    len += snprintf(NULL, 0, "fi_buffer[];")+1;
    char *result = (char *)malloc(sizeof(char)*len);
    snprintf(result, len, "fi_buffer[%d];", qv.index);
    return result;
}
char *qb_array_to_string(quibble_array qa){
    int len = snprintf(NULL, 0, "%d", qa.start_index);
    len += strlen("fi_buffer + ;");

    char *result = (char *)malloc(sizeof(char) * len);
    snprintf(result, len, "fi_buffer + %d", qa.start_index);
    return result;
}
