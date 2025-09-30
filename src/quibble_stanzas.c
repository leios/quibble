/*-------------stanza---------------------------------------------------------//

 Purpose: A quibble stanza is a user-defined method for composing verses

//----------------------------------------------------------------------------*/

#include "../include/quibble.h"

bool qb_is_stanza(char *stanza, int offset){
    char substr[9] = "__stanza";
    for (int i = 0; i < 8; ++i){
        if (stanza[i+offset] != substr[i]){
            return false;
        }
    }

    return true;
}

bool qb_find_keyword_in_stanzas(quibble_program qp, char *keyword){
    int found_index = -1;
    int curr_stanza = 0;

    while (found_index < 0 && curr_stanza < qp.num_stanzas){
        found_index = qb_find_next_string(qp.stanza_list[curr_stanza].prologue,
                                          0,
                                          keyword);
        found_index = qb_find_next_string(qp.stanza_list[curr_stanza].epilogue,
                                          0,
                                          keyword);
        curr_stanza++;
    }
    if (found_index >= 0){
        return true;
    }
    return false;
}

quibble_stanza qb_parse_stanza(char *stanza){
    int stanza_size = strlen(stanza);
    int offset = 0;
    bool is_split = true;
    bool qbinlined = qb_is_inlined(stanza);
    if (qbinlined){
        offset += 22;
    }

    if (qb_is_stanza(stanza, offset)){
        offset += 8;
    }
    else {
        fprintf(stderr, "Quibble stanzas must be configured with `__stanza`!\n");
        exit(1);
    }

    quibble_stanza final_stanza;

    int config_start = qb_find_next_char(stanza, offset, '(')+1;
    int config_end =
        qb_find_matching_char(stanza, stanza_size, config_start-1, '(', ')');


    if (config_end-config_start > 0){
        char *config =
            (char *)calloc((config_end-config_start)+1, sizeof(char));

        for (int i = 0; i < (config_end-config_start); ++i){
            config[i] = stanza[config_start+i];
        }

        final_stanza.num_args = qb_find_number_of_args(config);
        final_stanza.args =
            qb_parse_args(config, final_stanza.num_args);

        final_stanza.num_kwargs = qb_find_number_of_kwargs(config);
        final_stanza.kwargs =
            qb_parse_kwargs(config, final_stanza.num_kwargs);
        free(config);
    }
    else {
        final_stanza.args = NULL;
        final_stanza.num_args = 0;
        final_stanza.kwargs = NULL;
        final_stanza.num_kwargs = 0;
    }

    int prologue_start = qb_find_next_char(stanza, offset, '{')+1;
    int prologue_end =
        qb_find_next_string(stanza, prologue_start, "__split_stanza();");

    // No split found
    if (prologue_end < 0){
        is_split = false;
        prologue_end = qb_find_matching_char(stanza, stanza_size, 
                                             prologue_start-1, '{', '}');
    }

    if (prologue_end-prologue_start > 0 && prologue_end > 0){
        char *prologue = (char *)calloc((prologue_end-prologue_start)+1,
                                        sizeof(char));
        for (int i = 0; i < (prologue_end-prologue_start); ++i){
            prologue[i] = stanza[prologue_start+i];
        }

        if (qbinlined){
            qb_preprocess_content(prologue);
        }
        final_stanza.prologue = prologue;
    }
    else{
        final_stanza.prologue = NULL;
    }

    if (is_split){
        int epilogue_start = prologue_end + 18;
        int epilogue_end = qb_find_matching_char(stanza, stanza_size, 
                                                 prologue_start-1, '{', '}');

        if (epilogue_end-epilogue_start > 0){
            char *epilogue = (char *)calloc((epilogue_end-epilogue_start)+1,
                                            sizeof(char));
            for (int i = 0; i < (epilogue_end-epilogue_start); ++i){
                epilogue[i] = stanza[epilogue_start+i];
            }

            if (qbinlined){
                qb_preprocess_content(epilogue);
            }
            final_stanza.epilogue = epilogue;
        }
        else{
            final_stanza.epilogue = NULL;
        }
    }
    else{
        final_stanza.epilogue = NULL;
    }

    final_stanza.name = qb_strip_spaces(stanza, offset, config_start-2);

    return final_stanza;
}

char *qb_expand_stanza_extra(quibble_program qp,
                             quibble_stanza stanza,
                             bool is_prologue){

    char *body;
    if (is_prologue){
        body = stanza.prologue;
    }
    else {
        body = stanza.epilogue;
    }

    char *stanza_extra = calloc(QB_MAX_META_SIZE, sizeof(char));

    // Parse verse content
    int index = 0;
    int tmp_index = 0;
    int max_size = strlen(body);
    int vcall_match_count = 0;

    int verse_name_start;
    int verse_name_end;
    int config_end;
    int verse_length;
    char *vcall_string = "@VCALL";

    while (index < max_size){
        if (body[index] == vcall_string[vcall_match_count]){
            vcall_match_count++;
            if (vcall_match_count == 6){
                tmp_index -= 5;
                memset(stanza_extra+tmp_index, 0, 6);

                verse_name_start = index+1;
                verse_name_end = qb_find_next_char(body, verse_name_start, '(');
                char *verse_name = qb_strip_spaces(body, verse_name_start,
                                             verse_name_end - 1);
                config_end = qb_find_matching_char(body, max_size,
                    verse_name_end, '(',')')-1;

                if (config_end > verse_name_end+1){
                    char *config = (char *)calloc(config_end-verse_name_end+2,
                                                  sizeof(char));
                    for (int i = 0; i < config_end-verse_name_end; ++i){
                        config[i] = body[verse_name_end+1+i];
                    }
                    char *verse = qb_expand_verse(qp, verse_name, config);
                    verse_length = strlen(verse);
                    for (int i = 0; i < verse_length; ++i){
                        stanza_extra[tmp_index + i] = verse[i];
                    }
                    index = config_end+2;
                    if (body[index] == ';'){
                        index++;
                    }
                    tmp_index += verse_length;
                    free(config);
                    free(verse);
                }
                else{
                    fprintf(stderr, "Verse %s called incorrectly!\n", verse_name);
                    exit(1);
                }
                free(verse_name);
            }
        }
        else if (vcall_match_count != 0){
            vcall_match_count = 0;
        }
        stanza_extra[tmp_index] = body[index];
        ++index;
        ++tmp_index;
    }

    return stanza_extra;
    
}

char *qb_expand_stanza(quibble_program qp,
                       char* stanza_name, char *config, char *body){

    int idx = qb_find_stanza_index(qp, stanza_name);
    if (qp.stanza_list[idx].prologue == NULL){
        return NULL;
    }

    char *tmp_prologue = qb_create_prologue(config, stanza_name,
        qp.stanza_list[idx].args, qp.stanza_list[idx].num_args,
        qp.stanza_list[idx].kwargs, qp.stanza_list[idx].num_kwargs);

    char *tmp_body = (char *)calloc(QB_MAX_META_SIZE, sizeof(char));

    strcat(tmp_body, tmp_prologue);

    char *stanza_prologue = qb_expand_stanza_extra(qp, qp.stanza_list[idx], true);
    strcat(tmp_body, stanza_prologue);
    free(stanza_prologue);

    // Parse verse content
    int index = 0;
    int tmp_index = strlen(tmp_body);
    int max_size = strlen(body);
    int vcall_match_count = 0;

    int verse_name_start;
    int verse_name_end;
    int config_end;
    int verse_length;
    char *vcall_string = "@VCALL";
    while (index < max_size){
        if (body[index] == vcall_string[vcall_match_count]){
            vcall_match_count++;
            if (vcall_match_count == 6){
                tmp_index -= 5;
                memset(tmp_body+tmp_index, 0, 6);

                verse_name_start = index+1;
                verse_name_end = qb_find_next_char(body, verse_name_start, '(');
                char *verse_name = qb_strip_spaces(body, verse_name_start,
                                             verse_name_end - 1);
                config_end = qb_find_matching_char(body, max_size,
                    verse_name_end, '(',')')-1;

                if (config_end > verse_name_end+1){
                    char *config = (char *)calloc(config_end-verse_name_end+2,
                                                  sizeof(char));
                    for (int i = 0; i < config_end-verse_name_end; ++i){
                        config[i] = body[verse_name_end+1+i];
                    }
                    char *verse = qb_expand_verse(qp, verse_name, config);
                    verse_length = strlen(verse);
                    for (int i = 0; i < verse_length; ++i){
                        tmp_body[tmp_index + i] = verse[i];
                    }
                    index = config_end+2;
                    if (body[index] == ';'){
                        index++;
                    }
                    tmp_index += verse_length;
                    free(config);
                    free(verse);
                }
                else{
                    fprintf(stderr, "Verse %s called incorrectly!\n", verse_name);
                    exit(1);
                }
                free(verse_name);
            }
        }
        else if (vcall_match_count != 0){
            vcall_match_count = 0;
        }
        tmp_body[tmp_index] = body[index];
        ++index;
        ++tmp_index;
    }


    if (qp.stanza_list[idx].epilogue != NULL){
        char *stanza_epilogue =
            qb_expand_stanza_extra(qp, qp.stanza_list[idx], false);
        strcat(tmp_body, stanza_epilogue);
        free(stanza_epilogue);
    }

    int len = strlen(tmp_body);
    char *final_body = (char *)calloc(len+5, sizeof(char));

    final_body[0] = '{';
    final_body[1] = '\n';

    for (int i = 2; i < len+2; ++i){
        final_body[i] = tmp_body[i-2];
    }

    final_body[len+2] = '}';
    final_body[len+3] = '\n';

    free(tmp_body);
    free(tmp_prologue);

    return final_body;

}

void qb_free_stanza(quibble_stanza qs){
    free(qs.prologue);
    free(qs.epilogue);
    free(qs.name);
    qb_free_kwarg_array(qs.kwargs, qs.num_kwargs);
    qb_free_arg_array(qs.args, qs.num_args);
}

/*----------------------------------------------------------------------------//
    PRINTING
//----------------------------------------------------------------------------*/

void qb_print_stanza(quibble_stanza qs){
    printf("Quibble Stanza:\nName: %s\nPrologue:\n%s\nEpilogue\n%s\n",
           qs.name, qs.prologue, qs.epilogue);
    printf("Args...\n");
    for (int i = 0; i < qs.num_args; ++i){
        qb_print_arg(qs.args[i]);
    }
    printf("Kwargs...\n");
    for (int i = 0; i < qs.num_kwargs; ++i){
        qb_print_kwarg(qs.kwargs[i]);
    }
    printf("\n");
}
