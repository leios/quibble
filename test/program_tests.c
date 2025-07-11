#include "tests.h"

void quibble_arg_parsing_tests(void){

    printf("Arg / Kwarg Parsing Tests...\n");
    // qb_parse_kwargs
    char prologue_1[20] = " | float *x = 5;";
    char prologue_2[100] = "float *a , int b, double c | x = 5; y = 2*x+3;";
    char prologue_3[100] = "\t\n a, \t b, \n int c \n | \t\n int variable\t\n = \t\t\n whatever\n\n\t ;";
    char prologue_4[10] = "a,b,c";
    char prologue_5[10] = "meh| ";
    char prologue_6[10] = "a=6;";
    char prologue_7[10] = "    ";
    int num_kwargs_1 = qb_find_number_of_kwargs(prologue_1);
    int num_kwargs_2 = qb_find_number_of_kwargs(prologue_2);
    int num_kwargs_3 = qb_find_number_of_kwargs(prologue_3);
    int num_kwargs_4 = qb_find_number_of_kwargs(prologue_4);
    int num_kwargs_5 = qb_find_number_of_kwargs(prologue_5);
    int num_kwargs_6 = qb_find_number_of_kwargs(prologue_6);
    int num_kwargs_7 = qb_find_number_of_kwargs(prologue_7);

    if (num_kwargs_1 == 1 &&
        num_kwargs_2 == 2 &&
        num_kwargs_3 == 1 &&
        num_kwargs_4 == 0 &&
        num_kwargs_5 == 0 &&
        num_kwargs_6 == 1 &&
        num_kwargs_7 == 0 ){
        printf("\t"QBT_GREEN"Passed: "QBT_RESET"qb_find_number_of_kwargs\n");
    }
    else {
        printf("\t"QBT_RED"Failed: "QBT_RESET"qb_find_number_of_kwargs\n");
    }

    quibble_kwarg *check_kwargs_1 =
        qb_parse_kwargs(prologue_1, num_kwargs_1);

    quibble_kwarg *check_kwargs_2 =
        qb_parse_kwargs(prologue_2, num_kwargs_2);

    quibble_kwarg *check_kwargs_3 =
        qb_parse_kwargs(prologue_3, num_kwargs_3);

    quibble_kwarg *check_kwargs_4 =
        qb_parse_kwargs(prologue_4, num_kwargs_4);

    quibble_kwarg *check_kwargs_5 =
        qb_parse_kwargs(prologue_5, num_kwargs_5);

    quibble_kwarg *check_kwargs_6 =
        qb_parse_kwargs(prologue_6, num_kwargs_6);

    quibble_kwarg *check_kwargs_7 =
        qb_parse_kwargs(prologue_7, num_kwargs_7);

    if (strcmp(check_kwargs_1[0].variable, "x") == 0 &&
        strcmp(check_kwargs_1[0].type, "float *") == 0 &&
        strcmp(check_kwargs_1[0].value, "5") == 0 &&
        strcmp(check_kwargs_2[0].variable, "x") == 0 &&
        strcmp(check_kwargs_2[0].value, "5") == 0 &&
        strcmp(check_kwargs_2[1].variable, "y") == 0 &&
        strcmp(check_kwargs_2[1].value, "2*x+3") == 0 &&
        strcmp(check_kwargs_3[0].variable, "variable") == 0 &&
        strcmp(check_kwargs_3[0].value, "whatever") == 0 &&
        strcmp(check_kwargs_3[0].type, "int") == 0 &&
        check_kwargs_4 == NULL && check_kwargs_5 == NULL &&
        check_kwargs_7 == NULL &&
        strcmp(check_kwargs_6[0].variable, "a") == 0 &&
        strcmp(check_kwargs_6[0].value, "6") == 0 &&
        check_kwargs_6[0].type == NULL ){
        printf("\t"QBT_GREEN"Passed: "QBT_RESET"qb_parse_kwargs\n");
    }
    else {
        printf("\t"QBT_RED"Failed: "QBT_RESET"qb_parse_kwargs\n");
    }

    if (qb_find_kwarg_index(check_kwargs_2, num_kwargs_2, "x") == 0 &&
        qb_find_kwarg_index(check_kwargs_7, num_kwargs_7, "x") == -1 &&
        qb_find_kwarg_index(check_kwargs_3, num_kwargs_3, "variable") == 0 ){
        printf("\t"QBT_GREEN"Passed: "QBT_RESET"qb_find_kwarg_index\n");
    }
    else {
        printf("\t"QBT_RED"Failed: "QBT_RESET"qb_find_kwarg_index\n");
    }

    qb_free_kwarg_array(check_kwargs_1, num_kwargs_1);
    qb_free_kwarg_array(check_kwargs_2, num_kwargs_2);
    qb_free_kwarg_array(check_kwargs_3, num_kwargs_3);
    qb_free_kwarg_array(check_kwargs_4, num_kwargs_4);
    qb_free_kwarg_array(check_kwargs_5, num_kwargs_5);
    qb_free_kwarg_array(check_kwargs_6, num_kwargs_6);
    qb_free_kwarg_array(check_kwargs_7, num_kwargs_7);

    int num_args_1 = qb_find_number_of_args(prologue_1);
    int num_args_2 = qb_find_number_of_args(prologue_2);
    int num_args_3 = qb_find_number_of_args(prologue_3);
    int num_args_4 = qb_find_number_of_args(prologue_4);
    int num_args_5 = qb_find_number_of_args(prologue_5);
    int num_args_6 = qb_find_number_of_args(prologue_6);
    int num_args_7 = qb_find_number_of_args(prologue_6);

    if (num_args_1 == 0 &&
        num_args_2 == 3 &&
        num_args_3 == 3 &&
        num_args_4 == 3 &&
        num_args_5 == 1 &&
        num_args_6 == 0 &&
        num_args_7 == 0 ){
        printf("\t"QBT_GREEN"Passed: "QBT_RESET"qb_find_number_of_args\n");
    }
    else {
        printf("\t"QBT_RED"Failed: "QBT_RESET"qb_find_number_of_args\n");
    }

    quibble_arg *check_args_1 = qb_parse_args(prologue_1, num_args_1);
    quibble_arg *check_args_2 = qb_parse_args(prologue_2, num_args_2);
    quibble_arg *check_args_3 = qb_parse_args(prologue_3, num_args_3);
    quibble_arg *check_args_4 = qb_parse_args(prologue_4, num_args_4);
    quibble_arg *check_args_5 = qb_parse_args(prologue_5, num_args_5);
    quibble_arg *check_args_6 = qb_parse_args(prologue_6, num_args_6);
    quibble_arg *check_args_7 = qb_parse_args(prologue_7, num_args_7);

    if (check_args_1 == NULL &&
        strcmp(check_args_2[0].variable, "a") == 0 &&
        strcmp(check_args_2[0].type, "float *") == 0 &&
        strcmp(check_args_2[1].variable, "b") == 0 &&
        strcmp(check_args_2[1].type, "int") == 0 &&
        strcmp(check_args_2[2].variable, "c") == 0 &&
        strcmp(check_args_3[0].variable, "a") == 0 &&
        strcmp(check_args_3[1].variable, "b") == 0 &&
        strcmp(check_args_3[2].variable, "c") == 0 &&
        strcmp(check_args_3[2].type, "int") == 0 &&
        strcmp(check_args_4[0].variable, "a") == 0 &&
        strcmp(check_args_4[1].variable, "b") == 0 &&
        strcmp(check_args_4[2].variable, "c") == 0 &&
        strcmp(check_args_5[0].variable, "meh") == 0 &&
        check_args_6 == NULL && check_args_7 == NULL){
        printf("\t"QBT_GREEN"Passed: "QBT_RESET"qb_parse_args\n");
    }
    else {
        printf("\t"QBT_RED"Failed: "QBT_RESET"qb_parse_args\n");
    }

    if (qb_find_arg_index(check_args_5, num_args_5, "meh") == 0 &&
        qb_find_arg_index(check_args_7, num_args_7, "x") == -1 &&
        qb_find_arg_index(check_args_3, num_args_3, "b") == 1 ){
        printf("\t"QBT_GREEN"Passed: "QBT_RESET"qb_find_arg_index\n");
    }
    else {
        printf("\t"QBT_RED"Failed: "QBT_RESET"qb_find_arg_index\n");
    }

    qb_free_arg_array(check_args_1, num_args_1);
    qb_free_arg_array(check_args_2, num_args_2);
    qb_free_arg_array(check_args_3, num_args_3);
    qb_free_arg_array(check_args_4, num_args_4);
    qb_free_arg_array(check_args_5, num_args_5);
    qb_free_arg_array(check_args_6, num_args_6);
    qb_free_arg_array(check_args_7, num_args_7);
    
}

void quibble_program_tests(){
    printf("Program Parsing Tests...\n");

    char *simple_program = QBINLINE(
        __verse check(){}
        __stanza check(){}
        __poem check(){}
    );

    quibble_program qp_check = qb_parse_program(simple_program, "");

    if (qp_check.num_verses == 1 &&
        qp_check.everything_else == NULL &&
        strcmp(qp_check.verse_list[0].name, "check") == 0 &&
        qp_check.num_poems == 1 &&
        strcmp(qp_check.poem_list[0].name, "check") == 0 &&
        qp_check.num_stanzas == 1 &&
        strcmp(qp_check.stanza_list[0].name, "check") == 0 ){
        printf("\t"QBT_GREEN"Passed: "QBT_RESET"qb_parse_program\n");
    }
    else {
        printf("\t"QBT_RED"Failed: "QBT_RESET"qb_parse_program\n");
    }


    char *filename = qb_config_file("scribbles/example.qbl");
    quibble_program qp_2 = qb_parse_program_file(filename);
    free(filename);

    if (qp_2.num_verses == 4 &&
        strcmp(qp_2.verse_list[0].name, "nothing") == 0 &&
        qp_2.num_poems == 4 &&
        strcmp(qp_2.poem_list[3].name, "poem_check") == 0 &&
        qp_2.num_stanzas == 4 &&
        strcmp(qp_2.stanza_list[0].name, "nothing") == 0 ){
        printf("\t"QBT_GREEN"Passed: "QBT_RESET"qb_parse_program_file\n");
    }
    else {
        printf("\t"QBT_RED"Failed: "QBT_RESET"qb_parse_program_file\n");
    }

    qb_configure_program(&qp_2, 0, 0);
    int stanza_num = 0;

    int array_size = 10; 

    // Getting the arrays ready
    cl_int err;

    float *a = (float*)malloc(sizeof(float)*array_size);
    float *b = (float*)malloc(sizeof(float)*array_size);
    float *c = (float*)malloc(sizeof(float)*array_size);

    for (int i = 0; i < array_size; ++i){
        a[i] = 1;
        b[i] = 1;
        c[i] = 0;
    }

    cl_mem d_a = clCreateBuffer(qp_2.context,
                                CL_MEM_READ_WRITE,
                                array_size * sizeof(float),
                                NULL,
                                &err);

    cl_check(err);

    cl_mem d_b = clCreateBuffer(qp_2.context,
                                CL_MEM_READ_WRITE,
                                array_size * sizeof(float),
                                NULL,
                                &err);

    cl_check(err);

    cl_mem d_c = clCreateBuffer(qp_2.context,
                                CL_MEM_READ_WRITE,
                                array_size * sizeof(float),
                                NULL,
                                &err);

    cl_check(err);

    cl_check(
        clEnqueueWriteBuffer(qp_2.command_queue,
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
        clEnqueueWriteBuffer(qp_2.command_queue,
                             d_b,
                             CL_TRUE,
                             0,
                             array_size * sizeof(float),
                             b,
                             0,
                             NULL,
                             NULL)
    );

    cl_check(
        clEnqueueWriteBuffer(qp_2.command_queue,
                             d_c,
                             CL_TRUE,
                             0,
                             array_size * sizeof(float),
                             c,
                             0,
                             NULL,
                             NULL)
    );

    qb_set_args(&qp_2, "poem_check", 3, "cl_mem a", &d_a,
                                        "cl_mem b", &d_b,
                                        "cl_mem c", &d_c);
    qb_set_arg(&qp_2, "poem_check", "int stanza_num", &stanza_num);
    qb_set_arg(&qp_2, "poem_check", "int array_size", &array_size);

    qb_run(qp_2, "poem_check", array_size, array_size);

    cl_check(
        clEnqueueReadBuffer(qp_2.command_queue,
                            d_c,
                            CL_TRUE,
                            0,
                            array_size * sizeof(float),
                            c,
                            0,
                            NULL,
                            NULL)
    );

    bool result = true;
    for (int i = 0; i < array_size; ++i){
        if (c[i] != 2.5){
            result = false;
        }
    }

    if (result){
        printf("\t"QBT_GREEN"Passed: "QBT_RESET"qb_configure_program\n");
    }
    else {
        printf("\t"QBT_RED"Failed: "QBT_RESET"qb_configure_program\n");
    }

    quibble_program qp_sum = qb_combine_programs(qp_check, qp_2);

    if (qp_sum.num_verses == qp_2.num_verses + 1 &&
        qp_sum.num_stanzas == qp_2.num_stanzas + 1 &&
        qp_sum.num_poems == qp_2.num_poems + 1 &&
        strcmp(qp_sum.verse_list[1].name, "nothing") == 0 &&
        strcmp(qp_sum.stanza_list[1].name, "nothing") == 0 &&
        strcmp(qp_sum.poem_list[1].name, "nothing") == 0){
        printf("\t"QBT_GREEN"Passed: "QBT_RESET"qb_combine_programs\n");
    }
    else {
        printf("\t"QBT_RED"Failed: "QBT_RESET"qb_combine_programs\n");
    }

    quibble_program both_programs[2];
    both_programs[0] = qp_check;
    both_programs[1] = qp_2;
    quibble_program qp_array_sum = qb_combine_program_array(both_programs, 2);

    if (qp_sum.num_verses == qp_array_sum.num_verses &&
        qp_sum.num_stanzas == qp_array_sum.num_stanzas &&
        qp_sum.num_poems == qp_array_sum.num_poems){
        printf("\t"QBT_GREEN"Passed: "QBT_RESET"qb_combine_program_array\n");
    }
    else {
        printf("\t"QBT_RED"Failed: "QBT_RESET"qb_combine_program_array\n");
    }

    free(a);
    free(b);
    free(c);
    cl_check(clReleaseMemObject(d_a));
    cl_check(clReleaseMemObject(d_b));
    cl_check(clReleaseMemObject(d_c));
    qb_shallow_free_program(qp_check);
    qb_shallow_free_program(qp_2);
    qb_shallow_free_program(qp_sum);
    qb_free_program(qp_array_sum);
}
