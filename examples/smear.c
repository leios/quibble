/*------------smear-----------------------------------------------------------//
 Purpose: A simple video with smears
   Notes: After installation, compile with:
              `gcc -o smear smear.c -lquibble`
//----------------------------------------------------------------------------*/

#include <stdio.h>

#include "quibble.h"

void create_filename(char *buffer, int i){
    sprintf(buffer, "check%d%d%d%d.png", i%10000, i%1000, i%100, i%10);
}

int main(void){

    quibble_program qp = qb_parse_program_file("smear.qbl");
    qb_configure_program(&qp, 0, 0);

    qb_print_program(qp);

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

    quibble_point_2D position = qb_point_2D(0,0);
    quibble_point_2D velocity = qb_point_2D(0,0);

    char filename[15] = {0};
    for (int i = 0; i < 1; ++i){
        qb_set_args(&qp, "smear_shader", 2,
                    "quibble_pixels_prgba8888 qps", qpix,
                    "quibble_simple_camera qcam", &qcam,
                    "quibble_point_2D position", &position,
                    "quibble_point_2D velocity", &velocity);

        qb_run(qp, "smear_shader", width*height, 256);

        qb_pixels_device_to_host(qpix);

        create_filename(filename, i);
        qb_write_png_file(filename, qpix);
    }

    qb_free_program(qp);
    qb_free_pixels(qpix);

    return 0;
}
