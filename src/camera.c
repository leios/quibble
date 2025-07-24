/*-------------camera---------------------------------------------------------//
 Purpose: A quibble camera is a map from floating-point space to integer space
          so we can write to a pixel array

   Notes: I am calling it a "camera", but have not implemented any 3D structures
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
    qcam.world_position_x = world_position_x;
    qcam.world_position_y = world_position_y;

    return qcam;
}

int qb_find_camera_width(quibble_simple_camera qcam){
    return ceil(qcam.world_size_x * qcam.ppu);
}

int qb_find_camera_height(quibble_simple_camera qcam){
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

int find_bin(float x, float y, quibble_simple_camera qcam){
}

