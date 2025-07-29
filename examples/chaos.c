/*------------chaos-----------------------------------------------------------//
 Purpose: This file is a simple chaos game example with quibble
   Notes: After installation, compile with:
              `gcc -o chaos chaos.c -lquibble`
//----------------------------------------------------------------------------*/

#include <stdio.h>

#include "quibble.h"

int main(void){

    char *program = QBINLINE(
        @include "chaos.qbl"
        @include "QB/output.qbl"
        __poem chaos_shader(quibble_pixels_rgba8888 qps,
                            quibble_simple_camera qcam){
            quibble_point_2D pt = qb_find_point_location(_idx, qcam);
            quibble_color_rgba8888 qcolor = qb_color_rgba8888(0,0,0,1);
            histogram_output_rgba8888(pt, qcolor, qcam, qps);

            qcolor = qb_color_rgba8888(1, 0, 1, 1);

            quibble_point_2D pt_1 = qb_point_2D(0, 0.5);
            quibble_point_2D pt_2 = qb_point_2D(-1, -0.5);
            quibble_point_2D pt_3 = qb_point_2D(1, -0.5);

            @SCALL sierpinski_chaos(1000, pt, qcolor,
                                    pt_1, qcolor,
                                    pt_2, qcolor,
                                    pt_3, qcolor){
                if (_i > 10){
                    histogram_output_rgba8888(pt, clr, qcam, qps);
                }

            }
        }
    );

    quibble_program qp = qb_parse_program(program, "");
    qb_print_program(qp);
    qb_configure_program(&qp, 1, 0);

    int width = 1920;
    int height = 1080;
    quibble_pixels qpix = 
        qb_create_pixel_array(qp, width, height, RGBA8888);

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

    qb_set_args(&qp, "chaos_shader", 2,
                "quibble_pixels_rgba8888 qps", qpix,
                "quibble_simple_camera qcam", &qcam);

    qb_run(qp, "chaos_shader", width*height, 256);

    qb_pixels_device_to_host(qpix);
    qb_write_png_file("check.png", qpix);

    qb_free_program(qp);
    qb_free_pixels(qpix);

    return 0;
}
