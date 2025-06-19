/*-------------quibble_program.c----------------------------------------------//

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
quibble_program qb_create_program(char *filename){

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
    char *buffer = (char *)calloc(filesize, sizeof(char));

    while (!feof(fileptr)){
        fread(buffer, sizeof(char), filesize, fileptr);
    }

    quibble_program qp = qb_parse_program(buffer);

    free(buffer);
    fclose(fileptr);

    return qp;
}

quibble_program qb_parse_program(char *program){

    int num_verses = qb_find_occurrences("__verse", program);
    int num_stanzas = qb_find_occurrences("__stanza", program);
    int num_poems = qb_find_occurrences("__poem", program);

    int filesize = strlen(program);

    // creating program and populating verses and everything else
    quibble_program qp;

    // TODO: Also build!!!
    qp.body = NULL;
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
    STRING MANIP
//----------------------------------------------------------------------------*/

bool qb_is_stanza(char *stanza, int offset){
    char substr[9] = "__stanza";
    for (int i = 0; i < 8; ++i){
        if (stanza[i+offset] != substr[i]){
            return false;
        }
    }

    return true;
}

bool qb_is_poem(char *poem, int offset){
    char substr[7] = "__poem";
    for (int i = 0; i < 6; ++i){
        if (poem[i+offset] != substr[i]){
            return false;
        }
    }

    return true;
}

bool qb_is_verse(char *verse, int offset){
    char substr[8] = "__verse";
    for (int i = 0; i < 7; ++i){
        if (verse[i+offset] != substr[i]){
            return false;
        }
    }

    return true;
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


/*----------------------------------------------------------------------------//
    STRING -> QUIBBLE
//----------------------------------------------------------------------------*/

quibble_stanza qb_parse_stanza(char *stanza){
    int stanza_size = strlen(stanza);
    int offset = 0;
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
            (char *)calloc((config_end-config_start), sizeof(char));

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

    if (prologue_end-prologue_start > 0){
        char *prologue = (char *)calloc((prologue_end-prologue_start),
                                        sizeof(char));
        for (int i = 0; i < (prologue_end-prologue_start); ++i){
            prologue[i] = prologue[prologue_start+i];
        }

        if (qbinlined){
            qb_preprocess_content(prologue);
        }
        final_stanza.prologue = prologue;
    }
    else{
        final_stanza.prologue = NULL;
    }

    int epilogue_start = prologue_end + 18;
    int epilogue_end =
        qb_find_next_char(stanza, epilogue_start-1, '}');

    if (epilogue_end-epilogue_start > 0){
        char *epilogue = (char *)calloc((epilogue_end-epilogue_start),
                                        sizeof(char));
        for (int i = 0; i < (epilogue_end-epilogue_start); ++i){
            epilogue[i] = epilogue[epilogue_start+i];
        }

        if (qbinlined){
            qb_preprocess_content(epilogue);
        }
        final_stanza.epilogue = epilogue;
    }
    else{
        final_stanza.epilogue = NULL;
    }

    final_stanza.name = qb_strip_spaces(stanza, offset, config_start-2);

    return final_stanza;
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
            (char *)calloc((config_end-config_start), sizeof(char));

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
        char *body = (char *)calloc((body_end-body_start), sizeof(char));
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
            (char *)calloc((config_end-config_start), sizeof(char));

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
        char *body = (char *)calloc((body_end-body_start), sizeof(char));
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
    final_verse.echo = false;

    return final_verse;
}

/*----------------------------------------------------------------------------//
    ARGS / KWARGS
//----------------------------------------------------------------------------*/

int qb_find_number_of_args(char *config){

    int config_size = strlen(config);
    int i = 0;
    int num_entries = 0;
    int pipe_loc = qb_find_next_char(config, i, '|');
    if (pipe_loc > 0){
         config_size = pipe_loc-1;
    }

    if (config_size == 0){
        if (qb_is_space(config[i])){
            return 0;
        }
        else {
            return 1;
        }
    }

    // only kwargs, no args...
    if (qb_find_next_char(config, i, '=') <= config_size &&
        qb_find_next_char(config, i, '=') > 0){
        return 0;
    }

    int next_comma = qb_find_next_char(config, i, ',');
    // Only 1 or 0 entries...
    if (next_comma < 0){
        char *value = qb_strip_spaces(config, i, config_size);
        int ret = 1;
        if (value == NULL){
            return 0;
        }
        free(value);
        return ret;
    }
    while (next_comma > 0){

        if (next_comma > i){
            ++num_entries;
            i = next_comma+1;
        }
        next_comma = qb_find_next_char(config, i, ',');
    }

    return num_entries+1;
}


int qb_find_number_of_kwargs(char *config){

    int i = 0;
    int num_entries = 0;

    if (qb_find_next_char(config, i, '=') < 0){
        return 0;
    }

    if (qb_find_next_char(config, i, '|') > 0){
         i += qb_find_next_char(config, i, '|') + 1;
    }
    if (qb_find_next_char(config, i, ',') > 0){
        fprintf(stderr, "Comma (,) used instead of semicolon (;)!\nEach quibble kwarg is a self-contained C expression and must end with a `;`!\n");
        exit(1);
    }
    if (qb_find_next_char(config, i, '=') > 0 &&
        qb_find_next_char(config, i, ';') < 0){
        fprintf(stderr, "Equal (=) found without terminating semicolon!\nEach quibble kwarg is a self-contained C expression and must end with a `;`!\n");
        exit(1);
    }

    int next_equal = qb_find_next_char(config, i, '=');
    int next_semicolon = qb_find_next_char(config, i, ';');

    if(next_semicolon < 0){
        return 0;
    }
    while (next_semicolon > 0){

        if (next_equal > i && next_semicolon > next_equal){
            ++num_entries;
            i = next_semicolon + 1;
        }
        else {
            fprintf(stderr, "Equal (=) found without terminating semicolon!\nEach quibble kwarg is a self-contained C expression and must end with a `;`!\n");
            exit(1);
        }
        next_equal = qb_find_next_char(config, i, '=');
        next_semicolon = qb_find_next_char(config, i, ';');
    }

    return num_entries;

}

quibble_arg *qb_parse_args(char *config, int num_entries){

    int config_size = strlen(config);

    if (num_entries > 0){
        quibble_arg *final_args =
             (quibble_arg *)malloc(sizeof(quibble_arg) * num_entries);

        int i = 0;
        int curr_entry = 0;
        int next_comma = 0;
        if (qb_find_next_char(config, i, '|') > 0){
            config_size = qb_find_next_char(config, i, '|');
        }

        while (i < config_size){
            next_comma = qb_find_next_char(config, i, ',');

            if (next_comma < 0){
                next_comma = config_size;
            }
            final_args[curr_entry].variable =
                qb_strip_spaces(config, i, next_comma-1);

            // Check to make sure entry is unique
            for (int j = 1; j <= curr_entry; ++j){
                if (strcmp(final_args[j-1].variable,
                           final_args[j].variable) == 0){
                    fprintf(stderr, "Variable %s used more than once for arguments!\n", final_args[j].variable);
                    exit(1);
                }
            }

            i = next_comma + 1;
            ++curr_entry;
        }

        return final_args;
    }
    else {
        return NULL;
    }

}

quibble_kwarg *qb_parse_kwargs(char *config, int num_entries){

    int config_size = strlen(config);

    if (num_entries > 0){
        quibble_kwarg *final_kwargs =
             (quibble_kwarg *)malloc(sizeof(quibble_kwarg) * num_entries);

        int i = 0;
        int curr_entry = 0;
        int next_equal = 0;
        int next_semicolon = 0;
        if (qb_find_next_char(config, i, '|') > 0){
            i += qb_find_next_char(config, i, '|') + 1;
        }

        while (i < config_size){
            next_equal = qb_find_next_char(config, i, '=');
            next_semicolon = qb_find_next_char(config, i, ';');

            final_kwargs[curr_entry].variable =
                qb_strip_spaces(config, i, next_equal-1);

            final_kwargs[curr_entry].value =
                qb_strip_spaces(config, next_equal+1, next_semicolon-1);

            // Check to make sure entry is unique
            for (int j = 1; j <= curr_entry; ++j){
                if (strcmp(final_kwargs[j-1].variable,
                           final_kwargs[j].variable) == 0){
                    fprintf(stderr, "Variable %s used more than once for kwarg arguments!\n", final_kwargs[j].variable);
                    exit(1);
                }
            }

            i = next_semicolon + 1;
            ++curr_entry;

        }

        return final_kwargs;
    }
    else {
        return NULL;
    }

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

int qb_find_arg_index(quibble_arg *qa, int n, char *variable){
    if (n <= 0){
        return -1;
    }


    for (int i = 0; i < n; ++i){
        if (strcmp(qa[i].variable, variable) == 0){
            return i;
        }
    }
    fprintf(stderr, "Argument %s not found!\n", variable);
    exit(1);
}

int qb_find_kwarg_index(quibble_kwarg *qk, int n, char *variable){
    if (n <= 0){
        return -1;
    }

    for (int i = 0; i < n; ++i){
        if (strcmp(qk[i].variable, variable) == 0){
            return i;
        }
    }
    fprintf(stderr, "Keyword variable %s not found!\n", variable);
    exit(1);
}

// Configures prologues of existing verses
// The variadic function takes triples after the initial verse and number of
// kwargs being modified:
//     1. char * name of variable
//     2. char * type of value
//     3. auto value to be used
void qb_configure_verse_variadic(quibble_verse *qv, int n, va_list args){
    for (int i = 0; i < n; ++i){
        char *curr_variable = va_arg(args, char *);
        int kwarg_index = qb_find_kwarg_index(qv->kwargs,
                                              qv->num_kwargs,
                                              curr_variable);
        free(qv->kwargs[kwarg_index].value);

        char *curr_type = va_arg(args, char *);

        if (strcmp(curr_type, "int") == 0   ||
            strcmp(curr_type, "i") == 0     ||
            strcmp(curr_type, "d") == 0){

            int arg = va_arg(args, int);
            qv->kwargs[kwarg_index].value = qb_int_to_string(arg);
        }
        else if (strcmp(curr_type, "float") == 0  ||
                 strcmp(curr_type, "double") == 0 ||
                 strcmp(curr_type, "f") == 0      ||
                 strcmp(curr_type, "lf") == 0){
            double arg = va_arg(args, double);
            qv->kwargs[kwarg_index].value = qb_float_to_string(arg);
        }
        else if (strcmp(curr_type, "quibble_array") == 0 ||
                 strcmp(curr_type, "qa") == 0){
            qv->kwargs[kwarg_index].value = 
                qb_array_to_string(va_arg(args, quibble_array));
        }
        else if (strcmp(curr_type, "quibble_variable") == 0 ||
                 strcmp(curr_type, "qv") == 0){
            qv->kwargs[kwarg_index].value = 
                qb_variable_to_string(va_arg(args, quibble_variable));
        }
        else{
            fprintf(stderr,
                    "Type %s not supported as quibble input!\n",
                    curr_type);
            exit(1);
        }
    }

}

void qb_configure_verse(quibble_verse *qv, int n, ...){
    va_list args;
    va_start(args, n);

    qb_configure_verse_variadic(qv, n, args);
 
    va_end(args);
}

// An echo is a verse with the same body, but different prologue
quibble_verse qb_echo_verse(quibble_verse qv, int n, ...){
    quibble_verse final_qv;
    final_qv.echo = true;
    final_qv.body = qv.body;
    final_qv.name = qv.name;
    final_qv.num_kwargs = qv.num_kwargs;
    if (qv.num_kwargs > 0){
        final_qv.kwargs =
            (quibble_kwarg *)malloc(sizeof(quibble_kwarg)*qv.num_kwargs);
    }
    else {
        final_qv.kwargs = NULL;
    }

    for (int i = 0; i < qv.num_kwargs; ++i){
        int vlength = strlen(qv.kwargs[i].variable)+1;
        final_qv.kwargs[i].variable = (char *)malloc(sizeof(char)*vlength);

        for (int j = 0; j < vlength; ++j){
            final_qv.kwargs[i].variable[j] = qv.kwargs[i].variable[j];
        }

        vlength = strlen(qv.kwargs[i].value)+1;
        final_qv.kwargs[i].value = (char *)malloc(sizeof(char)*vlength);
        for (int j = 0; j < vlength; ++j){
            final_qv.kwargs[i].value[j] = qv.kwargs[i].value[j];
        }
    }

    va_list args;
    va_start(args, n);
 
    qb_configure_verse_variadic(&final_qv, n, args);
 
    va_end(args);

    return final_qv;
}

/*----------------------------------------------------------------------------//
    EXPANSION
//----------------------------------------------------------------------------*/

char *qb_expand_verse(quibble_program qp, char* verse_name, char *config){

    int idx = qb_find_verse_index(qp, verse_name);
    char *tmp_prologue = qb_create_prologue(config, verse_name,
        qp.verse_list[idx].args, qp.verse_list[idx].num_args,
        qp.verse_list[idx].kwargs, qp.verse_list[idx].num_kwargs);

    char *tmp_body = (char *)calloc(MAX_META_SIZE, sizeof(char));
    strcat(tmp_body, tmp_prologue);
    strcat(tmp_body, qp.verse_list[idx].body);

    int len = strlen(tmp_body);
    char *final_body = (char *)calloc(len, sizeof(char));

    for (int i = 0; i < len; ++i){
        final_body[i] = tmp_body[i];
    }

    free(tmp_body);
    free(tmp_prologue);
    return final_body;
    
}

char *qb_expand_stanza(quibble_program qp,
                       char* stanza_name, char *config, char *body){

    int idx = qb_find_stanza_index(qp, stanza_name);

    char *tmp_prologue = qb_create_prologue(config, stanza_name,
        qp.stanza_list[idx].args, qp.stanza_list[idx].num_args,
        qp.stanza_list[idx].kwargs, qp.stanza_list[idx].num_kwargs);

    char *tmp_body = (char *)calloc(MAX_META_SIZE, sizeof(char));
    strcat(tmp_body, tmp_prologue);
    strcat(tmp_body, qp.stanza_list[idx].prologue);

    // Parse verse content
    int index = 0;
    int offset = strlen(tmp_body);
    int max_size = strlen(body);
    int vcall_match_count = 0;

    int verse_name_start;
    int verse_name_end;
    int config_end;
    int verse_length;
    char *vcall_string = "@VCALL ";
    while (index < max_size){
        if (body[index] == vcall_string[vcall_match_count]){
            vcall_match_count++;
            if (vcall_match_count == 7){
                offset -= 6;
                memset(tmp_body+index+offset, 0,
                           strlen(tmp_body));

                verse_name_start = index;
                verse_name_end = qb_find_next_char(body, verse_name_start, '(');
                char *verse_name = qb_strip_spaces(body, verse_name_start,
                                             verse_name_end);
                config_end = qb_find_matching_char(body, max_size,
                    verse_name_end+1, '(',')');

                if (config_end > verse_name_end+1){
                    char *config = (char *)calloc(config_end-verse_name_end+1,
                                                  sizeof(char));
                    for (int i = 0; i < config_end-verse_name_end+1; ++i){
                        config[i] = body[verse_name_end+1+i];
                    }
                    char *verse = qb_expand_verse(qp, verse_name, config);
                    verse_length = strlen(verse);
                    for (int i = 0; i < verse_length; ++i){
                        tmp_body[index + offset + i] = verse[i];
                    }
                    offset += verse_length;
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
        tmp_body[index + offset] = body[index];
        ++index;
    }


    strcat(tmp_body, qp.stanza_list[idx].epilogue);

    int len = strlen(tmp_body);
    char *final_body = (char *)calloc(len, sizeof(char));

    for (int i = 0; i < len; ++i){
        final_body[i] = tmp_body[i];
    }

    free(tmp_body);
    free(tmp_prologue);

    return final_body;

}

// TODO: ADD OTHERS: _pt, _time, _world_size, _origin ...
char *qb_expand_poem(quibble_program qp, int poem_index){

    char *tmp_body = (char *)calloc(MAX_META_SIZE, sizeof(char));
    char *body = qp.poem_list[poem_index].body;

    strcat(tmp_body, "__kernel void ");
    strcat(tmp_body, "_kernel(__global float *_img, ");
    strcat(tmp_body, qp.poem_list[poem_index].name);
    for (int i = 0; i < qp.poem_list[poem_index].num_args; ++i){
        strcat(tmp_body, qp.poem_list[poem_index].args[i].variable);
        if (i == qp.poem_list[poem_index].num_args - 1){
            strcat(tmp_body, "){\n");
        }
        else{
            strcat(tmp_body, ", ");
        }
    }

    strcat(tmp_body, "    int _idx = get_global_id(0);\n");

    // Parse verse content
    int index = 0;
    int offset = strlen(tmp_body);
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
                offset -= 6;
                memset(tmp_body+index+offset, 0,
                           strlen(tmp_body));

                meta_name_start = index;
                meta_name_end = qb_find_next_char(body, meta_name_start, '(');
                char *verse_name = qb_strip_spaces(body, meta_name_start,
                                             meta_name_end);
                config_end = qb_find_matching_char(body, max_size,
                    meta_name_end+1, '(',')');

                if (config_end > meta_name_end+1){
                    char *config = (char *)calloc(config_end-meta_name_end+1,
                                                  sizeof(char));
                    for (int i = 0; i < config_end-meta_name_end+1; ++i){
                        config[i] = body[meta_name_end+1+i];
                    }
                    char *verse = qb_expand_verse(qp, verse_name, config);
                    meta_length = strlen(verse);
                    for (int i = 0; i < meta_length; ++i){
                        tmp_body[index + offset + i] = verse[i];
                    }
                    offset += meta_length;
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
                offset -= 6;
                memset(tmp_body+index+offset, 0,
                           strlen(tmp_body));

                meta_name_start = index;
                meta_name_end = qb_find_next_char(body, meta_name_start, '(');
                char *stanza_name = qb_strip_spaces(body, meta_name_start,
                                              meta_name_end);
                config_end = qb_find_matching_char(body, max_size,
                    meta_name_end+1, '(',')');

                stanza_body_start = qb_find_next_char(body, config_end, '(')+1;
                stanza_body_end = qb_find_matching_char(body, max_size,
                    stanza_body_start, '{','}');
                if (config_end > meta_name_end+1 &&
                    stanza_body_end > stanza_body_start){
                    char *config = (char *)calloc(config_end-meta_name_end+1,
                                                  sizeof(char));

                    for (int i = 0; i < config_end-meta_name_end+1; ++i){
                        config[i] = body[meta_name_end+1+i];
                    }
                    char *stanza_body = (char *)calloc(stanza_body_end -
                        stanza_body_start, sizeof(char));
                    for (int i = 0;
                         i < stanza_body_end - stanza_body_start;
                         ++i){
                        stanza_body[i] = body[stanza_body_start+i];
                    }
                    char *stanza = qb_expand_stanza(qp, stanza_name, config,
                                                    stanza_body);
                    meta_length = strlen(stanza);
                    for (int i = 0; i < meta_length; ++i){
                        tmp_body[index + offset + i] = stanza[i];
                    }
                    offset += meta_length;
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


        tmp_body[index + offset] = body[index];
        ++index;
    }

    int len = strlen(tmp_body);
    char *final_body = (char *)calloc(len, sizeof(char));

    for (int i = 0; i < len; ++i){
        final_body[i] = tmp_body[i];
    }

    free(tmp_body);

    return final_body;

}
 
void qb_rebuild_program(quibble_program qp){
    free(qp.body);
    qb_build_program(qp);
}

void qb_build_program(quibble_program qp){
    char *body = (char *)calloc(MAX_SOURCE_SIZE, sizeof(char));
    strcat(body, qp.everything_else);
    for (int i = 0; i < qp.num_poems; ++i){
        char *temp_poem = qb_expand_poem(qp, i);
        strcat(body, temp_poem);
        free(temp_poem);
    }

    int len = strlen(body);
    char *final_body = (char *)calloc(len, sizeof(char));
    for (int i = 0; i < len; ++i){
        final_body[i] = body[i];
    }
    qp.body = final_body;
}

// To be used in `qb_configure_verse` to create prologue string
char *qb_create_prologue(char *config, char *name,
                         quibble_arg *qargs, int num_args,
                         quibble_kwarg *qkwargs, int num_kwargs){

    char *temp = (char *)calloc(MAX_PROLOGUE_SIZE, sizeof(char));

    int num_config_args = qb_find_number_of_args(config);

    if (num_config_args == num_args){
        quibble_arg *temp_args = qb_parse_args(config, num_args);
        for (int i = 0; i < num_args; ++i){
            strcat(temp, qargs[i].variable);
            strcat(temp, " = ");
            strcat(temp, temp_args[i].variable);
            strcat(temp, ";\n");
        }
        qb_free_arg_array(temp_args, num_args);
    }
    else {
        fprintf(stderr, "Incorrect number of arguments for %s\nExpected %d, got %d!\n", name, num_args, num_config_args);
        exit(1);
    }

    int num_config_kwargs = qb_find_number_of_kwargs(config);

    if (num_config_kwargs > 0){
        quibble_kwarg *temp_kwargs =
            qb_parse_kwargs(config, num_config_kwargs);

        char *value;
        for (int i = 0; i < num_kwargs; ++i){
            value = qkwargs[i].value;
            for (int j  = 0; j < num_config_kwargs; ++j){
                if(strcmp(temp_kwargs[j].variable, qkwargs[i].variable)){
                    value = temp_kwargs[i].value;
                }
            }
            strcat(temp, qkwargs[i].variable);
            strcat(temp, " = ");
            strcat(temp, value);
            strcat(temp, ";\n");
        }
        qb_free_kwarg_array(temp_kwargs, num_config_kwargs);
    }
    else {
        for (int i = 0; i < num_kwargs; ++i){
            strcat(temp, qkwargs[i].variable);
            strcat(temp, " = ");
            strcat(temp, qkwargs[i].value);
            strcat(temp, ";\n");
        }
    }

    int len = strlen(temp);
    char *final_output = (char *)calloc(len, sizeof(char));

    for (int i = 0; i < len; ++i){
        final_output[i] = temp[i];
    }

    free(temp);
    return final_output;
}

/*----------------------------------------------------------------------------//
    FREE
//----------------------------------------------------------------------------*/

void qb_free_arg(quibble_arg qarg){
    free(qarg.variable);
}

void qb_free_kwarg(quibble_kwarg qkwarg){
    free(qkwarg.variable);
    free(qkwarg.value);
}

void qb_free_arg_array(quibble_arg *qargs, int n){
    for (int i = 0; i < n; ++i){
        qb_free_arg(qargs[i]);
    }
    free(qargs);
}

void qb_free_kwarg_array(quibble_kwarg *qkwargs, int n){
    for (int i = 0; i < n; ++i){
        qb_free_kwarg(qkwargs[i]);
    }
    free(qkwargs);
}

void qb_free_verse(quibble_verse qv){
    if (qv.echo == false){
        free(qv.body);
        free(qv.name);
    }
    qb_free_kwarg_array(qv.kwargs, qv.num_kwargs);
    qb_free_arg_array(qv.args, qv.num_args);
}

void qb_free_stanza(quibble_stanza qs){
    free(qs.prologue);
    free(qs.epilogue);
    free(qs.name);
    qb_free_kwarg_array(qs.kwargs, qs.num_kwargs);
    qb_free_arg_array(qs.args, qs.num_args);
}

void qb_free_poem(quibble_poem qp){
    free(qp.body);
    free(qp.name);
    qb_free_arg_array(qp.args, qp.num_args);
}

void qb_free_program(quibble_program qp){
    free(qp.everything_else);
    //free(qp.body);
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
}
