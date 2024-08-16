#ifndef QUIBBLE_VERSES_H
#define QUIBBLE_VERSES_H

typedef struct{
    char *variable;
    char *value;
} quibble_keyword;

tyedef struct{
    char *body;
    quibble_keyword *kwargs;
    bool echo;
} quibble_verse;

typedef struct{
    char *everything_else;
    char *names;
    quibble_verse *list;
} quibble program;

// Reads an input file and parses everything into verses or OCL functions
quibble_program qb_create_program(char *filename);
quibble_verse qb_find_verse(quibble_program qp, char *verse_name);

// Configures preambles of existing verses
void qb_configure_verse(quibble_verse *qv, ...);

// An echo is a verse with the same body, but different preamble
quibble verse qb_echo_verse(quibble_verse qv, ...);

// To be used in `qb_configure_verse` to create preamble string
char *qb_configure_preamble(quibble_verse qv);

void qb_free_keyword(quibble_keyword qkwarg);
void qb_free_verse(quibble_verse qv);
void qb_free_keyword_array(quibble keyword *qkwargs);

#endif
