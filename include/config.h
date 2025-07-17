#ifndef CONFIG_H
#define CONFIG_H

#include "quibble_program.h"
#include "images.h"

// OpenCL Interface
char *get_device_name(cl_device_id device_id);
char *get_platform_name(cl_platform_id platform_id);
void qb_find_platforms(quibble_program *qp);
void qb_find_devices(quibble_program *qp);
void qb_configure_program(quibble_program *qp, int platform, int device);
void qb_run(quibble_program qp, char *kernel_name,
            size_t global_item_size,
            size_t local_item_size);
void qb_set_arg(quibble_program *qp, char *poem, char *arg, void *data);
void qb_set_args(quibble_program *qp, char *poem, int n, ...);

// Build / expansion
bool qb_find_keyword(quibble_program qp, char *keyword);
bool qb_find_keyword_in_verses(quibble_program qp, char *keyword);
bool qb_find_keyword_in_stanzas(quibble_program qp, char *keyword);
bool qb_find_keyword_in_poems(quibble_program qp, char *keyword);
char *qb_create_prologue(char *config, char *name,
                         quibble_arg *args, int num_args,
                         quibble_kwarg *qkwargs, int num_kwargs);
void qb_build_program(quibble_program *qp);
void qb_rebuild_program(quibble_program *qp);
char *qb_expand_verse(quibble_program qp, char* verse_name, char *prologue);
char *qb_expand_stanza(quibble_program qp,
                       char* stanza_name, char *prologue, char *body);
char *qb_expand_poem(quibble_program qp, int index);

void qb_rebuild_program(quibble_program *qp);
void qb_build_program(quibble_program *qp);

char *qb_create_pixel_args(char *type, char *variable);
void qb_set_pixel_args(quibble_program *qp,
                       char *poem,
                       quibble_pixels qps,
                       char *variable);

#endif
