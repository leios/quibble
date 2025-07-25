/*-------------program--------------------------------------------------------//

 Purpose: A quibble program is a user-submitted set of verses, stanzas, poems,
          and functions (usually from a *.qbl file)

   Notes: Some unnecessary code duplication here, but... meh
          Make sure to allocate NULL when num_anything == 0

//----------------------------------------------------------------------------*/

#include "../include/quibble.h"

/*----------------------------------------------------------------------------//
    PROGRAM GENERATION
//----------------------------------------------------------------------------*/
// Reads an input file and parses everything into verses or OCL functions
quibble_program qb_parse_program_file(char *filename){

    // Opening file
    FILE* fileptr;
    fileptr = fopen(filename, "r");

    if (fileptr == NULL){
        fprintf(stderr, "Error opening file %s!\n", filename);
        exit(1);
    }

    // Finding file length
    fseek(fileptr, 0, SEEK_END);
    int filesize = ftell(fileptr);
    fseek(fileptr, 0, SEEK_SET);

    // Creating initial buffer
    char *buffer = (char *)calloc(filesize+1, sizeof(char));

    while (!feof(fileptr)){
        fread(buffer, sizeof(char), filesize, fileptr);
    }

    fclose(fileptr);
    char *path = qb_find_path(filename);
    quibble_program qp = qb_parse_program(buffer, path);
    free(path);
    free(buffer);

    return qp;
}

quibble_program qb_parse_program(char *program, char *path){

    int num_includes = qb_find_occurrences("@include", program);
    int num_verses = qb_find_occurrences("__verse", program);
    int num_stanzas = qb_find_occurrences("__stanza", program);
    int num_poems = qb_find_occurrences("__poem", program);

    int filesize = strlen(program);

    // creating program and populating verses and everything else
    quibble_program qp;
    qp.configured = false;

    qp.num_verses = num_verses;
    qp.num_stanzas = num_stanzas;
    qp.num_poems = num_poems;

    quibble_program *other_programs = NULL;
    if (num_includes > 0){
       other_programs =
           (quibble_program *)malloc(sizeof(quibble_program)*num_includes);
    }
    else {
        qp.verse_list = NULL;
    }

    if (num_verses > 0){
        qp.verse_list =
           (quibble_verse *)malloc(sizeof(quibble_verse)*num_verses);
    }
    else {
        qp.verse_list = NULL;
    }

    if (num_stanzas > 0){
        qp.stanza_list =
            (quibble_stanza *)malloc(sizeof(quibble_stanza)*num_stanzas);
    }
    else {
        qp.stanza_list = NULL;
    }

    if (num_poems > 0){
        qp.poem_list = (quibble_poem *)malloc(sizeof(quibble_poem)*num_poems);
    }
    else {
        qp.poem_list = NULL;
    }

    bool qbinlined = qb_is_inlined(program);
    char *buffer;
    if (qbinlined){
        buffer = qb_copy(&program[22]);
        qb_preprocess_content(buffer);
        filesize -= 22;
    }
    else {
        buffer = qb_copy(program);
    }

    if (num_includes > 0 ||
        num_verses > 0   ||
        num_stanzas > 0  ||
        num_poems > 0){
        char *tmp_everything_else =
            (char *)calloc(QB_MAX_SOURCE_SIZE, sizeof(char));

        char *short_path;
        char *full_path;

        char *tmp_verse = NULL;
        if (num_verses > 0){
            tmp_verse = (char *)calloc(QB_MAX_SOURCE_SIZE, sizeof(char));
        }

        char *tmp_poem = NULL;
        if (num_poems > 0){
            tmp_poem = (char *)calloc(QB_MAX_SOURCE_SIZE, sizeof(char));
        }

        char *tmp_stanza = NULL;
        if (num_stanzas > 0){
            tmp_stanza = (char *)calloc(QB_MAX_SOURCE_SIZE, sizeof(char));
        }

        int index = 0;
        int everything_else_index = 0;

        int include_match_count = 0;
        int verse_match_count = 0;
        int stanza_match_count = 0;
        int poem_match_count = 0;

        int curr_include = 0;
        int curr_verse = 0;
        int curr_poem = 0;
        int curr_stanza = 0;

        int include_start = 0;
        int verse_start = 0;
        int poem_start = 0;
        int stanza_start = 0;

        int include_end = 0;
        int verse_end = 0;
        int poem_end = 0;
        int stanza_end = 0;

        char *include_string = "@include";
        char *verse_string = "__verse";
        char *stanza_string = "__stanza";
        char *poem_string = "__poem";

        while (index < filesize){
            if (buffer[index] == include_string[include_match_count] &&
                curr_include < num_includes){
                ++include_match_count;
                if (include_match_count == 8){
                    include_start =
                        qb_find_next_char(buffer, index, '"');
                    include_end =
                        qb_find_next_char(buffer, include_start+1, '"');

                    // strips " on either side
                    include_start++;
                    include_end--;

                    short_path =
                        qb_strip_spaces(buffer, include_start, include_end);
                    full_path = qb_expand_path(short_path, path);

                    other_programs[curr_include] =
                        qb_parse_program_file(full_path);

                    everything_else_index -= 7;

                    memset(tmp_everything_else+everything_else_index, 0,
                           7*sizeof(char));

                    if (other_programs[curr_include].everything_else != NULL){
                        everything_else_index +=
                            strlen(other_programs[curr_include].everything_else);

                        strcat(tmp_everything_else,
                               other_programs[curr_include].everything_else);
                    }

                    ++curr_include;

                    // setting everything back
                    index = qb_find_next_char(buffer, include_end, '\n');
                    include_match_count = 0;
                    free(full_path);
                    free(short_path);
                    include_end = 0;
                    include_start = 0;
                }
            }
            else if (include_match_count != 0){
                include_match_count = 0;
            }

            if (buffer[index] == poem_string[poem_match_count] &&
                curr_poem < num_poems){
                ++poem_match_count;
                if (poem_match_count == 6){
                    poem_start = index - 5;
                    poem_end = qb_find_next_char(buffer, poem_start, '(');
                    poem_end = qb_find_matching_char(
                        buffer, filesize, poem_end, '(', ')');

                    poem_end = qb_find_next_char(buffer, poem_end, '{');
                    poem_end = qb_find_matching_char(
                        buffer, filesize, poem_end, '{', '}') + 1;

                    for (int j = 0; j < poem_end - poem_start; ++j){
                        tmp_poem[j] = buffer[poem_start + j];
                    }

                    qp.poem_list[curr_poem] = qb_parse_poem(tmp_poem);
                    ++curr_poem;

                    // setting everything back
                    index = poem_end;
                    poem_match_count = 0;
                    everything_else_index -= 5;
                    memset(tmp_everything_else+everything_else_index, 0,
                           5*sizeof(char));
                    poem_end = 0;
                    poem_start = 0;
                    memset(tmp_poem,0,strlen(tmp_poem));
                }
            }
            else if (poem_match_count != 0){
                poem_match_count = 0;
            }

            if (buffer[index] == stanza_string[stanza_match_count] &&
                curr_stanza < num_stanzas){
                ++stanza_match_count;
                if (stanza_match_count == 8){
                    stanza_start = index - 7;
                    stanza_end = qb_find_next_char(buffer, stanza_start, '(');
                    stanza_end = qb_find_matching_char(
                        buffer, filesize, stanza_end, '(', ')');

                    stanza_end = qb_find_next_char(buffer, stanza_end, '{');
                    stanza_end = qb_find_matching_char(
                        buffer, filesize, stanza_end, '{', '}') + 1;

                    for (int j = 0; j < stanza_end - stanza_start; ++j){
                        tmp_stanza[j] = buffer[stanza_start + j];
                    }

                    qp.stanza_list[curr_stanza] = qb_parse_stanza(tmp_stanza);
                    ++curr_stanza;

                    // setting everything back
                    index = stanza_end;
                    stanza_match_count = 0;
                    everything_else_index -= 7;
                    memset(tmp_everything_else+everything_else_index, 0,
                           7*sizeof(char));
                    stanza_end = 0;
                    stanza_start = 0;
                    memset(tmp_stanza,0,strlen(tmp_stanza));
                }
            }
            else if (stanza_match_count != 0){
                stanza_match_count = 0;
            }

            if (buffer[index] == verse_string[verse_match_count] &&
                curr_verse < num_verses){
                ++verse_match_count;
                if (verse_match_count == 7){
                    verse_start = index - 6;
                    verse_end = qb_find_next_char(buffer, verse_start, '(');
                    verse_end = qb_find_matching_char(
                        buffer, filesize, verse_end, '(', ')');

                    verse_end = qb_find_next_char(buffer, verse_end, '{');
                    verse_end = qb_find_matching_char(
                        buffer, filesize, verse_end, '{', '}') + 1;

                    for (int j = 0; j < verse_end - verse_start; ++j){
                        tmp_verse[j] = buffer[verse_start + j];
                    }

                    qp.verse_list[curr_verse] = qb_parse_verse(tmp_verse);
                    ++curr_verse;

                    // setting everything back
                    index = verse_end;
                    verse_match_count = 0;
                    everything_else_index -= 6;
                    memset(tmp_everything_else+everything_else_index, 0,
                           6*sizeof(char));
                    verse_end = 0;
                    verse_start = 0;
                    memset(tmp_verse,0,strlen(tmp_verse));
                }
            }
            else if (verse_match_count != 0){
                verse_match_count = 0;
            }

            tmp_everything_else[everything_else_index] = buffer[index];
            ++index;
            ++everything_else_index;
        }

        char *everything_else =
           qb_strip_spaces(tmp_everything_else, 
                           0, strlen(tmp_everything_else)-1);

        qp.everything_else = everything_else;

        // Only free if we are not using it directly
        free(buffer);
        free(tmp_everything_else);
        free(tmp_verse);
        free(tmp_stanza);
        free(tmp_poem);
    }
    else {
        qp.everything_else = buffer;
    }

    if (num_includes <= 0){
        qb_build_program(&qp);
        return qp;
    }
    else {
        quibble_program tmp_qp =
            qb_combine_program_array(other_programs, num_includes);

        quibble_program final_qp = qb_combine_programs(tmp_qp, qp);

        final_qp.everything_else = qp.everything_else;
        qp.everything_else = NULL;

        qb_build_program(&final_qp);

        qp.body = NULL;
        qb_shallow_free_program(qp);
        qb_shallow_free_program(tmp_qp);
        for (int i = 0; i < num_includes; ++i){
            qb_shallow_free_program(other_programs[i]);
        }
        free(other_programs);

        return(final_qp);
    }
}

quibble_verse qb_find_verse(quibble_program qp, char *verse_name){

    for (int i = 0; i < qp.num_verses; ++i){
        if (strcmp(qp.verse_list[i].name, verse_name) == 0){
            return qp.verse_list[i];
        }
    }
    fprintf(stderr, "No verse %s found!\n", verse_name);
    exit(1);
}

quibble_stanza qb_find_stanza(quibble_program qp, char *stanza_name){

    for (int i = 0; i < qp.num_stanzas; ++i){
        if (strcmp(qp.stanza_list[i].name, stanza_name) == 0){
            return qp.stanza_list[i];
        }
    }
    fprintf(stderr, "No stanza %s found!\n", stanza_name);
    exit(1);
}

quibble_poem qb_find_poem(quibble_program qp, char *poem_name){

    for (int i = 0; i < qp.num_poems; ++i){
        if (strcmp(qp.poem_list[i].name, poem_name) == 0){
            return qp.poem_list[i];
        }
    }
    fprintf(stderr, "No poem %s found!\n", poem_name);
    exit(1);
}

quibble_program qb_combine_programs(quibble_program qp_1, quibble_program qp_2){

    quibble_program qp;

    qp.configured = false;
    qp.everything_else = NULL;
    qp.body = NULL;

    qp.num_verses = qp_1.num_verses + qp_2.num_verses;
    qp.num_stanzas = qp_1.num_stanzas + qp_2.num_stanzas;
    qp.num_poems = qp_1.num_poems + qp_2.num_poems;


    if (qp.num_verses > 0){
        qp.verse_list =
            (quibble_verse *)malloc(sizeof(quibble_verse) * qp.num_verses);
    }
    else {
        qp.verse_list = NULL;
    }

    if (qp.num_stanzas > 0){
        qp.stanza_list =
            (quibble_stanza *)malloc(sizeof(quibble_stanza) * qp.num_stanzas);
    }
    else {
        qp.stanza_list = NULL;
    }

    if (qp.num_poems > 0){
        qp.poem_list =
            (quibble_poem *)malloc(sizeof(quibble_poem) * qp.num_poems);
    }
    else {
        qp.poem_list = NULL;
    }

    for (int i = 0; i < qp_1.num_verses; ++i){
        qp.verse_list[i] = qp_1.verse_list[i];
    }
    for (int i = 0; i < qp_2.num_verses; ++i){
        qp.verse_list[i+qp_1.num_verses] = qp_2.verse_list[i];
    }

    for (int i = 0; i < qp_1.num_stanzas; ++i){
        qp.stanza_list[i] = qp_1.stanza_list[i];
    }
    for (int i = 0; i < qp_2.num_stanzas; ++i){
        qp.stanza_list[i+qp_1.num_stanzas] = qp_2.stanza_list[i];
    }

    for (int i = 0; i < qp_1.num_poems; ++i){
        qp.poem_list[i] = qp_1.poem_list[i];
    }
    for (int i = 0; i < qp_2.num_poems; ++i){
        qp.poem_list[i+qp_1.num_poems] = qp_2.poem_list[i];
    }

    return qp;
    
}
quibble_program qb_combine_program_array(quibble_program *qps, int n){
    quibble_program qp;

    qp.configured = false;
    qp.everything_else = NULL;
    qp.body = NULL;

    qp.num_verses = 0;
    qp.num_stanzas = 0;
    qp.num_poems = 0;

    for (int i = 0; i < n; ++i){
        qp.num_verses += qps[i].num_verses;
        qp.num_stanzas += qps[i].num_stanzas;
        qp.num_poems += qps[i].num_poems;
    }

    qp.verse_list =
        (quibble_verse *)malloc(sizeof(quibble_verse) * qp.num_verses);
    qp.stanza_list =
        (quibble_stanza *)malloc(sizeof(quibble_stanza) * qp.num_stanzas);
    qp.poem_list =
        (quibble_poem *)malloc(sizeof(quibble_poem) * qp.num_poems);

    int verse_count = 0;
    int stanza_count = 0;
    int poem_count = 0;

    for (int i = 0; i < n; ++i){
        for (int j = 0; j < qps[i].num_verses; ++j){
            qp.verse_list[j+verse_count] = qps[i].verse_list[j];
        }
        verse_count += qps[i].num_verses;

        for (int j = 0; j < qps[i].num_stanzas; ++j){
            qp.stanza_list[j+stanza_count] = qps[i].stanza_list[j];
        }
        stanza_count += qps[i].num_stanzas;

        for (int j = 0; j < qps[i].num_poems; ++j){
            qp.poem_list[j+poem_count] = qps[i].poem_list[j];
        }
        poem_count += qps[i].num_poems;

    }

    return qp;

}

/*----------------------------------------------------------------------------//
CONFIGURATION
//----------------------------------------------------------------------------*/

int qb_find_verse_index(quibble_program qp, char *name){
    for (int i = 0; i < qp.num_verses; ++i){
        if (strcmp(qp.verse_list[i].name, name) == 0){
            return i;
        }
    }
    fprintf(stderr, "Verse %s not found!\n", name);
    exit(1);
}

int qb_find_poem_index(quibble_program qp, char *name){
    for (int i = 0; i < qp.num_poems; ++i){
        if (strcmp(qp.poem_list[i].name, name) == 0){
            return i;
        }
    }
    fprintf(stderr, "Poem %s not found!\n", name);
    exit(1);
}

int qb_find_stanza_index(quibble_program qp, char *name){
    for (int i = 0; i < qp.num_stanzas; ++i){
        if (strcmp(qp.stanza_list[i].name, name) == 0){
            return i;
        }
    }
    fprintf(stderr, "Stanza %s not found!\n", name);
    exit(1);
}

void qb_shallow_free_program(quibble_program qp){

    free(qp.everything_else);
    free(qp.body);
    free(qp.verse_list);
    free(qp.stanza_list);
    free(qp.poem_list);


    // OpenCL freeing
    if (qp.configured){
        free(qp.platform_ids);
        free(qp.device_ids);
        cl_check(clFlush(qp.command_queue));
        cl_check(clFinish(qp.command_queue));
        cl_check(clReleaseCommandQueue(qp.command_queue));
        cl_check(clReleaseContext(qp.context));
        cl_check(clReleaseProgram(qp.program));

        for (int i = 0; i < qp.num_poems; ++i){
            cl_check(clReleaseKernel(qp.kernels[i]));
        }
        free(qp.kernels);
    }

}

void qb_free_program(quibble_program qp){
    for (int i = 0; i < qp.num_verses; ++i){
        qb_free_verse(qp.verse_list[i]);
    }
    for (int i = 0; i < qp.num_stanzas; ++i){
        qb_free_stanza(qp.stanza_list[i]);
    }
    for (int i = 0; i < qp.num_poems; ++i){
        qb_free_poem(qp.poem_list[i]);
    }
    qb_shallow_free_program(qp);
}

void qb_print_program(quibble_program qp){

    printf("Quibble Program:\nBody\n%s\nEverything Else:\n%s\n\n",
           qp.body, qp.everything_else);

    for (int i = 0; i < qp.num_poems; ++i){
        qb_print_poem(qp.poem_list[i]);
    }

    for (int i = 0; i < qp.num_stanzas; ++i){
        qb_print_stanza(qp.stanza_list[i]);
    }

    for (int i = 0; i < qp.num_verses; ++i){
        qb_print_verse(qp.verse_list[i]);
    }

    if (qp.configured){
        print_cl_info(qp);
    }
}
