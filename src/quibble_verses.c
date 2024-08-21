/*-------------quibble_verses.c-----------------------------------------------//

Purpose: A quibble_verse is a user-submitted code fragment that will later be
         transformed into a working OpenCL kernel

//----------------------------------------------------------------------------*/

#include "../include/quibble_verses.h"

/*----------------------------------------------------------------------------//
FILE IO WORK
//----------------------------------------------------------------------------*/

char *qb_strip_spaces(char *verse, int start_index, int end_index){
    int start_offset = 0;
    int end_offset = 0;

    // Finding start offset
    char curr_char = verse[start_index];
    while (curr_char == ' ' || curr_char == '\t' || curr_char == '\n'){
        ++start_offset;
        curr_char = verse[start_index+start_offset];
    }

    // Finding end offset
    curr_char = verse[end_index - 1];
    while (curr_char == ' ' || curr_char == '\t' || curr_char == '\n'){
        ++end_offset;
        curr_char = verse[end_index - 1 - end_offset];
    }

    int str_len = (end_index - end_offset) - (start_index + start_offset);
    char *final_str = (char *)malloc(sizeof(char) * str_len);

    for (int i = 0; i < str_len; ++i){
        final_str[i] = verse[start_index + start_offset + i];
    }

    return final_str;
}

int qb_find_next_char(char *verse, int verse_size, int current_index, char a){
    for (int i = current_index; i < verse_size; ++i){
        if (verse[i] == a){
            return i;
        }
    }

    return -1;
}

int qb_find_next_string(char *verse, int verse_size, int current_index,
                        char *a, int word_size){
    bool find_match = true;
    int count = 0;
    for (int i = current_index; i < verse_size; ++i){
        while (find_match){
            // ending iteration early if strings are not equal
            if (verse[i+count] != a[count]){
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

int qb_find_matching_char(char *verse, int verse_size, int current_index,
                          char a, char b){

    // Checking initial character to make sure it is valid
    int open_count = -1;
    if (verse[current_index] == a){
        open_count = 1;
    }
    else{
        return -1;
    }

    // Iterating through to find closing brackets
    int i = current_index;
    while (open_count > 0){
        ++i;
        if (verse[i] == a){
            ++open_count;
        }
        if (verse[i] == b){
            --open_count;
        }
        if (i == verse_size){
            return -1;
        }
    }
    return i;
}

// Reads an input file and parses everything into verses or OCL functions
quibble_program qb_create_program(char *filename){
}

quibble_verse qb_find_verse(quibble_program qp, char *verse_name){
}

quibble_verse qb_parse_verse(char *verse){
}

int qb_find_number_of_kwargs(char *preamble){

    int preamble_size = strlen(preamble);
    int i = 0;
    int num_entries = 0;
    int next_equal = 0;
    int next_semicolon = 0;

    while (i < preamble_size){
        next_equal = qb_find_next_char(preamble, preamble_size, i, '=');
        next_semicolon = qb_find_next_char(preamble, preamble_size, i, ';');

        if (next_equal > i && next_semicolon > next_equal){
            ++num_entries;
            i += next_semicolon + 1;
        }
        else {
            fprintf(stderr, "Keyword(s) not able to be parsed!\n");
            exit(1);
        }
    }

    return num_entries;

}

quibble_keyword *qb_parse_keywords(char *preamble){

    int preamble_size = strlen(preamble);

    int num_entries = qb_find_number_of_kwargs(preamble);

    quibble_keyword *final_keywords =
         (quibble_keyword *)malloc(sizeof(quibble_keyword) * num_entries);

    int i = 0;
    int curr_entry = 0;
    int next_equal = 0;
    int next_semicolon = 0;

    while (i < preamble_size){
        next_equal = qb_find_next_char(preamble, preamble_size, i, '=');
        next_semicolon = qb_find_next_char(preamble, preamble_size, i, ';');

        final_keywords[curr_entry].variable =
            qb_strip_spaces(preamble, i, next_equal);

        final_keywords[curr_entry].value =
            qb_strip_spaces(preamble, next_equal+1, next_semicolon);

        // Check to make sure entry is unique
        for (int j = 1; j <= curr_entry; ++j){
            if (strcmp(final_keywords[j-1].variable,
                       final_keywords[j].variable) == 0){
                fprintf(stderr, "Variable %s used more than once for keyword arguments!\n", final_keywords[j]);
                exit(1);
            }
        }

        i += next_semicolon + 1;
        ++curr_entry;

    }

    return final_keywords;

}

/*----------------------------------------------------------------------------//
DCOMPILE INTERFACE
//----------------------------------------------------------------------------*/

void qb_replace_char(char *verse, int verse_size, char a, char b){
    for (int i = 0; i < verse_size; ++i){
        if (verse[i] == a){
            verse[i] = b;
        }
    }
}

void qb_replace_char_if_proceeding(char *verse, int verse_size,
                                   char *preamble, int preamble_size,
                                   char a, char b){

    bool find_match = true;
    bool match_found = false;
    int count = 0;
    for (int i = 0; i < verse_size; ++i){
        while (find_match){
            if (verse[i+count] != preamble[count]){
                find_match = false;
            }

            count++;

            if (count == preamble_size && find_match){
                find_match = false;
                match_found = true;
            }
        }

        // match found
        if (match_found &&
            verse[i+count] == a){
            verse[i+count] = b;
            match_found = false;
        }
        find_match = true;
        count = 0;
    }
}

bool qb_is_dcompiled(char *verse){
    char substr[22] = "// DCOMPILE GENERATED\n";
    for (int i = 0; i < 22; ++i){
        if (verse[i] != substr[i]){
            return false;
        }
    }

    return true;
}

void qb_preprocess_verse(char *verse){
    if (qb_is_dcompiled(verse)){
        int verse_size = strlen(verse);
        // replace all spaces by new lines
        qb_replace_char(verse, verse_size, ' ', '\n');

        // except for the arguments after some preprocessor options
        // that need to be in the same line
        qb_replace_char_if_proceeding(verse, verse_size,
                                      "#ifdef", 7, '\n', ' ');
        qb_replace_char_if_proceeding(verse, verse_size,
                                      "#ifndef", 8, '\n', ' ');

        // #define with two arguments will not work
        qb_replace_char_if_proceeding(verse, verse_size,
                                      "#define", 7, '\n', ' ');

        // don't leave any spaces in arguments
        qb_replace_char_if_proceeding(verse, verse_size, "#if", 3, '\n', ' ');

        // don't leave any spaces in arguments
        qb_replace_char_if_proceeding(verse, verse_size, "#elif", 5, '\n', ' ');
        qb_replace_char_if_proceeding(verse, verse_size, "#pragma",
                                      7, '\n', ' ');
    }
}

/*----------------------------------------------------------------------------//
CONFIGURATION
//----------------------------------------------------------------------------*/

// Configures preambles of existing verses
void qb_configure_verse(quibble_verse *qv, ...){
}

// An echo is a verse with the same body, but different preamble
quibble_verse qb_echo_verse(quibble_verse qv, ...){
}

// To be used in `qb_configure_verse` to create preamble string
char *qb_create_preamble(quibble_keyword *qkwargs, int num_kwargs){

    char temp[MAX_PREAMBLE_SIZE];
    memset(temp,0,strlen(temp));
    for (int i = 0; i < num_kwargs; ++i){
        strcat(temp, qkwargs[i].variable);
        strcat(temp, " = ");
        strcat(temp, qkwargs[i].value);
        strcat(temp, ";\n");
    }

    int len = strlen(temp);
    char *final_output = (char *)malloc(sizeof(char)*len);

    for (int i = 0; i < len; ++i){
        final_output[i] = temp[i];
    }

    return final_output;
}

/*----------------------------------------------------------------------------//
FREE
//----------------------------------------------------------------------------*/

void qb_free_keyword(quibble_keyword qkwarg){
    free(qkwarg.variable);
    free(qkwarg.value);
}

void qb_free_keyword_array(quibble_keyword *qkwargs, int n){
    for (int i = 0; i < n; ++i){
        qb_free_keyword(qkwargs[i]);
    }
}

void qb_free_verse(quibble_verse qv){
    if (qv.echo == false){
        free(qv.body);
    }
    qb_free_keyword_array(qv.kwargs, qv.num_kwargs);
}
