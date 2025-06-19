#include "tests.h"

void quibble_io_tests(void){

    printf("Quibble IO String tests...\n");

    // QBINLINE
    char *kernel_string = QBINLINE(
        __verse check(){
        }
    );

    // qb_is_qbinlined
    if (qb_is_inlined(kernel_string)){
        printf("\t"QBT_GREEN "Passed: " QBT_RESET "qb_is_qbinlined\n");
    }
    else{
        printf("\t"QBT_RED "Failed: " QBT_RESET "qb_is_qbinlined\n");
    }

    // qb_preprocess_content
    // QBINLINE creates a const, so copying to another container
    char *check_string = qb_copy(kernel_string);
    qb_preprocess_content(check_string);
    if (strcmp(check_string,
               "//\nQBINLINE\nGENERATED\n__verse\ncheck(){\n}") == 0){
        printf("\t"QBT_GREEN "Passed: " QBT_RESET "qb_preprocess_content\n");
    }
    else{
        printf("\t"QBT_RED "Failed: " QBT_RESET "qb_preprocess_content\n");
    }
    free(check_string);

    // Others
    // qb_replace_char_if_proceeding
    char check[23] = "This is a test string ";
    qb_replace_char_if_proceeding(check, "test", ' ', '\n');
    qb_replace_char_if_proceeding(check, "T", ' ', '_');
    if (strcmp(check, "This_is a test\nstring ") == 0){
        printf("\t"QBT_GREEN "Passed: "QBT_RESET"qb_replace_char_if_proceeding\n");
    }
    else{
        printf("\t"QBT_RED "Failed: "QBT_RESET"qb_replace_char_if_proceeding\n");
    }

    // qb_replace_char
    qb_replace_char(check, 23, '\n', ' ');
    if (strcmp(check, "This_is a test string ") == 0){
        printf("\t"QBT_GREEN "Passed: "QBT_RESET"qb_replace_char\n");
    }
    else{
        printf("\t"QBT_RED "Failed: "QBT_RESET"qb_replace_char\n");
    }

    // qb_replace_next_char
    qb_replace_next_char(check, 11, ' ', '_');
    if (strcmp(check, "This_is a test_string ") == 0){
        printf("\t"QBT_GREEN "Passed: "QBT_RESET"qb_replace_next_char\n");
    }
    else{
        printf("\t"QBT_RED "Failed: "QBT_RESET"qb_replace_next_char\n");
    }

    // qb_find_occurrences
    int occurrences = qb_find_occurrences(" ", check);

    if (occurrences == 3){
        printf("\t"QBT_GREEN "Passed: "QBT_RESET"qb_find_occurrences\n");
    }
    else{
        printf("\t"QBT_RED "Failed: "QBT_RESET"qb_find_occurrences\n");
    }

    char *stripped_1 = qb_strip_spaces(check, 9, 14);
    char *stripped_2 = qb_strip_spaces(check, 7, 9);
    // qb_strip_spaces
    if (strcmp(stripped_1, "test_") == 0 &&
        strcmp(stripped_2, "a") == 0){
        printf("\t"QBT_GREEN "Passed: "QBT_RESET"qb_strip_spaces\n");
    }
    else{
        printf("\t"QBT_RED "Failed: "QBT_RESET"qb_strip_spaces\n");
    }

    free(stripped_1);
    free(stripped_2);

    // qb_find_next_char
    if (qb_find_next_char(check, 0, 't') == 10 &&
        qb_find_next_char(check, 0, 'z') == -1 &&
        qb_find_next_char(check, 11, 't') == 13){
        printf("\t"QBT_GREEN "Passed: "QBT_RESET"qb_find_next_char\n");
    }
    else{
        printf("\t"QBT_RED "Failed: "QBT_RESET"qb_find_next_char\n");
    }

    // qb_find_next_string
    if (qb_find_next_string(check, 0, "t") == 10 &&
        qb_find_next_string(check, 0, "test") == 10 &&
        qb_find_next_string(check, 0, "z") == -1 &&
        qb_find_next_string(check, 11, "test") == -1 &&
        qb_find_next_string(check, 11, "string") == 15){
        printf("\t"QBT_GREEN"Passed: "QBT_RESET"qb_find_next_string\n");
    }
    else{
        printf("\t"QBT_RED"Failed: "QBT_RESET"qb_find_next_string\n");
    }

    // qb_find_matching_char
    //                 "0123456789";
    char check_2[11] = "([]()(()))";
        // qb_find_next_string
    if (qb_find_matching_char(check_2, 10, 0, '(', ')') == 9 &&
        qb_find_matching_char(check_2, 10, 1, '[', ']') == 2 &&
        qb_find_matching_char(check_2, 10, 5, '(', ')') == 8 &&
        qb_find_matching_char(check_2, 10, 7, '(', ')') == -1 &&
        qb_find_matching_char("(()((", 5, 0, '(', ')') == -1){
        printf("\t"QBT_GREEN"Passed: "QBT_RESET"qb_find_matching_char\n");
    }
    else{
        printf("\t"QBT_RED"Failed: "QBT_RESET"qb_find_matching_char\n");
    }

}
