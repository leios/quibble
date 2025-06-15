#include "tests.h"

void quibble_program_tests(void){

    printf("String Parsing Tests...\n");
    // qb_parse_kwargs
    char prologue_1[10] = " | x = 5;";
    char prologue_2[30] = "a , b, c | x = 5; y = 2*x+3;";
    char prologue_3[60] = "\t\n a, \t b, \n c \n | \t\n variable\t\n = \t\t\n whatever\n\n\t ;";
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
        strcmp(check_kwargs_1[0].value, "5") == 0 &&
        strcmp(check_kwargs_2[0].variable, "x") == 0 &&
        strcmp(check_kwargs_2[0].value, "5") == 0 &&
        strcmp(check_kwargs_2[1].variable, "y") == 0 &&
        strcmp(check_kwargs_2[1].value, "2*x+3") == 0 &&
        strcmp(check_kwargs_3[0].variable, "variable") == 0 &&
        strcmp(check_kwargs_3[0].value, "whatever") == 0 &&
        check_kwargs_4 == NULL && check_kwargs_5 == NULL &&
        check_kwargs_7 == NULL &&
        strcmp(check_kwargs_6[0].variable, "a") == 0 &&
        strcmp(check_kwargs_6[0].value, "6") == 0 ){
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
        strcmp(check_args_2[1].variable, "b") == 0 &&
        strcmp(check_args_2[2].variable, "c") == 0 &&
        strcmp(check_args_3[0].variable, "a") == 0 &&
        strcmp(check_args_3[1].variable, "b") == 0 &&
        strcmp(check_args_3[2].variable, "c") == 0 &&
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
    
    
/*

    // qb_create_prologue
    if (strcmp(qb_create_prologue(check_kwargs_1, num_args_1),
               "x = 5;\n") == 0 &&
        strcmp(qb_create_prologue(check_kwargs_2, num_args_2),
               "x = 5;\ny = 2*x+3;\n") == 0 &&
        strcmp(qb_create_prologue(check_kwargs_3, num_args_3),
               "variable = whatever;\n") == 0){
        printf(QBT_GREEN"Passed: "QBT_RESET"qb_create_prologue\n");
    }
    else {
        printf(QBT_RED"Failed: "QBT_RESET"qb_create_prologue\n");
    }

    qb_free_kwarg_array(check_kwargs_1, num_args_1);
    qb_free_kwarg_array(check_kwargs_2, num_args_2);
    qb_free_kwarg_array(check_kwargs_3, num_args_3);


    // qb_parse_verse
    char *empty_verse = QBINLINE(
        __verse check(){}
    );

    quibble_verse qv_1 = qb_parse_verse(empty_verse);
    if (qv_1.kwargs == NULL && qv_1.body == NULL &&
        strcmp(qv_1.name, "check") == 0){
        printf(QBT_GREEN"Passed: "QBT_RESET"qb_parse_verse\n");
    }
    else {
        printf(QBT_RED"Failed: "QBT_RESET"qb_parse_verse\n");
    }

    qb_free_verse(qv_1);

    // qb_create_program
    quibble_program qp = qb_create_program("../test/example.qbl");
    quibble_verse qv_2 = qb_find_verse(qp, "check");
    quibble_verse qv_3 = qb_find_verse(qp, "check2");

    char *stripped_verse = qb_strip_spaces(qv_2.body, 0, strlen(qv_2.body));

    quibble_program qp_2 = qb_create_program("../test/example_2.qbl");

    if (strcmp(qv_2.name, "check") == 0 &&
        strcmp(qv_2.kwargs[0].variable, "x") == 0 &&
        strcmp(qv_2.kwargs[0].value, "7") == 0 &&
        strcmp(qv_3.name, "check2") == 0 &&
        strcmp(stripped_verse, "simple_print();") == 0 &&
        qp_2.verse_list == NULL){
        printf(QBT_GREEN"Passed: "QBT_RESET"qb_create_program\n");
    }
    else {
        printf(QBT_RED"Failed: "QBT_RESET"qb_create_program\n");
    }

    // qb_configure_verse and qb_create_echo
    qb_configure_verse(&qv_2, 1, "x", "int", 8);

    bool final_val = true;
    if (final_val){
        final_val = strcmp(qv_2.kwargs[0].value, "8") == 0;
    }

    // Checking that x can be overwritten
    quibble_verse e_1 = qb_echo_verse(qv_2, 1, "x", "float", 8.0);
    if (final_val){
        final_val = strcmp(e_1.kwargs[0].value, "8.000000") == 0;
    }

    // Checking that y is correct
    if (final_val){
        final_val = strcmp(e_1.kwargs[1].value, "8") == 0;
    }

    if (final_val){
        printf(QBT_GREEN"Passed: "QBT_RESET"qb_configure_verse\n");
    }
    else {
        printf(QBT_RED"Failed: "QBT_RESET"qb_configure_verse\n");
    }

    qb_free_verse(e_1);
    qb_free_program(qp);
    qb_free_program(qp_2);
*/
}
