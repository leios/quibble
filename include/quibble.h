#ifndef QUIBBLE_H
#define QUIBBLE_H

/*----------------------------------------------------------------------------//
    MACROS
//----------------------------------------------------------------------------*/
// These numbers are arbitrarily set
#define QB_MAX_SOURCE_SIZE (0x999999)
#define QB_MAX_PROLOGUE_SIZE (0x100000)
#define QB_MAX_META_SIZE (0x100000)

// Delay Compilation
#define QBINLINE(...) "// QBINLINE GENERATED\n" #__VA_ARGS__ "\0"

// QBT -> Quibble Text
#define QBT_RED     "\x1b[31m"
#define QBT_GREEN   "\x1b[32m"
#define QBT_YELLOW  "\x1b[33m"
#define QBT_BLUE    "\x1b[34m"
#define QBT_MAGENTA "\x1b[35m"
#define QBT_CYAN    "\x1b[36m"
#define QBT_RESET   "\x1b[0m"

// Color Types
#define PRGBA8888 2
#define PRGB888 1

#define RGBA8888 4
#define RGB888 3

/*----------------------------------------------------------------------------//
    INCLUDES
//----------------------------------------------------------------------------*/
#define CL_TARGET_OPENCL_VERSION 300
#include <CL/cl.h>

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>


/*----------------------------------------------------------------------------//
    STRUCT DEFINITION
//----------------------------------------------------------------------------*/
typedef struct{
    char *type;
    char *variable;
} quibble_arg;

typedef struct{
    char *type;
    char *variable;
    char *value;
} quibble_kwarg;

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
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char priority;
} quibble_pcolor_rgb888;

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
    unsigned char priority;
} quibble_pcolor_rgba8888;

typedef struct {
    float x;
} quibble_point_1D;

typedef struct {
    float x;
    float y;
} quibble_point_2D;

typedef struct {
    float x;
    float y;
    float z;
} quibble_point_3D;

typedef struct {
    void *host_data;
    int color_type;
    int height;
    int width;

    unsigned char *output_array;

    // CL stuff
    cl_mem device_data;
    cl_command_queue command_queue;
} quibble_pixels;

typedef struct {
    float ppu;
    float world_size_x;
    float world_size_y;
    float world_position_x;
    float world_position_y;
} quibble_simple_camera;

/*----------------------------------------------------------------------------//
    ERROR HANDLING
//----------------------------------------------------------------------------*/
void qb_check(bool value, char *err_string);
void cl_check(cl_int error);

const char *get_cl_error_string(cl_int error);
void cl_check_program(cl_int error, cl_program program, cl_device_id device_id);


/*----------------------------------------------------------------------------//
    PROGRAM / STANZA / POEM / VERSES
//----------------------------------------------------------------------------*/
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
void qb_output_program_to_file(quibble_program qp, char *filename);
void print_cl_info(quibble_program qp);


/*----------------------------------------------------------------------------//
    ARGS / KWARGS
//----------------------------------------------------------------------------*/
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


/*----------------------------------------------------------------------------//
    STRING MANIP
//----------------------------------------------------------------------------*/
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
/*----------------------------------------------------------------------------//
    POINTS
//----------------------------------------------------------------------------*/
quibble_point_1D qb_point_1D(float x);
quibble_point_2D qb_point_2D(float x, float y);
quibble_point_3D qb_point_3D(float x, float y, float z);

/*----------------------------------------------------------------------------//
    IMAGES
//----------------------------------------------------------------------------*/
unsigned char qb_color_clamp(float value,
                             float min_value,
                             float max_value);
quibble_color_rgba8888 qb_zero_color_rgba8888(void);
quibble_color_rgb888 qb_zero_color_rgb888(void);

quibble_pcolor_rgba8888 qb_zero_pcolor_rgba8888(void);
quibble_pcolor_rgb888 qb_zero_pcolor_rgb888(void);

quibble_color_rgba8888 qb_color_rgba8888(float red,
                                         float green,
                                         float blue,
                                         float alpha);
quibble_color_rgb888 qb_color_rgb888(float red, float green, float blue);

quibble_pcolor_rgba8888 qb_pcolor_rgba8888(float red,
                                           float green,
                                           float blue,
                                           float alpha,
                                           float priority);

quibble_pcolor_rgb888 qb_pcolor_rgb888(float red,
                                       float green,
                                       float blue,
                                       float priority);

quibble_pixels qb_create_pixel_array(quibble_program qp,
                                     int width,
                                     int height,
                                     int color_type);
quibble_pixels qb_create_pixel_array_from_file(char *filename,
                                               quibble_program qp,
                                               int width,
                                               int height,
                                               int color_type);

int qb_get_color_size(int color_type);

void qb_pixels_device_to_host(quibble_pixels qps);
void qb_pixels_host_to_device(quibble_pixels qps);

// File Formats

void qb_deprioritize_array(quibble_pixels qps);

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

/*----------------------------------------------------------------------------//
    CAMERA
//----------------------------------------------------------------------------*/

quibble_simple_camera qb_create_simple_camera(float ppu,
                                              float world_size_x,
                                              float world_size_y,
                                              float world_position_x,
                                              float world_position_y);

int qb_find_camera_width(quibble_simple_camera qcam);
int qb_find_camera_height(quibble_simple_camera qcam);

bool qb_pixels_cam_consistent(quibble_pixels qpix, quibble_simple_camera qcam);

int qb_find_bin(float x, float y, quibble_simple_camera qcam);

/*----------------------------------------------------------------------------//
    CONFIG
//----------------------------------------------------------------------------*/
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
size_t qb_find_type_size(char *type);
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
