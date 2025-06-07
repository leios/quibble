/*-------------io.h-----------------------------------------------------------//

Purpose: This file is meant to define most of what is needed for 
         image and video creation

//----------------------------------------------------------------------------*/

#include "../include/io.h"

quibble_pixel qb_zero_pixel(void){
    quibble_pixel qp;
    qp.red = 0;
    qp.green = 0;
    qp.blue = 0;
    qp.alpha = 0;
    return qp;
}

quibble_pixel *qb_create_pixel_array(int height, int width){
    quibble_pixel *qpa =
        (quibble_pixel *)malloc(sizeof(quibble_pixel)*width*height);

    for (int i = 0; i < width * height; ++i){
        qpa[i] = qb_zero_pixel();
    }
    return qpa;
}
