/*-------------demo.c---------------------------------------------------------//

Purpose: This file is a quick example of how we might read function fragments in
         to construct an OpenCL kernel with Quibble.

//----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/macros.h"
#include <CL/cl.h>

#include "../include/errors.h"
#include "../include/quibble_program.h"
#include "../test/tests.h"

int main(void){


    // Tests
    quibble_io_tests();
    quibble_arg_parsing_tests();
    quibble_program_tests();
    quibble_image_tests();

    printf("Testing done!\n");

    return 0;
}
