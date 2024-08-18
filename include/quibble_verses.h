#ifndef QUIBBLE_VERSES_H
#define QUIBBLE_VERSES_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct{
    char *variable;
    char *value;
} quibble_keyword;

typedef struct{
    char *body;
    quibble_keyword *kwargs;
    int num_kwargs;
    bool echo;
} quibble_verse;

typedef struct{
    char *everything_else;
    char *names;
    quibble_verse *list;
} quibble_program;

void qb_replace_char(char *verse, int verse_size, char a, char b);
void qb_replace_char_if_proceeding(char *verse, int verse_size,
                                   char *preamble, int preamble_size,
                                   char a, char b);

void preprocess_verse(char *verse, int verse_size);

// Reads an input file and parses everything into verses or OCL functions
quibble_program qb_create_program(char *filename);
quibble_verse qb_find_verse(quibble_program qp, char *verse_name);
quibble_verse qb_parse_verse(char *verse);
quibble_keyword *qb_parse_keywords(char *preamble);

// Configures preambles of existing verses
void qb_configure_verse(quibble_verse *qv, ...);

// An echo is a verse with the same body, but different preamble
quibble_verse qb_echo_verse(quibble_verse qv, ...);

char *qb_create_preamble(quibble_verse qv);

void qb_free_keyword(quibble_keyword qkwarg);
void qb_free_verse(quibble_verse qv);
void qb_free_keyword_array(quibble_keyword *qkwargs, int n);

#endif
