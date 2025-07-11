/*-------------io.c-----------------------------------------------------------//

Purpose: This file is meant to define most of what is needed for 
         image and video creation

//----------------------------------------------------------------------------*/

#include "../include/io.h"

quibble_pixel qb_zero_pixel(void){
    quibble_pixel qp;
    qp.red = 0;
    qp.green = 0;
    qp.blue = 0;
    qp.alpha = 0;
    return qp;
}

quibble_pixel *qb_create_pixel_array(int height, int width){
    quibble_pixel *qpa =
        (quibble_pixel *)malloc(sizeof(quibble_pixel)*width*height);

    for (int i = 0; i < width * height; ++i){
        qpa[i] = qb_zero_pixel();
    }
    return qpa;
}

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

char *qb_find_path(char *path){
    if (path[0] == 'Q' && path[1] == 'B'){
        char *ret = qb_config_file(path+3);
        free(path);
        return ret;
    }
    return path;
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
    strcat(ret, path);

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
