/*-------------camera---------------------------------------------------------//
 Purpose: A quibble camera is a map from floating-point space to integer space
          so we can write to a pixel array

   Notes: I am calling it a "camera", but have not implemented any 3D structures
          `find_bin(...)` has a cutoff from [0->1)*world_size.
              this means `find_bin(0,0,qcam)` will round up 
//----------------------------------------------------------------------------*/

#include "../include/quibble.h"

quibble_simple_camera qb_create_simple_camera(float ppu,
                                              float world_size_x,
                                              float world_size_y,
                                              float world_position_x,
                                              float world_position_y){
    quibble_simple_camera qcam;
    qcam.ppu = ppu;
    qcam.world_size_x = world_size_x;
    qcam.world_size_y = world_size_y;
    qcam.world_position_x = world_position_x - 0.5 * world_size_x;
    qcam.world_position_y = world_position_y - 0.5 * world_size_y;

    return qcam;
}

int qb_find_camera_width(quibble_simple_camera qcam){
    if (qcam.world_size_x == 0){
        return 1;
    }
    return ceil(qcam.world_size_x * qcam.ppu);
}

int qb_find_camera_height(quibble_simple_camera qcam){
    if (qcam.world_size_y == 0){
        return 1;
    }
    return ceil(qcam.world_size_y * qcam.ppu);
}

bool qb_pixels_cam_consistent(quibble_pixels qpix, quibble_simple_camera qcam){
    if (qpix.height == qb_find_camera_height(qcam) &&
        qpix.width == qb_find_camera_width(qcam)){
        return true;
    }
    else {
        return false;
    }
}

int qb_find_bin(float x, float y, quibble_simple_camera qcam){
    int width = qb_find_camera_width(qcam);
    int height = qb_find_camera_height(qcam);

    int x_bin = 0;
    if (qcam.world_size_x > 0){
        x_bin = floor(width * ((x - qcam.world_position_x)/qcam.world_size_x));
    }

    if (x_bin < 0 || x_bin > width){
        return -1;
    }

    int y_bin = 0;
    if (qcam.world_size_y > 0){
        y_bin = floor(height * ((y - qcam.world_position_y)/qcam.world_size_y));
    }

    if (y_bin < 0 || y_bin > width){
        return -1;
    }

    return x_bin + y_bin * width;
}

