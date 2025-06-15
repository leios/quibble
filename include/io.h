#ifndef IO_H
#define IO_H

#include <stdlib.h>
#include <string.h>
#include <png.h>

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
} quibble_pixel;

quibble_pixel qb_zero_pixel(void);
quibble_pixel *qb_create_pixel_array(int height, int width);

// String Manipulation
char *qb_copy(char *buffer);
char *qb_strip_spaces(char *body, int start_index, int end_index);
int qb_find_next_char(char *body, int body_size, int current_index, char a);
int qb_find_next_string(char *body, int body_size, int current_index,
                        char *a, int word_size);
int qb_find_matching_char(char *body, int body_size, int current_index,
                          char a, char b);

void qb_replace_char(char *body, int body_size, char a, char b);

void qb_replace_char_if_proceeding(char *body, int body_size,
                                   char *prologue, int prologue_size,
                                   char a, char b);

int qb_find_occurrences(char *query, char *body);

// QBINLINE
bool qb_is_inlined(char *verse);
void qb_preprocess_content(char *verse);

#endif
