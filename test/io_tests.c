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
    test_value = (
        qb_find_next_char(check, 0, 't') == 10 &&
        qb_find_next_char(check, 0, 'z') == -1 &&
        qb_find_next_char(check, 11, 't') == 13
    );
    qb_check(test_value, "qb_find_next_char");

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

bool qb_color_compare_rgba8888(quibble_color_rgba8888 qc_1,
                               quibble_color_rgba8888 qc_2){
    if (qc_1.red == qc_2.red &&
        qc_1.green == qc_2.green &&
        qc_1.blue == qc_2.blue &&
        qc_1.alpha == qc_2.alpha){
        return true;
    }
    else{
        return false;
    }
}

bool qb_color_compare_rgb888(quibble_color_rgb888 qc_1,
                             quibble_color_rgb888 qc_2){
    if (qc_1.red == qc_2.red &&
        qc_1.green == qc_2.green &&
        qc_1.blue == qc_2.blue){
        return true;
    }
    else{
        return false;
    }
}


void quibble_image_tests(int platform, int device){

    bool test_value = false;

    printf("Quibble Image tests...\n");

    quibble_color_rgba8888 blank_rgba8888 = qb_zero_color_rgba8888();
    quibble_color_rgb888 blank_rgb888 = qb_zero_color_rgb888();

    test_value = (
        blank_rgba8888.red == 0 &&
        blank_rgba8888.blue == 0 &&
        blank_rgba8888.green == 0 &&
        blank_rgba8888.alpha == 0
    );
    qb_check(test_value, "qb_zero_color_rgba8888");

    test_value = (
        blank_rgb888.red == 0 &&
        blank_rgb888.blue == 0 &&
        blank_rgb888.green == 0
    );
    qb_check(test_value, "qb_zero_color_rgb888");


    quibble_color_rgba8888 test_color_rgba8888 =
        qb_color_rgba8888(1, 0.5, 0, 0.125);
    quibble_color_rgb888 test_color_rgb888 = qb_color_rgb888(1, 0.5, 0);

    test_value = (
        test_color_rgba8888.blue == 0 &&
        test_color_rgba8888.red == 255
    );
    qb_check(test_value, "qb_color_rgba8888");

    test_value = (
        test_color_rgb888.blue == 0 &&
        test_color_rgb888.red == 255
    );
    qb_check(test_value, "qb_color_rgb888");

    char *program_string = QBINLINE(
        @include "QB/colors.qbl"
        __poem check(quibble_pixels_rgba8888 qps, quibble_color_rgba8888 test_color){
            if (_idx == 0){
                qps[_idx].red = test_color.red;
                qps[_idx].green = test_color.green;
                qps[_idx].blue = test_color.blue;
                qps[_idx].alpha = test_color.alpha;
            }
        }
    );

    quibble_program qp = qb_parse_program(program_string, "");

    qb_configure_program(&qp, platform, device);

    int width = 2;
    int height = 1;
    quibble_pixels qps_rgba8888 =
        qb_create_pixel_array(qp,width,height,RGBA8888);
    quibble_pixels qps_rgb888 = qb_create_pixel_array(qp,width,height,RGB888);

    qb_set_args(&qp, "check", 2,
                "quibble_pixels_rgba8888 qps", qps_rgba8888,
                "quibble_color_rgba8888 test_color", &test_color_rgba8888);

    qb_run(qp, "check", 2, 2);

    qb_pixels_device_to_host(qps_rgba8888);

    quibble_color_rgba8888 *qps_rgba8888_color_array =
        (quibble_color_rgba8888 *)qps_rgba8888.host_data;

    quibble_color_rgb888 *qps_rgb888_color_array =
        (quibble_color_rgb888 *)qps_rgb888.host_data;
    qps_rgb888_color_array[0] = test_color_rgb888;

    test_value = (
        qb_color_compare_rgba8888(qps_rgba8888_color_array[0],
                                  test_color_rgba8888) &&
        qb_color_compare_rgba8888(qps_rgba8888_color_array[1],
                                  blank_rgba8888) &&
        qps_rgba8888.height == 1 &&
        qps_rgba8888.width == 2 &&
        qps_rgba8888.color_type == RGBA8888 &&
        qb_color_compare_rgb888(qps_rgb888_color_array[0], test_color_rgb888) &&
        qps_rgb888.color_type == RGB888
    );
    qb_check(test_value, "qb_create_pixel_array");

    test_value = (strcmp(qb_find_file_extension("check.png"), "png") == 0);
    qb_check(test_value, "qb_find_file_extension");

    // Checked by hand in gimp...
    // probably should have a better way to do it though
    qb_write_png_file("check_alpha.png", qps_rgba8888);
    qb_write_file("check_noalpha.png", qps_rgb888);
    qb_write_file("check_noalpha.jpg", qps_rgb888);
    qb_write_file("check_noalpha.bmp", qps_rgb888);
    qb_write_file("check_noalpha", qps_rgb888);

    quibble_pixels qps_from_file_rgba8888 =
        qb_create_pixel_array_from_file("check_alpha.png",
                                        qp,
                                        width,
                                        height,
                                        RGBA8888);
    quibble_color_rgba8888 *qps_from_file_rgba8888_colors =
        (quibble_color_rgba8888 *)qps_from_file_rgba8888.host_data;

    quibble_pixels qps_from_file_rgb888 =
        qb_create_pixel_array_from_file("check_noalpha.png",
                                        qp,
                                        width,
                                        height,
                                        RGB888);
    quibble_color_rgb888 *qps_from_file_rgb888_colors =
        (quibble_color_rgb888 *)qps_from_file_rgb888.host_data;

    quibble_pixels qps_from_jpg_rgb888 =
        qb_create_pixel_array_from_file("check_noalpha.jpg",
                                        qp,
                                        width,
                                        height,
                                        RGB888);
    quibble_color_rgb888 *qps_from_jpg_rgb888_colors =
        (quibble_color_rgb888 *)qps_from_jpg_rgb888.host_data;

    quibble_pixels qps_from_bmp_rgb888 =
        qb_create_pixel_array_from_file("check_noalpha.bmp",
                                        qp,
                                        width,
                                        height,
                                        RGB888);
    quibble_color_rgb888 *qps_from_bmp_rgb888_colors =
        (quibble_color_rgb888 *)qps_from_bmp_rgb888.host_data;

    quibble_pixels qps_from_none_rgb888 =
        qb_create_pixel_array_from_file("check_noalpha",
                                        qp,
                                        width,
                                        height,
                                        RGB888);
    quibble_color_rgb888 *qps_from_none_rgb888_colors =
        (quibble_color_rgb888 *)qps_from_none_rgb888.host_data;

    test_value = (
        qb_color_compare_rgba8888(qps_rgba8888_color_array[0],
                                  qps_from_file_rgba8888_colors[0]) ||
        qb_color_compare_rgb888(qps_rgb888_color_array[0],
                                qps_from_file_rgb888_colors[0]) ||
        qb_color_compare_rgb888(qps_rgb888_color_array[0],
                                qps_from_bmp_rgb888_colors[0]) ||
        qb_color_compare_rgb888(qps_rgb888_color_array[0],
                                qps_from_jpg_rgb888_colors[0]) ||
        qb_color_compare_rgb888(qps_rgb888_color_array[0],
                                qps_from_none_rgb888_colors[0])
    );
    qb_check(test_value, "Reading and Writing to file");

    qb_free_pixels(qps_rgba8888);
    qb_free_pixels(qps_rgb888);
    qb_free_pixels(qps_from_file_rgba8888);
    qb_free_pixels(qps_from_file_rgb888);
    qb_free_pixels(qps_from_bmp_rgb888);
    qb_free_pixels(qps_from_jpg_rgb888);
    qb_free_pixels(qps_from_none_rgb888);

    qb_free_program(qp);

    remove("check_alpha.png");
    remove("check_noalpha.png");
    remove("check_noalpha.jpg");
    remove("check_noalpha.bmp");
    remove("check_noalpha");

}
