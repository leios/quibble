#ifndef QUIBBLE_ARGS_H
#define QUIBBLE_ARGS_H

#include <stdlib.h>
#include <stdio.h>

#include "macros.h"
#include "string_manip.h"

typedef struct{
    char *type;
    char *variable;
} quibble_arg;

typedef struct{
    char *type;
    char *variable;
    char *value;
} quibble_kwarg;

void qb_find_type_arg(char *arg, char **type, char **variable);
void qb_parse_arg(quibble_arg *qa, char *arg);
void qb_parse_kwarg(quibble_kwarg *qk, char *lhs, char *rhs);

// Args / Kwargs
int qb_find_number_of_kwargs(char *config);
int qb_find_number_of_args(char *config);

quibble_arg *qb_parse_args(char *config, int num_entries);
quibble_kwarg *qb_parse_kwargs(char *config, int num_entries);

int qb_find_arg_index(quibble_arg *arg, int n, char *variable);
int qb_find_kwarg_index(quibble_kwarg *qk, int n, char *variable);

// prologue
char *qb_create_prologue(char *config, char *name,
                         quibble_arg *qargs, int num_args,
                         quibble_kwarg *qkwargs, int num_kwargs);

// Free
void qb_free_arg(quibble_arg arg);
void qb_free_kwarg(quibble_kwarg qkwarg);
void qb_free_arg_array(quibble_arg *args, int n);
void qb_free_kwarg_array(quibble_kwarg *qkwargs, int n);

// Printing
void qb_print_arg(quibble_arg qa);
void qb_print_kwarg(quibble_kwarg qk);

#endif
