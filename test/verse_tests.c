#include "tests.h"

void quibble_verse_tests(void){

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
}
