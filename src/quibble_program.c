/*-------------quibble_program.c----------------------------------------------//

 Purpose: A quibble program is a user-submitted set of verses, stanzas, poems,
          and functions (usually from a *.qbl file)

   Notes: Some unnecessary code duplication here, but... meh

//----------------------------------------------------------------------------*/

#include "../include/quibble_program.h"

// Reads an input file and parses everything into verses or OCL functions
quibble_program qb_create_program(char *filename){

    // Opening file
    FILE* fileptr;
    fileptr = fopen(filename, "r");

    if (fileptr == NULL){
        fprintf(stderr, "Error opening file %s!\n", filename);
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

    int num_verses = qb_find_occurrences("__verse", buffer);
    int num_stanzas = qb_find_occurrences("__stanza", buffer);
    int num_poems = qb_find_occurrences("__poem", buffer);

    // creating program and populating verses and everything else
    quibble_program qp;
    qp.num_verses = num_verses;
    qp.verse_list = (quibble_verse *)malloc(sizeof(quibble_verse)*num_verses);

    if (num_verses > 0 ||
        num_stanzas > 0 ||
        num_poems > 0){
        char *tmp_everything_else =
            (char *)calloc(MAX_SOURCE_SIZE, sizeof(char));

        char *tmp_verse = (char *)calloc(MAX_SOURCE_SIZE, sizeof(char));
        char *tmp_poem = (char *)calloc(MAX_SOURCE_SIZE, sizeof(char));
        char *tmp_stanza = (char *)calloc(MAX_SOURCE_SIZE, sizeof(char));

        int index = 0;
        int everything_else_index = 0;

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
            if (buffer[index] == poem_string[poem_match_count]){
                ++poem_match_count;
                if (poem_match_count == 6){
                    poem_start = index - 5;
                    poem_end = qb_find_next_char(
                        buffer, filesize, poem_start, '(');
                    poem_end = qb_find_matching_char(
                        buffer, filesize, poem_end, '(', ')');

                    poem_end = qb_find_next_char(
                        buffer, filesize, poem_end, '{');
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
                    everything_else_index -= 7;
                    memset(tmp_everything_else+everything_else_index, 0,
                           strlen(tmp_everything_else));
                    poem_end = 0;
                    poem_start = 0;
                    memset(tmp_poem,0,strlen(tmp_poem));
                }
            }
            else if (poem_match_count != 0){
                poem_match_count = 0;
            }

            if (buffer[index] == stanza_string[stanza_match_count]){
                ++stanza_match_count;
                if (stanza_match_count == 8){
                    stanza_start = index - 7;
                    stanza_end = qb_find_next_char(
                        buffer, filesize, stanza_start, '(');
                    stanza_end = qb_find_matching_char(
                        buffer, filesize, stanza_end, '(', ')');

                    stanza_end = qb_find_next_char(
                        buffer, filesize, stanza_end, '{');
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
                           strlen(tmp_everything_else));
                    stanza_end = 0;
                    stanza_start = 0;
                    memset(tmp_stanza,0,strlen(tmp_stanza));
                }
            }
            else if (stanza_match_count != 0){
                stanza_match_count = 0;
            }

            if (buffer[index] == verse_string[verse_match_count]){
                ++verse_match_count;
                if (verse_match_count == 7){
                    verse_start = index - 6;
                    verse_end = qb_find_next_char(
                        buffer, filesize, verse_start, '(');
                    verse_end = qb_find_matching_char(
                        buffer, filesize, verse_end, '(', ')');

                    verse_end = qb_find_next_char(
                        buffer, filesize, verse_end, '{');
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
                    everything_else_index -= 7;
                    memset(tmp_everything_else+everything_else_index, 0,
                           strlen(tmp_everything_else));
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
           qb_strip_spaces(tmp_everything_else, 0, everything_else_index);

        qp.everything_else = everything_else;

        // Only free if we ar not using it directly
        free(buffer);
        free(tmp_everything_else);
        free(tmp_verse);
    }
    else {
        qp.verse_list = NULL;
        qp.everything_else = buffer;
    }


    // Freeing everything
    fclose(fileptr);

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
        fprintf(stderr, "Quibble stanzas must be configured with `__stanza`!");
        exit(1);
    }

    quibble_stanza final_stanza;

    int config_start = qb_find_next_char(stanza, stanza_size, offset, '(')+1;
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
            qb_parse_keywords(config, final_stanza.num_kwargs);
        free(config);
    }
    else {
        final_stanza.args = NULL;
        final_stanza.num_args = 0;
        final_stanza.kwargs = NULL;
        final_stanza.num_kwargs = 0;
    }

    int prologue_start = qb_find_next_char(stanza, stanza_size, offset, '{')+1;
    int prologue_end =
        qb_find_next_string(stanza, stanza_size, prologue_start-1,
                            "__split_stanza();", 18);

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
        qb_find_next_char(stanza, stanza_size, epilogue_start-1, '}');

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

    final_stanza.name = qb_strip_spaces(stanza, offset, prologue_start-1);

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
        fprintf(stderr, "Quibble poems must be configured with `__poem`!");
        exit(1);
    }

    quibble_poem final_poem;

    int prologue_start = qb_find_next_char(poem, poem_size, offset, '(')+1;
    int prologue_end =
        qb_find_matching_char(poem, poem_size, prologue_start-1, '(', ')');


    if (prologue_end-prologue_start > 0){
        char *prologue =
            (char *)calloc((prologue_end-prologue_start), sizeof(char));

        for (int i = 0; i < (prologue_end-prologue_start); ++i){
            prologue[i] = poem[prologue_start+i];
        }

        final_poem.num_args = qb_find_number_of_args(prologue);
        final_poem.args =
            qb_parse_args(prologue, final_poem.num_args);

        final_poem.num_kwargs = qb_find_number_of_kwargs(prologue);
        final_poem.kwargs =
            qb_parse_keywords(prologue, final_poem.num_kwargs);
        free(prologue);
    }
    else {
        final_poem.args = NULL;
        final_poem.num_args = 0;
        final_poem.kwargs = NULL;
        final_poem.num_kwargs = 0;
    }

    int body_start = qb_find_next_char(poem, poem_size, offset, '{')+1;
    int body_end =
        qb_find_matching_char(poem, poem_size, body_start-1, '{', '}');

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
    final_poem.name = qb_strip_spaces(poem, offset, prologue_start-1);

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
        fprintf(stderr, "Quibble verses must be configured with `__verse`!");
        exit(1);
    }

    quibble_verse final_verse;

    int prologue_start = qb_find_next_char(verse, verse_size, offset, '(')+1;
    int prologue_end =
        qb_find_matching_char(verse, verse_size, prologue_start-1, '(', ')');


    if (prologue_end-prologue_start > 0){
        char *prologue =
            (char *)calloc((prologue_end-prologue_start), sizeof(char));

        for (int i = 0; i < (prologue_end-prologue_start); ++i){
            prologue[i] = verse[prologue_start+i];
        }

        final_verse.num_args = qb_find_number_of_args(prologue);
        final_verse.args =
            qb_parse_args(prologue, final_verse.num_args);

        final_verse.num_kwargs = qb_find_number_of_kwargs(prologue);
        final_verse.kwargs =
            qb_parse_keywords(prologue, final_verse.num_kwargs);
        free(prologue);
    }
    else {
        final_verse.args = NULL;
        final_verse.num_args = 0;
        final_verse.kwargs = NULL;
        final_verse.num_kwargs = 0;
    }

    int body_start = qb_find_next_char(verse, verse_size, offset, '{')+1;
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
    final_verse.name = qb_strip_spaces(verse, offset, prologue_start-1);
    final_verse.echo = false;

    return final_verse;
}

int qb_find_number_of_args(char *prologue){

    int prologue_size = strlen(prologue);
    int i = 0;
    int num_entries = 0;
    int next_comma = 0;
    if (qb_find_next_char(prologue, prologue_size, i, '|') > 0){
         prologue_size = qb_find_next_char(prologue, prologue_size, i, '|');
    }
    if (qb_find_next_char(prologue, prologue_size, i, ';') > 0){
        fprintf(stderr, "Semicolon (;) used instead of comma (,)!\nEach quibble arg is a single variable to be configured on function launch!");
        exit(1);
    }

    while (i < prologue_size){
        next_comma = qb_find_next_char(prologue, prologue_size, i, ',');
        if (next_comma > i){
            ++num_entries;
            i += next_comma + 1;
        }
        else {
            return 0;
        }
    }

    return num_entries;
}


int qb_find_number_of_kwargs(char *prologue){

    int prologue_size = strlen(prologue);
    int i = 0;
    int num_entries = 0;
    int next_equal = 0;
    int next_semicolon = 0;
    if (qb_find_next_char(prologue, prologue_size, i, '|') > 0){
         i += qb_find_next_char(prologue, prologue_size, i, '|') + 1;
    }
    if (qb_find_next_char(prologue, prologue_size, i, ',') > 0){
        fprintf(stderr, "Comma (,) used instead of semicolon (;)!\nEach quibble keyword is a self-contained C expression and must end with a `;`!");
        exit(1);
    }

    while (i < prologue_size){
        next_equal = qb_find_next_char(prologue, prologue_size, i, '=');
        next_semicolon = qb_find_next_char(prologue, prologue_size, i, ';');

        if (next_equal > i && next_semicolon > next_equal){
            ++num_entries;
            i += next_semicolon + 1;
        }
        else {
            return 0;
        }
    }

    return num_entries;

}

quibble_arg *qb_parse_args(char *prologue, int num_entries){

    int prologue_size = strlen(prologue);

    if (num_entries > 0){
        quibble_arg *final_args =
             (quibble_arg *)malloc(sizeof(quibble_arg) * num_entries);

        int i = 0;
        int curr_entry = 0;
        int next_comma = 0;
        if (qb_find_next_char(prologue, prologue_size, i, '|') > 0){
            prologue_size = qb_find_next_char(prologue, prologue_size, i, '|');
        }

        while (i < prologue_size){
            next_comma = qb_find_next_char(prologue, prologue_size, i, ',');

            final_args[curr_entry].variable =
                qb_strip_spaces(prologue, i, next_comma);

            // Check to make sure entry is unique
            for (int j = 1; j <= curr_entry; ++j){
                if (strcmp(final_args[j-1].variable,
                           final_args[j].variable) == 0){
                    fprintf(stderr, "Variable %s used more than once for arguments!\n", final_args[j].variable);
                    exit(1);
                }
            }

            i += next_comma + 1;
            ++curr_entry;
        }

        return final_args;
    }
    else {
        return NULL;
    }

}

quibble_keyword *qb_parse_keywords(char *prologue, int num_entries){

    int prologue_size = strlen(prologue);

    if (num_entries > 0){
        quibble_keyword *final_keywords =
             (quibble_keyword *)malloc(sizeof(quibble_keyword) * num_entries);

        int i = 0;
        int curr_entry = 0;
        int next_equal = 0;
        int next_semicolon = 0;
        if (qb_find_next_char(prologue, prologue_size, i, '|') > 0){
            i += qb_find_next_char(prologue, prologue_size, i, '|') + 1;
        }

        while (i < prologue_size){
            next_equal = qb_find_next_char(prologue, prologue_size, i, '=');
            next_semicolon = qb_find_next_char(prologue, prologue_size, i, ';');

            final_keywords[curr_entry].variable =
                qb_strip_spaces(prologue, i, next_equal);

            final_keywords[curr_entry].value =
                qb_strip_spaces(prologue, next_equal+1, next_semicolon);

            // Check to make sure entry is unique
            for (int j = 1; j <= curr_entry; ++j){
                if (strcmp(final_keywords[j-1].variable,
                           final_keywords[j].variable) == 0){
                    fprintf(stderr, "Variable %s used more than once for keyword arguments!\n", final_keywords[j].variable);
                    exit(1);
                }
            }

            i += next_semicolon + 1;
            ++curr_entry;

        }

        return final_keywords;
    }
    else {
        return NULL;
    }

}

/*----------------------------------------------------------------------------//
QBINLINE INTERFACE
//----------------------------------------------------------------------------*/

void qb_replace_char(char *verse, int verse_size, char a, char b){
    for (int i = 0; i < verse_size; ++i){
        if (verse[i] == a){
            verse[i] = b;
        }
    }
}

void qb_replace_char_if_proceeding(char *verse, int verse_size,
                                   char *prologue, int prologue_size,
                                   char a, char b){

    bool find_match = true;
    bool match_found = false;
    int count = 0;
    for (int i = 0; i < verse_size; ++i){
        while (find_match){
            if (verse[i+count] != prologue[count]){
                find_match = false;
            }

            count++;

            if (count == prologue_size && find_match){
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

bool qb_is_inlined(char *verse){
    char substr[23] = "// QBINLINE GENERATED\n";
    for (int i = 0; i < 22; ++i){
        if (verse[i] != substr[i]){
            return false;
        }
    }

    return true;
}

void qb_preprocess_content(char *content){
    if (qb_is_inlined(content)){
        int content_size = strlen(content);
        // replace all spaces by new lines
        qb_replace_char(content, content_size, ' ', '\n');

        // except for the arguments after some preprocessor options
        // that need to be in the same line
        qb_replace_char_if_proceeding(content, content_size,
                                      "#ifdef", 7, '\n', ' ');
        qb_replace_char_if_proceeding(content, content_size,
                                      "#ifndef", 8, '\n', ' ');

        // #define with two arguments will not work
        qb_replace_char_if_proceeding(content, content_size,
                                      "#define", 7, '\n', ' ');

        // don't leave any spaces in arguments
        qb_replace_char_if_proceeding(content, content_size,
                                      "#if", 3, '\n', ' ');

        // don't leave any spaces in arguments
        qb_replace_char_if_proceeding(content, content_size,
                                      "#elif", 5, '\n', ' ');
        qb_replace_char_if_proceeding(content, content_size, "#pragma",
                                      7, '\n', ' ');
    }
}

/*----------------------------------------------------------------------------//
CONFIGURATION
//----------------------------------------------------------------------------*/

int qb_find_arg_index(quibble_arg *qa, int n, char *variable){
    for (int i = 0; i < n; ++i){
        if (strcmp(qa[i].variable, variable) == 0){
            return i;
        }
    }
    fprintf(stderr, "Keyword variable %s not found!\n", variable);
    exit(1);
}

int qb_find_kwarg_index(quibble_keyword *qk, int n, char *variable){
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
    final_qv.kwargs =
        (quibble_keyword *)malloc(sizeof(quibble_keyword)*qv.num_kwargs);

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

// To be used in `qb_configure_verse` to create prologue string
char *qb_create_prologue(char *config,
                         quibble_arg *args, int num_args,
                         quibble_keyword *qkwargs, int num_kwargs){

    char *temp = (char *)calloc(MAX_PROLOGUE_SIZE, sizeof(char));

    for (int i = 0; i < num_kwargs; ++i){
        strcat(temp, qkwargs[i].variable);
        strcat(temp, " = ");
        strcat(temp, qkwargs[i].value);
        strcat(temp, ";\n");
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
void qb_free_keyword(quibble_keyword qkwarg){
    free(qkwarg.variable);
    free(qkwarg.value);
}

void qb_free_arg_array(quibble_arg *qargs, int n){
    for (int i = 0; i < n; ++i){
        qb_free_arg(qargs[i]);
    }
    free(qargs);
}

void qb_free_keyword_array(quibble_keyword *qkwargs, int n){
    for (int i = 0; i < n; ++i){
        qb_free_keyword(qkwargs[i]);
    }
    free(qkwargs);
}

void qb_free_verse(quibble_verse qv){
    if (qv.echo == false){
        free(qv.body);
        free(qv.name);
    }
    qb_free_keyword_array(qv.kwargs, qv.num_kwargs);
    qb_free_arg_array(qv.args, qv.num_args);
}

void qb_free_stanza(quibble_stanza qs){
    free(qs.prologue);
    free(qs.epilogue);
    free(qs.name);
    qb_free_keyword_array(qs.kwargs, qs.num_kwargs);
    qb_free_arg_array(qs.args, qs.num_args);
}

void qb_free_poem(quibble_poem qp){
    free(qp.body);
    free(qp.name);
    qb_free_keyword_array(qp.kwargs, qp.num_kwargs);
    qb_free_arg_array(qp.args, qp.num_args);
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
    free(qp.verse_list);
}
