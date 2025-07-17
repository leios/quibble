#ifndef QUIBBLE_PROGRAM_H
#define QUIBBLE_PROGRAM_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

#include "macros.h"
#include <CL/cl.h>
#include "errors.h"
#include "quibble_args.h"

typedef struct{
    char *body;
    char *name;
    quibble_kwarg *kwargs;
    quibble_arg *args;
    int num_kwargs;
    int num_args;
} quibble_verse;

typedef struct{
    quibble_kwarg *kwargs;
    int num_kwargs;
    quibble_arg *args;
    int num_args;
    char *name;
    char *prologue;
    char *epilogue;
} quibble_stanza;

typedef struct{
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

    // CL stuff
    cl_platform_id *platform_ids;
    cl_device_id *device_ids;
    cl_uint num_devices;
    cl_uint num_platforms;
    int chosen_platform;
    int chosen_device;
    cl_context context;
    cl_command_queue command_queue;
    cl_program program;
    cl_kernel *kernels;
    bool configured;

} quibble_program;

// program functions
quibble_program qb_combine_programs(quibble_program qp_1, quibble_program qp_2);
quibble_program qb_combine_program_array(quibble_program *qps, int n);

// string manip
bool qb_is_stanza(char *stanza, int offset);
bool qb_is_poem(char *poem, int offset);
bool qb_is_verse(char *verse, int offset);

quibble_program qb_parse_program(char *buffer, char *path);
quibble_program qb_parse_program_file(char *filename);

quibble_verse qb_find_verse(quibble_program qp, char *verse_name);
quibble_stanza qb_find_stanza(quibble_program qp, char *stanza_name);
quibble_poem qb_find_poem(quibble_program qp, char *poem_name);

int qb_find_verse_index(quibble_program qp, char *verse_name);
int qb_find_stanza_index(quibble_program qp, char *stanza_name);
int qb_find_poem_index(quibble_program qp, char *poem_name);

// Parsing from file
quibble_stanza qb_parse_stanza(char *stanza);
quibble_poem qb_parse_poem(char *stanza);
quibble_verse qb_parse_verse(char *verse);

// Free
void qb_free_arg(quibble_arg arg);
void qb_free_kwarg(quibble_kwarg qkwarg);
void qb_free_verse(quibble_verse qv);
void qb_free_stanza(quibble_stanza qs);
void qb_free_poem(quibble_poem qp);
void qb_free_kwarg_array(quibble_kwarg *qkwargs, int n);
void qb_free_arg_array(quibble_arg *args, int n);
void qb_free_program(quibble_program qp);
void qb_shallow_free_program(quibble_program qp);

// Printing
void qb_print_arg(quibble_arg qa);
void qb_print_kwarg(quibble_kwarg qk);
void qb_print_verse(quibble_verse qv);
void qb_print_stanza(quibble_stanza qs);
void qb_print_poem(quibble_poem qp);
void qb_print_program(quibble_program qp);
void print_cl_info(quibble_program qp);

#endif
