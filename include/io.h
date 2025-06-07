#ifndef IO_H
#define IO_H

#include <stdlib.h>
#include <png.h>

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
} quibble_pixel;

quibble_pixel qb_zero_pixel(void);
quibble_pixel *qb_create_pixel_array(int height, int width);



#endif
