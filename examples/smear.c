/*------------smear-----------------------------------------------------------//
 Purpose: A simple video with smears
   Notes: After installation, compile with:
              `gcc -o smear smear.c -lquibble`
//----------------------------------------------------------------------------*/

#include <stdio.h>

#include "quibble.h"

void create_filename(char *buffer, int i){
    sprintf(buffer, "check%d%d%d%d.png",
            (i%10000 - i%1000)/1000,
            (i%1000 - i%100)/100,
            (i%100 - i%10)/10,
            i%10);
}

int main(void){

    quibble_program qp = qb_parse_program_file("smear.qbl");
    qb_output_program_to_file(qp, "program.txt");
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

    quibble_point_2D location = qb_point_2D(0,0);
    quibble_point_2D velocity = qb_point_2D(0,0);

    int num_frames = 10;
    float half_frame = num_frames*0.5;
    char filename[15] = {0};
    
    qb_set_args(&qp, "smear_shader", 4,
                "quibble_pixels_prgba8888 qps", qpix,
                "quibble_simple_camera qcam", &qcam,
                "quibble_point_2D location", &location,
                "quibble_point_2D velocity", &velocity);

    qb_set_args(&qp, "clear_bg", 2,
                "quibble_pixels_prgba8888 qps", qpix,
                "quibble_simple_camera qcam", &qcam);

    for (int i = 0; i < num_frames + 1; ++i){

        location = qb_point_2D(-0.5 + (float)i/num_frames,
                               -0.5 + (float)i/num_frames);

        if (i < half_frame){
            velocity = qb_point_2D(2 * (float) i / half_frame,
                                   2 * (float) i / half_frame);
        }
        else{
            velocity = qb_point_2D(2.0 - 2.0 * (i-half_frame) / (half_frame),
                                   2.0 - 2.0 * (i-half_frame) / (half_frame));
        }

        qb_set_args(&qp, "smear_shader", 2,
                    "quibble_point_2D location", &location,
                    "quibble_point_2D velocity", &velocity);

        qb_run(qp, "clear_bg", width*height, 256);
        qb_run(qp, "smear_shader", width*height, 256);

        qb_pixels_device_to_host(qpix);

        create_filename(filename, i);
        qb_write_png_file(filename, qpix);
        printf("%s\n", filename);
    }

    qb_free_program(qp);
    qb_free_pixels(qpix);

    return 0;
}
