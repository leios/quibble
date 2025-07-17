#ifndef IO_H
#define IO_H

#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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
    unsigned char *output_array;
    int color_type;
    int height;
    int width;
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

quibble_pixels qb_create_blank_pixel_array(int width,
                                           int height,
                                           int color_type);
quibble_pixels qb_create_pixel_array(int width, int height, int color_type);
quibble_pixels qb_create_pixel_array_from_file(char *filename,
                                               int width,
                                               int height,
                                               int color_type);

int qb_get_color_size(int color_type);

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

// String Manip
char *qb_expand_path(char *path, char *base_path);
char *qb_find_path(char *filename);
char *qb_config_file(char *path);

size_t qb_find_type_size(char *type);

// String Manipulation
bool qb_is_space(char a);
char *qb_copy(char *buffer);
char *qb_strip_spaces(char *body, int start_index, int end_index);
int qb_find_next_char(char *body, int current_index, char a);
int qb_find_next_string(char *body, int current_index, char *a);
int qb_find_matching_char(char *body, int body_size, int current_index,
                          char a, char b);

void qb_replace_char(char *body, int body_size, char a, char b);
void qb_replace_next_char(char *content, int index, char a, char b);
void qb_replace_char_if_proceeding(char *content, char *query, char a, char b);

int qb_find_occurrences(char *query, char *body);

// QBINLINE
bool qb_is_inlined(char *verse);
void qb_preprocess_content(char *verse);

#endif
