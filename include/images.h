#ifndef IMAGES_H
#define IMAGES_H

#include "quibble_program.h"

#define RGBA8888 4
#define RGB888 3

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
} quibble_color_rgba8888;

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} quibble_color_rgb888;

typedef struct {
    void *host_data;
    int color_type;
    int height;
    int width;

    // CL stuff
    cl_mem device_data;
    cl_command_queue command_queue;
} quibble_pixels;

// Image IO
unsigned char qb_color_clamp(float value,
                             float min_value,
                             float max_value);
quibble_color_rgba8888 qb_zero_color_rgba8888(void);
quibble_color_rgb888 qb_zero_color_rgb888(void);

quibble_color_rgba8888 qb_color_rgba8888(float red,
                                         float green,
                                         float blue,
                                         float alpha);
quibble_color_rgb888 qb_color_rgb888(float red, float green, float blue);

quibble_pixels qb_create_pixel_array(quibble_program qp,
                                     int width,
                                     int height,
                                     int color_type);
quibble_pixels qb_create_pixel_array_from_file(char *filename,
                                               quibble_program qp,
                                               int width,
                                               int height,
                                               int color_type);

int qb_get_color_size(int color_type);

void qb_pixels_device_to_host(quibble_pixels qps);
void qb_pixels_host_to_device(quibble_pixels qps);

// File Formats

char *qb_find_file_extension(char* filename);

unsigned char *qb_read_file(char *filename,
                            int width,
                            int height,
                            int color_type);

void qb_write_file(char *filename, quibble_pixels qps);
void qb_write_png_file(char *filename, quibble_pixels qps);
void qb_write_bmp_file(char *filename, quibble_pixels qps);
void qb_write_jpg_file(char *filename, quibble_pixels qps, int quality);

void qb_free_pixels(quibble_pixels qps);

#endif
