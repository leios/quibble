/*-------------poem-----------------------------------------------------------//

 Purpose: A quibble poem is a user-submitted collection of stanzas and verses

//----------------------------------------------------------------------------*/

#include "../include/quibble_program.h"

bool qb_is_poem(char *poem, int offset){
    char substr[7] = "__poem";
    for (int i = 0; i < 6; ++i){
        if (poem[i+offset] != substr[i]){
            return false;
        }
    }

    return true;
}

quibble_poem qb_parse_poem(char *poem){
    int poem_size = strlen(poem);
    int offset = 0;
    bool qbinlined = qb_is_inlined(poem);
    if (qbinlined){
        offset += 22;
    }

    if (qb_is_poem(poem, offset)){
        offset += 6;
    }
    else {
        fprintf(stderr, "Quibble poems must be configured with `__poem`!\n");
        exit(1);
    }

    quibble_poem final_poem;

    int config_start = qb_find_next_char(poem, offset, '(')+1;
    int config_end =
        qb_find_matching_char(poem, poem_size, config_start-1, '(', ')');


    if (config_end-config_start > 0){
        char *config =
            (char *)calloc((config_end-config_start)+1, sizeof(char));

        for (int i = 0; i < (config_end-config_start); ++i){
            config[i] = poem[config_start+i];
        }

        final_poem.num_args = qb_find_number_of_args(config);
        final_poem.args =
            qb_parse_args(config, final_poem.num_args);

        free(config);
    }
    else {
        final_poem.args = NULL;
        final_poem.num_args = 0;
    }


    int body_start = qb_find_next_char(poem, offset, '{')+1;
    int body_end = qb_find_matching_char(poem, poem_size, body_start-1, '{', '}');

    if (body_end-body_start > 0){
        char *body = (char *)calloc((body_end-body_start)+2, sizeof(char));
        for (int i = 0; i < (body_end-body_start); ++i){
            body[i] = poem[body_start+i];
        }

        if (qbinlined){
            qb_preprocess_content(body);
        }
        final_poem.body = body;
    }
    else{
        final_poem.body = NULL;
    }
    final_poem.name = qb_strip_spaces(poem, offset, config_start-2);

    return final_poem;
}

// TODO: ADD OTHERS: _pt, _time, _world_size, _origin ...
char *qb_expand_poem(quibble_program qp, int poem_index){

    bool do_expansion = true;
    if (qp.poem_list[poem_index].body == NULL){
        do_expansion = false;
    }
    char *tmp_body = (char *)calloc(MAX_META_SIZE, sizeof(char));
    char *body = qp.poem_list[poem_index].body;

    strcat(tmp_body, "__kernel void ");
    strcat(tmp_body, qp.poem_list[poem_index].name);
    strcat(tmp_body, "(");
    if (qp.poem_list[poem_index].num_args > 0){
        //strcat(tmp_body, ", ");
        for (int i = 0; i < qp.poem_list[poem_index].num_args; ++i){

            if (qp.poem_list[poem_index].args[i].type != NULL){
                strcat(tmp_body, qp.poem_list[poem_index].args[i].type);
                strcat(tmp_body, " ");
            }
            strcat(tmp_body, qp.poem_list[poem_index].args[i].variable);
            if (i == qp.poem_list[poem_index].num_args - 1){
                strcat(tmp_body, "){\n");
            }
            else{
                strcat(tmp_body, ", ");
            }
        }
    }
    else {
        strcat(tmp_body, "){\n");
    }

    if (do_expansion){
        strcat(tmp_body, "    int _idx = get_global_id(0);\n");

        // Parse verse content
        int index = 0;
        int tmp_index = strlen(tmp_body);
        int max_size = strlen(body);
        int vcall_match_count = 0;
        int scall_match_count = 0;

        int meta_name_start;
        int meta_name_end;
        int config_end;
        int meta_length;

        int stanza_body_start;
        int stanza_body_end;

        char *vcall_string = "@VCALL ";
        char *scall_string = "@SCALL ";
        while (index < max_size){
            if (body[index] == vcall_string[vcall_match_count]){
                vcall_match_count++;
                if (vcall_match_count == 7){

                    tmp_index -= 6;
                    memset(tmp_body+tmp_index, 0, strlen(tmp_body));

                    meta_name_start = index + 1;
                    meta_name_end = qb_find_next_char(body, meta_name_start, '(');
                    char *verse_name = qb_strip_spaces(body, meta_name_start,
                                                 meta_name_end - 1);
                    config_end = qb_find_matching_char(body, max_size,
                        meta_name_end, '(',')')-1;

                    if (config_end > meta_name_end+1){
                        char *config = (char *)calloc(config_end-meta_name_end+2,
                                                      sizeof(char));
                        for (int i = 0; i < config_end-meta_name_end; ++i){
                            config[i] = body[meta_name_end+1+i];
                        }
                        char *verse = qb_expand_verse(qp, verse_name, config);
                        meta_length = strlen(verse);
                        for (int i = 0; i < meta_length; ++i){
                            tmp_body[tmp_index + i] = verse[i];
                        }
                        tmp_index += meta_length;
                        index = config_end + 2;
                        if (body[index] == ';'){
                            index++;
                        }
                        free(verse);
                        free(config);
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

            if (body[index] == scall_string[scall_match_count]){
                scall_match_count++;
                if (scall_match_count == 7){
                    tmp_index -= 6;
                    memset(tmp_body+tmp_index, 0, 7);

                    meta_name_start = index + 1;
                    meta_name_end = qb_find_next_char(body, meta_name_start, '(');
                    char *stanza_name = qb_strip_spaces(body, meta_name_start,
                                                  meta_name_end - 1);
                    config_end = qb_find_matching_char(body, max_size,
                        meta_name_end, '(',')')-1;

                    stanza_body_start = qb_find_next_char(body, config_end, '{')+1;
                    stanza_body_end = qb_find_matching_char(body, max_size,
                    stanza_body_start-1, '{','}');
                    if (config_end > meta_name_end+1 &&
                        stanza_body_end > stanza_body_start){
                    char *config = (char *)calloc(config_end-meta_name_end+2,
                                                      sizeof(char));
    
                        for (int i = 0; i < config_end-meta_name_end; ++i){
                            config[i] = body[meta_name_end+1+i];
                        }
                        char *stanza_body = (char *)calloc(stanza_body_end -
                            stanza_body_start+2, sizeof(char));
                        for (int i = 0;
                             i < stanza_body_end - stanza_body_start;
                             ++i){
                            stanza_body[i] = body[stanza_body_start+i];
                        }
                        char *stanza = qb_expand_stanza(qp, stanza_name, config,
                                                        stanza_body);
                        meta_length = strlen(stanza);
                        for (int i = 0; i < meta_length; ++i){
                            tmp_body[tmp_index + i] = stanza[i];
                        }
                        tmp_index += meta_length;
                        index = stanza_body_end + 1;
                        free(stanza);
                        free(config);
                        free(stanza_body);
                    }
                    else{
                        fprintf(stderr, "Stanza %s called incorrectly!\n",
                                stanza_name);
                        exit(1);
                    }
                    free(stanza_name);
                }
            }
            else if (scall_match_count != 0){
                scall_match_count = 0;
            }


            tmp_body[tmp_index] = body[index];
            ++index;
            ++tmp_index;
        }
    }


    strcat(tmp_body, "\n}\n");
    int len = strlen(tmp_body);
    char *final_body = (char *)calloc(len+2, sizeof(char));

    for (int i = 0; i < len; ++i){
        final_body[i] = tmp_body[i];
    }

    free(tmp_body);
    return final_body;

}
 
void qb_free_poem(quibble_poem qp){
    free(qp.body);
    free(qp.name);
    qb_free_arg_array(qp.args, qp.num_args);
}

void qb_print_poem(quibble_poem qp){
    printf("Quibble Poem:\nName: %s\nBody:\n%s\n", qp.name, qp.body);
    printf("Args...\n");
    for (int i = 0; i < qp.num_args; ++i){
        qb_print_arg(qp.args[i]);
    }
    printf("\n");
}
