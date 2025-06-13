#ifndef QUIBBLE_VERSES_H
#define QUIBBLE_VERSES_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

#include "macros.h"
#include "io.h"
#include "quibble_inputs.h"

typedef struct{
    char *variable;
} quibble_arg;

typedef struct{
    char *variable;
    char *value;
} quibble_keyword;

typedef struct{
    char *body;
    char *name;
    quibble_keyword *kwargs;
    quibble_arg *args;
    int num_kwargs;
    int num_args;
    bool echo;
} quibble_verse;

typedef struct{
    quibble_keyword *kwargs;
    int num_kwargs;
    quibble_arg *args;
    int num_args;
    char *name;
    char *prologue;
    char *epilogue;
} quibble_stanza;

typedef struct{
    quibble_keyword *kwargs;
    int num_kwargs;
    quibble_arg *args;
    int num_args;
    char *body;
    char *name;
} quibble_poem;

typedef struct{
    char *everything_else;
    char *body;
    quibble_verse *verse_list;
    quibble_stanza *stanza_list;
    quibble_poem *poem_list;
    int num_verses;
    int num_stanzas;
    int num_poems;
} quibble_program;

// TODO
// Remove echoes
void qb_configure_verse(quibble_verse *qv, int n, ...);
quibble_verse qb_echo_verse(quibble_verse qv, int n, ...);

char *qb_expand_verse(quibble_program qp, char* verse_name, char *prologue);
char *qb_expand_stanza(quibble_program qp,
                       char* stanza_name, char *prologue, char *body);
char *qb_expand_poem(quibble_program qp, int index);

void qb_configure_program(quibble_program *qp, int n, ...);

// string manip
bool qb_is_stanza(char *stanza, int offset);
bool qb_is_poem(char *poem, int offset);
bool qb_is_verse(char *verse, int offset);
bool qb_is_inlined(char *verse);
void qb_preprocess_content(char *verse);

// Reads an input file and parses everything into verses or OCL functions
quibble_program qb_parse_program(char *buffer);
quibble_program qb_create_program(char *filename);
quibble_verse qb_find_verse(quibble_program qp, char *verse_name);
quibble_stanza qb_find_stanza(quibble_program qp, char *stanza_name);
quibble_poem qb_find_poem(quibble_program qp, char *poem_name);
int qb_find_verse_index(quibble_program qp, char *verse_name);
int qb_find_stanza_index(quibble_program qp, char *stanza_name);
int qb_find_poem_index(quibble_program qp, char *poem_name);

// Args / Kwargs
int qb_find_number_of_kwargs(char *prologue);
int qb_find_number_of_args(char *prologue);

quibble_arg *qb_parse_args(char *prologue, int num_entries);
quibble_keyword *qb_parse_keywords(char *prologue, int num_entries);

int qb_find_arg_index(quibble_arg *arg, int n, char *variable);
int qb_find_kwarg_index(quibble_keyword *qk, int n, char *variable);

// Parsing from file
quibble_stanza qb_parse_stanza(char *stanza);
quibble_poem qb_parse_poem(char *stanza);
quibble_verse qb_parse_verse(char *verse);

// Build / expansion
char *qb_create_prologue(char *config, char *name,
                         quibble_arg *args, int num_args,
                         quibble_keyword *qkwargs, int num_kwargs);
void qb_build_program(quibble_program qp);
void qb_rebuild_program(quibble_program qp);

// Free
void qb_free_arg(quibble_arg arg);
void qb_free_keyword(quibble_keyword qkwarg);
void qb_free_verse(quibble_verse qv);
void qb_free_stanza(quibble_stanza qs);
void qb_free_poem(quibble_poem qp);
void qb_free_keyword_array(quibble_keyword *qkwargs, int n);
void qb_free_arg_array(quibble_arg *args, int n);
void qb_free_program(quibble_program qp);

#endif
