/*------------swirled---------------------------------------------------------//
 Purpose: This file is a slightly more complicated use-case for chaos generators
          and will create a square and "swirl" it.
   Notes: After installation, compile with:
              `gcc -o swirled swirled.c -lquibble`
//----------------------------------------------------------------------------*/

#include <stdio.h>

#include "quibble.h"

int main(void){

    quibble_program qp = qb_parse_program_file("swirled.qbl");
    qb_configure_program(&qp, 1, 0);

    int width = 1920;
    int height = 1080;
    quibble_pixels qpix = 
        qb_create_pixel_array(qp, width, height, PRGBA8888);

    float world_size_x = 4;
    float ppu = width / world_size_x;
    float world_size_y = height/ppu;

    float world_position_x = 0;
    float world_position_y = 0;

    quibble_simple_camera qcam = qb_create_simple_camera(ppu,
                                                         world_size_x,
                                                         world_size_y,
                                                         world_position_x,
                                                         world_position_y);

    qb_set_args(&qp, "square_shader", 2,
                "quibble_pixels_prgba8888 qps", qpix,
                "quibble_simple_camera qcam", &qcam);

    qb_run(qp, "square_shader", width*height, 256);

    qb_pixels_device_to_host(qpix);
    qb_write_png_file("check.png", qpix);

    qb_free_program(qp);
    qb_free_pixels(qpix);

    return 0;
}
