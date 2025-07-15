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

bool qb_color_compare(quibble_color qc_1, quibble_color qc_2, int color_size){
    if (qc_1.red == qc_2.red &&
        qc_1.green == qc_2.green &&
        qc_1.blue == qc_2.blue &&
        ((color_size == 4) || (color_size == 4 && qc_1.alpha == qc_2.alpha))){
        return true;
    }
    else{
        return false;
    }
}

void quibble_image_tests(void){
    printf("Quibble Image tests...\n");

    quibble_color blank = qb_zero_color();
    if (blank.red == 0 &&
        blank.blue == 0 &&
        blank.green == 0 &&
        blank.alpha == 0){
        printf("\t"QBT_GREEN "Passed: " QBT_RESET "qb_zero_color\n");
    }
    else{
        printf("\t"QBT_RED "Failed: " QBT_RESET "qb_zero_color\n");
    }

    quibble_color test_color = qb_color(1, 0.5, 0, 0.125);

    if (test_color.blue == 0 &&
        test_color.red == 255){ 
        printf("\t"QBT_GREEN "Passed: " QBT_RESET "qb_color\n");
    }
    else{
        printf("\t"QBT_RED "Failed: " QBT_RESET "qb_color\n");
    }


    int width = 2;
    int height = 1;
    quibble_pixels qps_rgba8888 = qb_create_pixel_array(width,height,RGBA8888);
    quibble_pixels qps_rgb888 = qb_create_pixel_array(width,height,RGB888);

    qps_rgba8888.colors[0] = test_color;
    qps_rgb888.colors[0] = test_color;

    if (qb_color_compare(qps_rgba8888.colors[0], test_color, 4) &&
        qb_color_compare(qps_rgba8888.colors[1], blank, 4) &&
        qps_rgba8888.height == 1 &&
        qps_rgba8888.width == 2 &&
        qps_rgba8888.color_type == RGBA8888 &&
        qb_color_compare(qps_rgb888.colors[0], test_color, 4) &&
        qps_rgb888.color_type == RGB888){
        printf("\t"QBT_GREEN "Passed: " QBT_RESET "qb_create_pixel_array\n");
    }
    else{
        printf("\t"QBT_RED "Failed: " QBT_RESET "qb_create_pixel_array\n");
    }

    qb_fill_array_with_colors(qps_rgba8888);
    qb_fill_array_with_colors(qps_rgb888);

    quibble_color read_color = qb_read_color_from_rgb888_array(
        qps_rgb888.output_array, 0);

    if (qb_color_compare(
            qb_read_color_from_rgba8888_array(
                qps_rgba8888.output_array, 0
            ),
        test_color, 4) &&
        qb_color_compare(
            qb_read_color_from_rgba8888_array(
                qps_rgba8888.output_array, 1
            ),
        blank, 4) &&
        read_color.alpha == 255){
        printf("\t"QBT_GREEN "Passed: " QBT_RESET "qb_fill_array_with_colors\n");
    }
    else{
        printf("\t"QBT_RED "Failed: " QBT_RESET "qb_fill_array_with_colors\n");
    }

    // Checked by hand in gimp...
    // probably should have a better way to do it though
    qb_write_png_file("check_alpha.png", qps_rgba8888);
    qb_write_png_file("check_noalpha.png", qps_rgb888);

    quibble_pixels qps_from_file_rgba8888 =
        qb_create_pixel_array_from_file("check_alpha.png",
                                        width,
                                        height,
                                        RGBA8888);
    quibble_pixels qps_from_file_rgb888 =
        qb_create_pixel_array_from_file("check_noalpha.png",
                                        width,
                                        height,
                                        RGB888);

    if (qb_color_compare(qps_rgba8888.colors[0],
                         qps_from_file_rgba8888.colors[0], 4) ||
        qb_color_compare(qps_rgb888.colors[0],
                         qps_from_file_rgb888.colors[0], 3)){
        printf("\t"QBT_GREEN "Passed: " QBT_RESET "Reading and Writing to file\n")
;
    }
    else{
        printf("\t"QBT_RED "Failed: " QBT_RESET "Reading and Writing to file\n");
    }

    qb_free_pixels(qps_rgba8888);
    qb_free_pixels(qps_rgb888);
    qb_free_pixels(qps_from_file_rgba8888);
    qb_free_pixels(qps_from_file_rgb888);

    remove("check_alpha.png");
    remove("check_noalpha.png");

}
