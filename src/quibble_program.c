/*-------------program--------------------------------------------------------//

 Purpose: A quibble program is a user-submitted set of verses, stanzas, poems,
          and functions (usually from a *.qbl file)

   Notes: Some unnecessary code duplication here, but... meh
          Make sure to allocate NULL when num_anything == 0

    TODO: qp_parse_program should also build!!!

//----------------------------------------------------------------------------*/

#include "../include/quibble_program.h"

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
    quibble_program qp = qb_parse_program(buffer);

    // Setting default to 0,0
    qb_find_platforms(&qp);
    qb_find_devices(&qp);

    qp.configured = false;

    return qp;
}

quibble_program qb_parse_program(char *program){

    int num_verses = qb_find_occurrences("__verse", program);
    int num_stanzas = qb_find_occurrences("__stanza", program);
    int num_poems = qb_find_occurrences("__poem", program);

    int filesize = strlen(program);

    // creating program and populating verses and everything else
    quibble_program qp;

    qp.num_verses = num_verses;
    qp.num_stanzas = num_stanzas;
    qp.num_poems = num_poems;

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

    if (num_verses > 0 ||
        num_stanzas > 0 ||
        num_poems > 0){
        char *tmp_everything_else =
            (char *)calloc(MAX_SOURCE_SIZE, sizeof(char));

        char *tmp_verse = NULL;
        if (num_verses > 0){
            tmp_verse = (char *)calloc(MAX_SOURCE_SIZE, sizeof(char));
        }

        char *tmp_poem = NULL;
        if (num_poems > 0){
            tmp_poem = (char *)calloc(MAX_SOURCE_SIZE, sizeof(char));
        }

        char *tmp_stanza = NULL;
        if (num_stanzas > 0){
            tmp_stanza = (char *)calloc(MAX_SOURCE_SIZE, sizeof(char));
        }

        int index = 0;
        int everything_else_index = 0;

        bool qbinlined = qb_is_inlined(program);
        char *buffer = program;
        if (qbinlined){
            index += 22;
            buffer = qb_copy(buffer);
            qb_preprocess_content(buffer);
        }

        int verse_match_count = 0;
        int stanza_match_count = 0;
        int poem_match_count = 0;

        int curr_verse = 0;
        int curr_poem = 0;
        int curr_stanza = 0;

        int verse_start = 0;
        int poem_start = 0;
        int stanza_start = 0;

        int verse_end = 0;
        int poem_end = 0;
        int stanza_end = 0;

        char *verse_string = "__verse";
        char *stanza_string = "__stanza";
        char *poem_string = "__poem";

        while (index < filesize){
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
        qp.everything_else = program;
    }

    qb_build_program(&qp);


    return qp;
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

void qb_rebuild_program(quibble_program *qp){
    free(qp->body);
    qb_build_program(qp);
}

void qb_build_program(quibble_program *qp){
    char *body = (char *)calloc(MAX_SOURCE_SIZE, sizeof(char));
    if (qp->everything_else != NULL){
        strcat(body, qp->everything_else);
        strcat(body, "\n\n");
    }

    for (int i = 0; i < qp->num_poems; ++i){
        char *temp_poem = qb_expand_poem(qp[0], i);
        if (temp_poem != NULL){
            strcat(body, temp_poem);
            strcat(body, "\n");
        }
        free(temp_poem);
    }


    int len = strlen(body)+1;
    char *final_body = (char *)calloc(len, sizeof(char));
    for (int i = 0; i < len; ++i){
        final_body[i] = body[i];
    }
    qp->body = final_body;
    free(body);
}

void qb_free_program(quibble_program qp){
    free(qp.everything_else);
    free(qp.body);
    for (int i = 0; i < qp.num_verses; ++i){
        qb_free_verse(qp.verse_list[i]);
    }
    for (int i = 0; i < qp.num_stanzas; ++i){
        qb_free_stanza(qp.stanza_list[i]);
    }
    for (int i = 0; i < qp.num_poems; ++i){
        qb_free_poem(qp.poem_list[i]);
    }
    free(qp.verse_list);
    free(qp.stanza_list);
    free(qp.poem_list);

    // OpenCL freeing
    free(qp.platform_ids);
    free(qp.device_ids);
    if (qp.configured){
        cl_check(clFlush(qp.command_queue));
        cl_check(clFinish(qp.command_queue));
        cl_check(clReleaseCommandQueue(qp.command_queue));
        cl_check(clReleaseContext(qp.context));

        for (int i = 0; i < qp.num_poems; ++i){
            cl_check(clReleaseKernel(qp.kernels[i]));
            cl_check(clReleaseProgram(qp.programs[i]));
        }
    }
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

    print_cl_info(qp);
}

/*----------------------------------------------------------------------------//
    OpenCL Interface
//----------------------------------------------------------------------------*/

char *get_device_name(cl_device_id device_id){
    size_t size;
    clGetDeviceInfo(device_id, CL_DEVICE_NAME, 0, NULL, &size);
    char *str = (char *)malloc(size);
    clGetDeviceInfo(device_id, CL_DEVICE_NAME, size, str, NULL);
    return str;
}

char *get_platform_name(cl_platform_id platform_id){
    size_t size;
    clGetPlatformInfo(platform_id, CL_PLATFORM_NAME, 0, NULL, &size);
    char *str = (char *)malloc(size);
    clGetPlatformInfo(platform_id, CL_PLATFORM_NAME, size, str, NULL);
    return str;
}

void print_cl_info(quibble_program qp){
    char *platform_name =
        get_platform_name(qp.platform_ids[qp.chosen_platform]);
    char *device_name = get_device_name(qp.device_ids[qp.chosen_device]);

    printf("Platform: %s\nDevice: %s\n\n", platform_name, device_name);
    free(platform_name);
    free(device_name);
}

void qb_find_platforms(quibble_program *qp){
    // 10 is arbitrary, but should be high enough for almost any task
    // This finds &qp->num_platforms
    cl_check(
        clGetPlatformIDs(10, NULL, &qp->num_platforms)
    );

    qp->platform_ids = (cl_platform_id *)malloc(qp->num_platforms *
                                                sizeof(cl_platform_id));

    // This finds qp->platform_ids,
    cl_check(
        clGetPlatformIDs(qp->num_platforms, qp->platform_ids, NULL)
    );
}

void qb_find_devices(quibble_program *qp){

    // finds &qp->num_devices
    cl_check(
        clGetDeviceIDs(qp->platform_ids[qp->chosen_platform],
                       CL_DEVICE_TYPE_ALL,
                       0,
                       NULL,
                       &qp->num_devices)
    );

    qp->device_ids = (cl_device_id *)malloc(qp->num_devices *
                                            sizeof(cl_device_id));

    // finds &qp->num_devices
    cl_check(
        clGetDeviceIDs(qp->platform_ids[qp->chosen_platform],
                       CL_DEVICE_TYPE_ALL,
                       qp->num_devices,
                       qp->device_ids,
                       &qp->num_devices)
    );

}

void qb_configure_program(quibble_program *qp, int platform, int device){
    qp->chosen_platform = platform;
    qp->chosen_device = device;

    cl_int err;

    qp->context = clCreateContext(NULL,
                                  1,
                                  &qp->device_ids[qp->chosen_device],
                                  NULL,
                                  NULL,
                                  &err);
    cl_check(err);

    qp->command_queue = clCreateCommandQueueWithProperties(
         qp->context,
         qp->device_ids[qp->chosen_device],
         0,
         &err
    );
    cl_check(err);

    // Create program
    qp->programs = (cl_program *)malloc(sizeof(cl_kernel)*qp->num_poems);
    qp->kernels = (cl_kernel *)malloc(sizeof(cl_kernel)*qp->num_poems);
    for (int i = 0; i < qp->num_poems; ++i){
        size_t kernel_size = strlen(qp->poem_list[i].body);
        qp->programs[i] = clCreateProgramWithSource(
            qp->context,
            1,
            (const char**)&qp->poem_list[i].body,
            (const size_t *)&kernel_size,
            &err
        );
        cl_check(err);

        err = clBuildProgram(qp->programs[i],
                              1,
                              &qp->device_ids[qp->chosen_device],
                              NULL,
                              NULL,
                              NULL);
        cl_check_program(err, qp->programs[i],
                         qp->device_ids[qp->chosen_device]);

        qp->kernels[i] = clCreateKernel(qp->programs[i], 
                                        qp->poem_list[i].name, &err);
        cl_check(err);
    }

    qp->configured = true;

}

void qb_run(quibble_program qp, char *kernel_name,
            size_t global_item_size,
            size_t local_item_size){
    int kernel_num = qb_find_poem_index(qp, kernel_name);
    cl_check(
        clEnqueueNDRangeKernel(qp.command_queue,
                               qp.kernels[kernel_num],
                               1,
                               NULL,
                               &global_item_size,
                               &local_item_size,
                               0,
                               NULL,
                               NULL)
    );

}
