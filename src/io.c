/*-------------io.c-----------------------------------------------------------//

Purpose: This file is meant to define most of what is needed for 
         image and video creation

//----------------------------------------------------------------------------*/

#include "../include/io.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_PSD
#define STBI_NO_TGA
#define STBI_NO_GIF
#define STBI_NO_HDR
#define STBI_NO_PIC
#define STBI_NO_PNM 
#include "../extern/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../extern/stb_image_write.h"

/*----------------------------------------------------------------------------//
    IMAGE IO
//----------------------------------------------------------------------------*/

quibble_color qb_zero_color(void){
    quibble_color qp;
    qp.red = 0;
    qp.green = 0;
    qp.blue = 0;
    qp.alpha = 0;
    return qp;
}

unsigned char qb_color_clamp(float value,
                             float min_value,
                             float max_value){
    if (value < min_value){
        return 0;
    }
    else if (value > max_value){
        return 255;
    }
    else{
        return floor(255*((value - min_value)/(max_value - min_value)));
    }
}

quibble_color qb_color(float red, float green, float blue, float alpha){
    quibble_color qp;
    qp.red = qb_color_clamp(red, 0, 1);
    qp.green = qb_color_clamp(green, 0, 1);
    qp.blue = qb_color_clamp(blue, 0, 1);
    qp.alpha = qb_color_clamp(alpha, 0, 1);
    return qp;
}

int qb_get_color_size(int color_type){
    if (color_type == RGBA8888){
        return 4;
    }
    else if (color_type == RGB888){
        return 3;
    }
    else {
        fprintf(stderr, "Color type %d not found!", color_type);
        exit(1);
    }

}

quibble_pixels qb_create_blank_pixel_array(int width,
                                           int height,
                                           int color_type){
    quibble_pixels qps;
    qps.height = height;
    qps.width = width;

    qps.output_array = NULL;

    qps.color_type = color_type;
    qps.colors =
        (quibble_color *)malloc(sizeof(quibble_color)*width*height);

    for (int i = 0; i < width * height; ++i){
        qps.colors[i] = qb_zero_color();
    }

    return qps;
}


quibble_pixels qb_create_pixel_array_from_file(char *filename,
                                               int width,
                                               int height,
                                               int color_type){
    quibble_pixels qps = qb_create_blank_pixel_array(width, height, color_type);
    qps.output_array = qb_read_file(filename, width, height, color_type);
    qb_fill_colors_from_array(qps);
    return qps;
}

quibble_pixels qb_create_pixel_array(int width, int height, int color_type){
    quibble_pixels qps = qb_create_blank_pixel_array(width, height, color_type);

    int color_size = qb_get_color_size(color_type);

    qps.output_array =
        (unsigned char *)calloc(width*height*color_size,
                                sizeof(unsigned char *));
    return qps;
}

quibble_color qb_read_color_from_rgba8888_array(unsigned char *a, int i){
    quibble_color qc;
    qc.red = a[i*4 + 0];
    qc.green = a[i*4 + 1];
    qc.blue = a[i*4 + 2];
    qc.alpha = a[i*4 + 3];
    return qc;
}

quibble_color qb_read_color_from_rgb888_array(unsigned char *a, int i){
    quibble_color qc;
    qc.red = a[i*3 + 0];
    qc.green = a[i*3 + 1];
    qc.blue = a[i*3 + 2];
    qc.alpha = 255;
    return qc;
}

void qb_write_color_to_rgba8888_array(unsigned char *a,
                                      int i,
                                      quibble_color qc){
    a[i*4 + 0] = qc.red;
    a[i*4 + 1] = qc.green;
    a[i*4 + 2] = qc.blue;
    a[i*4 + 3] = qc.alpha;
}

void qb_write_color_to_rgb888_array(unsigned char *a, int i, quibble_color qc){
    a[i*3 + 0] = qc.red;
    a[i*3 + 1] = qc.green;
    a[i*3 + 2] = qc.blue;
}

void qb_fill_array_with_colors(quibble_pixels qps){

    int index;
    for (int i = 0; i < qps.height; ++i){
        for (int j = 0; j < qps.width; ++j){
            index = i*qps.width + j;
            if (qps.color_type == RGBA8888){
                qb_write_color_to_rgba8888_array(qps.output_array,
                                              index,
                                              qps.colors[index]);
            }
            else if (qps.color_type == RGB888){
                qb_write_color_to_rgb888_array(qps.output_array,
                                            index,
                                            qps.colors[index]);
            }
            else {
                fprintf(stderr, "Color type %d not found!", qps.color_type);
                exit(1);
            }
        }
    }

}

void qb_fill_colors_from_array(quibble_pixels qps){
    int index;
    for (int i = 0; i < qps.height; ++i){
        for (int j = 0; j < qps.width; ++j){
            index = i*qps.width + j;
            if (qps.color_type == RGBA8888){
                qps.colors[index] = 
                    qb_read_color_from_rgba8888_array(qps.output_array, index);
            }
            else if (qps.color_type == RGB888){
                qps.colors[index] = 
                    qb_read_color_from_rgb888_array(qps.output_array, index);
            }
            else {
                fprintf(stderr, "Color type %d not found!", qps.color_type);
                exit(1);
            }
        }
    }

}

/*----------------------------------------------------------------------------//
    FILE FORMATS
//----------------------------------------------------------------------------*/

char *qb_find_file_extension(char* filename){
    int len = strlen(filename);
    bool iterate = true;
    int index = len-1;

    while (iterate){
        if (filename[index] == '.'){
            iterate = false;
        }
        index -= 1;
        if (index < 0){
            iterate = false;
        }
    }

    if (index < 0){
        return NULL;
    }
    else {
        return &filename[index+2];
    }
}

unsigned char *qb_read_file(char *filename,
                            int width,
                            int height,
                            int color_type){
    int color_size = qb_get_color_size(color_type);

    unsigned char *data = stbi_load(filename, &width, &height, &color_size, 0);
    return data;

}

void qb_write_file(char *filename, quibble_pixels qps){

    char *file_extension = qb_find_file_extension(filename);

    if (file_extension == NULL){
        printf("Warning, no file extension found!\nDefaulting to BMP!\n");
        qb_write_bmp_file(filename, qps);
    }
    else if (strcmp(file_extension, "png") == 0 ||
             strcmp(file_extension, "PNG") == 0){
        qb_write_png_file(filename, qps);
    }
    else if (strcmp(file_extension, "jpg") == 0 ||
             strcmp(file_extension, "JPG") == 0 ||
             strcmp(file_extension, "jpeg") == 0 ||
             strcmp(file_extension, "JPEG") == 0){
        printf("Warning: no quality set for jpg file. Defaulting to 100!\n");
        printf("To set quality, run `qb_write_jpg_file(filename, pixels, quality)` instead!\n");
        qb_write_jpg_file(filename, qps, 100);
    }
    else if (strcmp(file_extension, "bmp") == 0 ||
             strcmp(file_extension, "BMP") == 0){
        qb_write_bmp_file(filename, qps);
    }
    else {
        printf("Warning, file extension %s is not supported!\n",
               file_extension);
        printf("Defaulting to BMP!\n");
        qb_write_bmp_file(filename, qps);
    }
}

void qb_write_png_file(char *filename, quibble_pixels qps){
    qb_fill_array_with_colors(qps);
    stbi_write_png(filename,
                   qps.width,
                   qps.height,
                   qps.color_type,
                   qps.output_array,
                   qps.width*qb_get_color_size(qps.color_type));
}

void qb_write_bmp_file(char *filename, quibble_pixels qps){
    qb_fill_array_with_colors(qps);
    stbi_write_bmp(filename,
                   qps.width,
                   qps.height,
                   qps.color_type,
                   qps.output_array);
}

void qb_write_jpg_file(char *filename, quibble_pixels qps, int quality){
    qb_fill_array_with_colors(qps);
    stbi_write_jpg(filename,
                   qps.width,
                   qps.height,
                   qps.color_type,
                   qps.output_array,
                   quality);
}

void qb_free_pixels(quibble_pixels qps){
    free(qps.colors);
    free(qps.output_array);
}

/*----------------------------------------------------------------------------//
    STRING MANIP
//----------------------------------------------------------------------------*/
size_t qb_find_type_size(char *type){

    if (type == NULL){
        return 4;
    }

    if (strcmp(type, "long double") == 0){
        return sizeof(long double);
    }

    if (qb_find_next_char(type, 0, '*') >= 0 ||
        qb_find_next_string(type, 0, "long long") >= 0 ||
        qb_find_next_string(type, 0, "double") >= 0 ||
        qb_find_next_string(type, 0, "cl_mem") >= 0){
        return 8;
    }

    if (qb_find_next_string(type, 0, "long") >= 0){
        return sizeof(long);
    }

    if (qb_find_next_string(type, 0, "short") >= 0){
        return 2;
    }

    if (qb_find_next_string(type, 0, "char") >= 0 ||
        qb_find_next_string(type, 0, "bool") >= 0){
        return 1;
    }

    if (qb_find_next_string(type, 0, "int") >= 0 ||
        qb_find_next_string(type, 0, "signed") >= 0 ||
        qb_find_next_string(type, 0, "unsigned") >= 0 ||
        qb_find_next_string(type, 0, "float") >= 0){
        return 4;
    }

    printf("Warning: type %s not found! Defaulting to int...\n", type);
    return 4;
}

char *qb_expand_path(char *path, char *base_path){
    if (path == NULL){
        return NULL;
    }

    char *ret = NULL;
    if (path[0] == 'Q' && path[1] == 'B'){
        char *scribble_path = "scribbles/";

        int total_length = strlen(scribble_path) + strlen(path+3) + 1;

        char *tmp_path = (char *)calloc(total_length, sizeof(char));

        strcat(tmp_path, scribble_path);
        strcat(tmp_path, path+3);

        ret = qb_config_file(tmp_path);
        free(tmp_path);
        return ret;
       
    }
    else {
        int total_length = strlen(path) + 1;
        if (base_path != NULL){
            total_length += strlen(base_path);
        }

        ret = (char *)calloc(total_length, sizeof(char));

        if (base_path != NULL){
            strcat(ret, base_path);
        }
        strcat(ret, path);
    }
    return ret;
}

char *qb_find_path(char *filename){
    if (filename == NULL){
        return NULL;
    }

    int idx = strlen(filename);
    bool found_slash = false;
    while (!found_slash && idx > 0){
        idx--;
        if (filename[idx] == '/'){
            found_slash = true;
        }
    }
    if(idx >= 0){
        return qb_strip_spaces(filename, 0, idx);
    }
    else {
        return NULL;
    }
}

char *qb_config_file(char *path){
    char *ret = (char *)calloc(256, sizeof(char));
    if (getenv("XDG_CONFIG_HOME") == NULL){
        strcat(ret, getenv("HOME"));
        strcat(ret, "/.config/quibble/");
    }
    else {
        strcat(ret, getenv("XDG_CONFIG_HOME"));
        strcat(ret, "/");
    }
    if (path != NULL){
        strcat(ret, path);
    }

    return ret;
}

/*----------------------------------------------------------------------------//
FILE IO WORK
//----------------------------------------------------------------------------*/

bool qb_is_space(char a){
    if (a == ' ' || a == '\t' || a == '\n'){
        return true;
    }
    return false;
}

// Necessary to convert QBINLINE **const** char * values to mutable char *'s
char *qb_copy(char *buffer){
    int n = strlen(buffer)+1;
    char *copy_buffer = (char *)calloc(n, sizeof(char));
    strcpy(copy_buffer, buffer);
    return copy_buffer;
}

char *qb_strip_spaces(char *body, int start_index, int end_index){

    char *final_str = NULL;
    char curr_char;
    if (start_index == end_index){
        curr_char = body[start_index];
        if (!qb_is_space(curr_char)){
            char *tmp_str = (char *)calloc(2, sizeof(char));
            tmp_str[0] = body[start_index];
            final_str = tmp_str;
            return final_str;
        }
        else {
            return final_str;
        }
    }

    int start_offset = 0;
    int end_offset = 0;

    // Finding start offset
    curr_char = body[start_index];
    while (qb_is_space(curr_char) &&
           start_offset + start_index < end_index){
        ++start_offset;
        curr_char = body[start_index+start_offset];
    }

    // Finding end offset
    curr_char = body[end_index];
    while (qb_is_space(curr_char) &&
           end_index - end_offset > start_index){
        ++end_offset;
        curr_char = body[end_index - end_offset];
    }

    int str_len = (end_index - end_offset) - (start_index + start_offset)+1;
    if (str_len > 0){
        final_str = (char *)calloc(str_len+1, sizeof(char));
        strncpy(final_str, &body[start_index+start_offset], str_len);
    }

    return final_str;
}

int qb_find_next_char(char *body, int current_index, char a){
    if (body == NULL){
        return -1;
    }

    char *ptr = strchr(&body[current_index], a);
    if (ptr != NULL){
        return ptr - body;
    }

    return -1;
}

int qb_find_next_string(char *body, int current_index, char *a){
    if (body == NULL){
        return -1;
    }

    char *ptr = strstr(&body[current_index], a);
    if (ptr != NULL){
        return ptr - body;
    }

    return -1;
}

int qb_find_matching_char(char *body, int body_size, int current_index,
                          char a, char b){

    // Checking initial character to make sure it is valid
    int open_count = -1;
    if (body[current_index] == a){
        open_count = 1;
    }
    else{
        return -1;
    }

    // Iterating through to find closing brackets
    int i = current_index;
    while (open_count > 0){
        ++i;
        if (body[i] == a){
            ++open_count;
        }
        if (body[i] == b){
            --open_count;
        }
        if (i == body_size){
            return -1;
        }
    }
    return i;
}

int qb_find_occurrences(char *query, char *body){


    char *temp_str = strstr(body, query);
    if (temp_str == NULL){
        return 0;
    }

    int count = 0;
    int query_length = strlen(query);

    while (temp_str != NULL){
        count++;
        temp_str += query_length;
        temp_str = strstr(temp_str, query);
    }

    return count;
}

/*----------------------------------------------------------------------------//
    QBINLINE
//----------------------------------------------------------------------------*/

// replaces a character a with b if after char *prologue
void qb_replace_char_if_proceeding(char *content, char *query, char a, char b){

    char *substr = strstr(content, query);
    if (substr != NULL){
        qb_replace_next_char(substr, 0, a, b);
    }
}

void qb_replace_char(char *verse, int verse_size, char a, char b){
    
    for (int i = 0; i < verse_size; ++i){
        if (verse[i] == a){
            verse[i] = b;
        }
    }
}

void qb_replace_next_char(char *content, int index, char a, char b){
    char *tmp = strchr(&content[index], a);
    content[tmp-content] = b;
}

void qb_preprocess_content(char *content){
    if (qb_is_inlined(content)){
        int content_size = strlen(content);
        // replace all spaces by new lines
        qb_replace_char(content, content_size, ' ', '\n');

        // except for the arguments after some preprocessor options
        // that need to be in the same line
        qb_replace_char_if_proceeding(content, "#ifdef", '\n', ' ');
        qb_replace_char_if_proceeding(content, "#ifndef", '\n', ' ');

        // #define with two arguments will not work
        qb_replace_char_if_proceeding(content, "#define", '\n', ' ');

        // don't leave any spaces in arguments
        qb_replace_char_if_proceeding(content, "#if", '\n', ' ');

        // don't leave any spaces in arguments
        qb_replace_char_if_proceeding(content, "#elif", '\n', ' ');
        qb_replace_char_if_proceeding(content, "#pragma", '\n', ' ');
    }
}

bool qb_is_inlined(char *verse){
    char substr[23] = "// QBINLINE GENERATED\n";
    for (int i = 0; i < 22; ++i){
        if (verse[i] != substr[i]){
            return false;
        }
    }

    return true;
}
