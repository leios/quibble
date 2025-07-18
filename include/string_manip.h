#ifndef STRING_MANIP_H
#define STRING_MANIP_H

#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

char *qb_expand_path(char *path, char *base_path);
char *qb_find_path(char *filename);
char *qb_config_file(char *path);

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
int qb_find_limited_occurrences(char *query, int limit, char *body);

// QBINLINE
bool qb_is_inlined(char *verse);
void qb_preprocess_content(char *verse);

#endif
