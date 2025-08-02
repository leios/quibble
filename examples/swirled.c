/*------------swirled---------------------------------------------------------//
 Purpose: This file is a slightly more complicated use-case for chaos generators
          and will create a square and "swirl" it.
   Notes: After installation, compile with:
              `gcc -o swirled swirled.c -lquibble`
//----------------------------------------------------------------------------*/

#include <stdio.h>

#include "quibble.h"

int main(void){

    char *program = QBINLINE(
        @include "QB/chaos.qbl"
        @include "QB/output.qbl"
        __poem square_shader(quibble_pixels_prgba8888 qps,
                             quibble_simple_camera qcam){
            quibble_point_2D pt = qb_find_point_location(_idx, qcam);
            quibble_pcolor_rgba8888 qcolor = qb_pcolor_rgba8888(0,0,0,1,0.1);
            histogram_output_prgba8888(pt, qcolor, qcam, qps);

            qcolor = qb_pcolor_rgba8888(1,0,1,1,1);

            @SCALL rectangle_chaos(100, qcam, qcolor){
                if (_i > 10){
                    histogram_output_prgba8888(_pt, _clr, cam, qps);
                }

            }
        }
    );

    quibble_program qp = qb_parse_program(program, "");
    qb_print_program(qp);
    qb_configure_program(&qp, 0, 0);

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
