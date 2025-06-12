/*-------------io.h-----------------------------------------------------------//

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

/*----------------------------------------------------------------------------//
FILE IO WORK
//----------------------------------------------------------------------------*/

char *qb_strip_spaces(char *body, int start_index, int end_index){

    int start_offset = 0;
    int end_offset = 0;

    // Finding start offset
    char curr_char = body[start_index];
    while (curr_char == ' ' || curr_char == '\t' || curr_char == '\n'){
        ++start_offset;
        curr_char = body[start_index+start_offset];
    }

    // Finding end offset
    curr_char = body[end_index-1];
    while (curr_char == ' ' || curr_char == '\t' || curr_char == '\n'){
        ++end_offset;
        curr_char = body[end_index - 1 - end_offset];
    }

    int str_len = (end_index - end_offset) - (start_index + start_offset);
    char *final_str = (char *)calloc(str_len+1, sizeof(char));

    for (int i = 0; i < str_len; ++i){
        final_str[i] = body[start_index + start_offset + i];
    }

    final_str[str_len] = 0;

    return final_str;
}

int qb_find_next_char(char *body, int body_size, int current_index, char a){
    for (int i = current_index; i < body_size; ++i){
        if (body[i] == a){
            return i;
        }
    }

    return -1;
}

int qb_find_next_string(char *body, int body_size, int current_index,
                        char *a, int word_size){
    bool find_match = true;
    int count = 0;
    for (int i = current_index; i < body_size; ++i){
        while (find_match){
            // ending iteration early if strings are not equal
            if (body[i+count] != a[count]){
                find_match = false;
            }

            count++;

            // Exiting with `i` if we hit the end of the word with equal strings
            if (count == word_size && find_match){
                find_match = false;
                return i;
            }
        }

        find_match = true;
        count = 0;
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
    int query_length = strlen(query);
    int bodysize = strlen(body);
    int match_count = 0;
    int num_queries = 0;
    for (int i = 0; i < bodysize; ++i){
        if (body[i] == query[match_count]){
            ++match_count;
            if (match_count == query_length){
                ++num_queries;
                match_count = 0;
            }
        }
        else if (match_count != 0){
            match_count = 0;
        }
    }
    return num_queries;
}
