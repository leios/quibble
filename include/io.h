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
} quibble_color;

typedef struct {
    quibble_color *colors;
    unsigned char *output_array;
    int color_type;
    int height;
    int width;
} quibble_pixels;

// Image IO
unsigned char qb_color_clamp(float value,
                             float min_value,
                             float max_value);
quibble_color qb_color(float red, float green, float blue, float alpha);
quibble_color qb_zero_color(void);

quibble_pixels qb_create_blank_pixel_array(int width,
                                           int height,
                                           int color_type);
quibble_pixels qb_create_pixel_array(int width, int height, int color_type);
quibble_pixels qb_create_pixel_array_from_file(char *filename,
                                               int width,
                                               int height,
                                               int color_type);

int qb_get_color_size(int color_type);

quibble_color qb_read_color_from_rgb888_array(unsigned char *a, int i);
quibble_color qb_read_color_from_rgba8888_array(unsigned char *a, int i);
void qb_write_color_to_rgb888_array(unsigned char *a, int i, quibble_color qc);
void qb_write_color_to_rgba8888_array(unsigned char *a,
                                      int i,
                                      quibble_color qc);

void qb_fill_array_with_colors(quibble_pixels qps);
void qb_fill_colors_from_array(quibble_pixels qps);
unsigned char *qb_read_file(char *filename,
                            int width,
                            int height,
                            int color_type);
void qb_write_png_file(char *filename, quibble_pixels qps);

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
