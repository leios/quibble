#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tests.h"

int main(int argc, char **argv){
    int platform = 0;
    int device = 0;

    if (argc == 1){
        fprintf(stderr, "No platform or device provided, defaulting to 0 0!");
    }
    else if (argc == 2){
        fprintf(stderr, "Too few arguments provided. Defaulting to 0 0!");
    }
    else if (argc > 3){
        fprintf(stderr, "Too many arguments provided. Defaulting to 0 0!");
    }
    else {
        platform = strtol(argv[1], NULL, 10);
        device = strtol(argv[2], NULL, 10);
        printf("Platform %d\nDevice %d\n", platform, device);
    }

    // Tests
    quibble_io_tests();
    quibble_arg_parsing_tests();
    quibble_program_tests(platform, device);
    quibble_image_tests(platform, device);

    printf("Testing done!\n");

    return 0;
}
