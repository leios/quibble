#include "tests.h"

void quibble_io_tests(void){

    bool test_value = false;

    printf("Quibble IO String tests...\n");

    // QBINLINE
    char *kernel_string = QBINLINE(
        __verse check(){
        }
    );

    test_value = qb_is_inlined(kernel_string);
    qb_check(test_value, "qb_is_inlined");

    // qb_preprocess_content
    // QBINLINE creates a const, so copying to another container
    char *check_string = qb_copy(kernel_string);
    qb_preprocess_content(check_string);

    test_value = (strcmp(check_string,
                         "//\nQBINLINE\nGENERATED\n__verse\ncheck(){\n}") == 0);
    qb_check(test_value, "qb_preprocess_content");

    free(check_string);

    // Others
    // qb_replace_char_if_proceeding
    char check[23] = "This is a test string ";
    qb_replace_char_if_proceeding(check, "test", ' ', '\n');
    qb_replace_char_if_proceeding(check, "T", ' ', '_');

    test_value = (strcmp(check, "This_is a test\nstring ") == 0);
    qb_check(test_value, "qb_replace_char_if_proceeding");

    // qb_replace_char
    qb_replace_char(check, 23, '\n', ' ');

    test_value = (strcmp(check, "This_is a test string ") == 0);
    qb_check(test_value, "qb_replace_char");

    // qb_replace_next_char
    qb_replace_next_char(check, 11, ' ', '_');

    test_value = (strcmp(check, "This_is a test_string ") == 0);
    qb_check(test_value, "qb_replace_next_char");

    // qb_find_occurrences
    int occurrences = qb_find_occurrences(" ", check);

    qb_check((occurrences == 3), "qb_find_occurrences");

    // qb_strip_spaces
    char *stripped_1 = qb_strip_spaces(check, 9, 14);
    char *stripped_2 = qb_strip_spaces(check, 7, 9);

    test_value = (
        strcmp(stripped_1, "test_") == 0 &&
        strcmp(stripped_2, "a") == 0
    );
    qb_check(test_value, "qb_strip_spaces");

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
    test_value = (
        qb_find_next_string(check, 0, "t") == 10 &&
        qb_find_next_string(check, 0, "test") == 10 &&
        qb_find_next_string(check, 0, "z") == -1 &&
        qb_find_next_string(check, 11, "test") == -1 &&
        qb_find_next_string(check, 11, "string") == 15
    );
    qb_check(test_value, "qb_find_next_string");

    // qb_find_matching_char
    //                 "0123456789";
    char check_2[11] = "([]()(()))";

    test_value = (
        qb_find_matching_char(check_2, 10, 0, '(', ')') == 9 &&
        qb_find_matching_char(check_2, 10, 1, '[', ']') == 2 &&
        qb_find_matching_char(check_2, 10, 5, '(', ')') == 8 &&
        qb_find_matching_char(check_2, 10, 7, '(', ')') == -1 &&
        qb_find_matching_char("(()((", 5, 0, '(', ')') == -1
    );
    qb_check(test_value, "qb_find_matching_char");

    test_value = (
        qb_find_type_size("bool") == sizeof(char) &&
        qb_find_type_size("char") == sizeof(char) &&
        qb_find_type_size("signed char") == sizeof(signed char) &&
        qb_find_type_size("unsigned char") == sizeof(unsigned char) &&
        qb_find_type_size("short") == sizeof(short) &&
        qb_find_type_size("short int") == sizeof(short int) &&
        qb_find_type_size("signed short") == sizeof(signed short) &&
        qb_find_type_size("signed short int") == sizeof(signed short int) &&
        qb_find_type_size("unsigned short") == sizeof(unsigned short) &&
        qb_find_type_size("unsigned short int") == sizeof(unsigned short int) &&
        qb_find_type_size("int") == sizeof(int) &&
        qb_find_type_size("signed") == sizeof(signed) &&
        qb_find_type_size("signed int") == sizeof(signed int) &&
        qb_find_type_size("unsigned") == sizeof(unsigned) &&
        qb_find_type_size("unsigned int") == sizeof(unsigned int) &&
        qb_find_type_size("long") == sizeof(long) &&
        qb_find_type_size("long int") == sizeof(long int) &&
        qb_find_type_size("signed long") == sizeof(signed long) &&
        qb_find_type_size("signed long int") == sizeof(signed long int) &&
        qb_find_type_size("unsigned long") == sizeof(unsigned long) &&
        qb_find_type_size("unsigned long int") == sizeof(unsigned long int) &&
        qb_find_type_size("long long") == sizeof(long long) &&
        qb_find_type_size("long long int") == sizeof(long long int) &&
        qb_find_type_size("signed long long") == sizeof(signed long long) &&
        qb_find_type_size("signed long long int") == sizeof(signed long long int) &&
        qb_find_type_size("unsigned long long") == sizeof(unsigned long long) &&
        qb_find_type_size("unsigned long long int") == sizeof(unsigned long long int) &&
        qb_find_type_size("float") == sizeof(float) &&
        qb_find_type_size("double") == sizeof(double) &&
        qb_find_type_size("long double") == sizeof(long double) &&
        qb_find_type_size("meh") == sizeof(int) &&
        qb_find_type_size("char *") == sizeof(char *)
    );
    qb_check(test_value, "qb_find_type_size");

    char *path = qb_find_path("meh/../meh/hey.qbl");
    test_value = (strcmp(path, "meh/../meh/") == 0);
    qb_check(test_value, "qb_find_path");

    char *short_path_1 = "QB/check.qbl";
    char *short_path_2 = "ok???";

    char *full_path_1 = qb_expand_path(short_path_1, path);
    char *full_path_2 = qb_expand_path(short_path_2, path);

    // full_path_1 is different per system, uncomment to print what it shows.
    // printf("%s\n", full_path_1);
    test_value = (strcmp(full_path_2, "meh/../meh/ok???") == 0);
    qb_check(test_value, "qb_expand_path");

    free(full_path_1);
    free(full_path_2);
    free(path);
}
