/*------------simple_shader---------------------------------------------------//
 Purpose: This file is a simple example of a fragment shader with quibble
   Notes: After installation, compile with:
              `gcc -o simple_shader simple_shader.c -lquibble`
//----------------------------------------------------------------------------*/

#include <stdio.h>

#include "quibble.h"

int main(void){

    // Step 1: Define program.
    //         This is done with QBINLINE, but can also be a separate file.
    char *program = QBINLINE(
        @include "QB/simple_shader.qbl"
        __poem simple_shader(quibble_pixels_rgba8888 qps,
                             quibble_simple_camera qcam){
            quibble_point_2D pt = qb_find_point_location(_idx, qcam);
            quibble_color_rgba8888 qcolor = qb_color_rgba8888(0,0,0,1);

            qcolor.red = qb_color_clamp(
                (pt.x - qcam.world_position_x) / qcam.world_size_x,
                0, 1
            );
            qcolor.blue = qb_color_clamp(
                (pt.y - qcam.world_position_y) / qcam.world_size_y,
                0, 1
            );

            histogram_output_rgba8888(pt, qcolor, qcam, qps);
        }
    );

    // Step 2: Build program and configure with appropriate platform / device.
    //         These can be found by calling the associated `qbinfo` command.
    //  Notes: for this example, platform_id = 0, device_id = 0
    //         The second argument for parsing is the directory we wish to
    //             expand the program at. We are leaving it blank for now ("").

    quibble_program qp = qb_parse_program(program, "");
    qb_configure_program(&qp, 0, 0);

    // Step 3: Define the output

    int width = 1920;
    int height = 1080;
    quibble_pixels qpix = 
        qb_create_pixel_array(qp, width, height, RGBA8888);

    // Step 4: Create a camera (a method to convert real-space into pixel-space.
    //         world_size = real-space units to map pixels on to
    //         world_position = the location of the camera in space
    //         ppu = "points per unit" is similar to DPI for printing.
    //             It is a measure of how many points we want in each real-space
    //             unit (of length 1). Here, we want 1920 points in 4 real-space
    //             units so we can exactle measure one point per pixel
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

    // Step 5: Set args. This is done with a variadic function where we are:
    //         1. Calling on the program (&qp)
    //         2. Specifying the poem from the program
    //         3. Choosing the number of arguments to set
    //         4. Setting each arg with the poem's argument (with type info)
    //                as well as the data in C we want associated with that arg 
    printf("yo yo\n");
    qb_set_args(&qp, "simple_shader", 2,
                "quibble_pixels_rgba8888 qps", qpix,
                "quibble_simple_camera qcam", &qcam);

    // Step 6: Run. We need to set the number of threads and workgroup size
    //         Number of threads should be the numbe of pixels
    //         workgroup size can be arbitrarily chosen, so we are choosing 256

    qb_run(qp, "simple_shader", width*height, 256);

    // Step 7: return the data from the GPU and save
    qb_pixels_device_to_host(qpix);
    qb_write_png_file("check.png", qpix);

    qb_free_program(qp);
    qb_free_pixels(qpix);

    return 0;
}
