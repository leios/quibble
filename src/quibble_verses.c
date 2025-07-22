/*-------------verses---------------------------------------------------------//

 Purpose: A quibble verse is a user-submitted function fragement to be compiled
          later

//----------------------------------------------------------------------------*/

#include "../include/quibble.h"

bool qb_is_verse(char *verse, int offset){
    char substr[8] = "__verse";
    for (int i = 0; i < 7; ++i){
        if (verse[i+offset] != substr[i]){
            return false;
        }
    }

    return true;
}

bool qb_find_keyword_in_verses(quibble_program qp, char *keyword){
    int found_index = -1;
    for (int i = 0; i < qp.num_verses; ++i){
        found_index = qb_find_next_string(qp.verse_list[i].body, 0, keyword);
    }
    if (found_index >= 0){
        return true;
    }
    return false;
}

quibble_verse qb_parse_verse(char *verse){
    int verse_size = strlen(verse);
    int offset = 0;
    bool qbinlined = qb_is_inlined(verse);
    if (qbinlined){
        offset += 22;
    }

    if (qb_is_verse(verse, offset)){
        offset += 7;
    }
    else {
        fprintf(stderr, "Quibble verses must be configured with `__verse`!\n");
        exit(1);
    }

    quibble_verse final_verse;

    int config_start = qb_find_next_char(verse, offset, '(')+1;
    int config_end = qb_find_matching_char(verse, verse_size, config_start-1, '(', ')');


    if (config_end-config_start > 0){
        char *config =
            (char *)calloc((config_end-config_start)+1, sizeof(char));

        for (int i = 0; i < (config_end-config_start); ++i){
            config[i] = verse[config_start+i];
        }

        final_verse.num_args = qb_find_number_of_args(config);
        final_verse.args =
            qb_parse_args(config, final_verse.num_args);

        final_verse.num_kwargs = qb_find_number_of_kwargs(config);
        final_verse.kwargs =
            qb_parse_kwargs(config, final_verse.num_kwargs);
        free(config);
    }
    else {
        final_verse.args = NULL;
        final_verse.num_args = 0;
        final_verse.kwargs = NULL;
        final_verse.num_kwargs = 0;
    }

    int body_start = qb_find_next_char(verse, offset, '{')+1;
    int body_end =
        qb_find_matching_char(verse, verse_size, body_start-1, '{', '}');

    if (body_end-body_start > 0){
        char *body = (char *)calloc((body_end-body_start)+1, sizeof(char));
        for (int i = 0; i < (body_end-body_start); ++i){
            body[i] = verse[body_start+i];
        }

        if (qbinlined){
            qb_preprocess_content(body);
        }
        final_verse.body = body;
    }
    else{
        final_verse.body = NULL;
    }
    final_verse.name = qb_strip_spaces(verse, offset, config_start-2);

    return final_verse;
}

char *qb_expand_verse(quibble_program qp, char* verse_name, char *config){

    int idx = qb_find_verse_index(qp, verse_name);
    if (qp.verse_list[idx].body == NULL){
        return NULL;
    }
    char *tmp_prologue = qb_create_prologue(config, verse_name,
        qp.verse_list[idx].args, qp.verse_list[idx].num_args,
        qp.verse_list[idx].kwargs, qp.verse_list[idx].num_kwargs);

    char *tmp_body = (char *)calloc(QB_MAX_META_SIZE, sizeof(char));
    strcat(tmp_body, tmp_prologue);
    strcat(tmp_body, qp.verse_list[idx].body);

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

void qb_free_verse(quibble_verse qv){
    free(qv.body);
    free(qv.name);
    qb_free_kwarg_array(qv.kwargs, qv.num_kwargs);
    qb_free_arg_array(qv.args, qv.num_args);
}

void qb_print_verse(quibble_verse qv){
    printf("Quibble Verse:\nName: %s\nBody:\n%s\n", qv.name, qv.body);
    printf("Args...\n");
    for (int i = 0; i < qv.num_args; ++i){
        qb_print_arg(qv.args[i]);
    }
    printf("Kwargs...\n");
    for (int i = 0; i < qv.num_kwargs; ++i){
        qb_print_kwarg(qv.kwargs[i]);
    }
    printf("\n");
}
