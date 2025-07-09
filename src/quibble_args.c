/*-------------args-----------------------------------------------------------//

 Purpose: To define all functions to deal with string -> args and prologue gen

//----------------------------------------------------------------------------*/

#include "../include/quibble_args.h"

void qb_parse_arg(quibble_arg *qa, char *arg){
    int len = strlen(arg);
    bool iterate = true;
    int index = len-1;

    while (iterate){
        if (arg[index] == '*' || qb_is_space(arg[index])){
            iterate = false;
        }
        index -= 1;
        if (index < 0){
            iterate = false;
        }
    }

    // `index` should be just before the last character of the type or -1
    if (index < 0){
        qa->type = NULL;
        qa->variable = arg;
    }
    else {
        index++;
        qa->type = qb_strip_spaces(arg, 0, index);
        qa->variable = qb_strip_spaces(arg, index+1, len);
        free(arg);
    }

}

void qb_parse_kwarg(quibble_kwarg *qk, char *lhs, char *rhs){

    int len = strlen(lhs);
    bool iterate = true;
    int index = len-1;

    while (iterate){
        if (lhs[index] == '*' || qb_is_space(lhs[index])){
            iterate = false;
        }
        index -= 1;
        if (index < 0){
            iterate = false;
        }
    }

    // `index` should be just before the last character of the type or -1
    if (index < 0){
        qk->type = NULL;
        qk->variable = lhs;
    }
    else {
        index++;
        qk->type = qb_strip_spaces(lhs, 0, index);
        qk->variable = qb_strip_spaces(lhs, index+1, len);
        free(lhs);
    }

    qk->value = rhs;
}


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
            qb_parse_arg(&final_args[curr_entry],
                         qb_strip_spaces(config, i, next_comma-1));

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

            qb_parse_kwarg(&final_kwargs[curr_entry],
                           qb_strip_spaces(config, i, next_equal-1),
                           qb_strip_spaces(config,
                                           next_equal+1,
                                           next_semicolon-1));

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

// To be used in `qb_configure_verse` to create prologue string
char *qb_create_prologue(char *config, char *name,
                         quibble_arg *qargs, int num_args,
                         quibble_kwarg *qkwargs, int num_kwargs){

    char *temp = (char *)calloc(MAX_PROLOGUE_SIZE, sizeof(char));

    int num_config_args = qb_find_number_of_args(config);

    if (num_config_args == num_args){
        quibble_arg *temp_args = qb_parse_args(config, num_args);
        for (int i = 0; i < num_args; ++i){
            if (qargs[i].type != NULL){
                strcat(temp, qargs[i].type);
            }
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
                // Note that the + strlen(temp_kwargs[j].variable) offsets the
                // pointer so we will grab only the variable name and not the
                // type definition, so we are comparing x == ~float ~x
                if (strcmp(temp_kwargs[j].variable,
                       qkwargs[i].variable + strlen(temp_kwargs[j].variable)
                          ) == 0){
                    value = temp_kwargs[i].value;
                }
            }

            if (qkwargs[i].type != NULL){
                strcat(temp, qkwargs[i].type);
                strcat(temp, " ");
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
            if (qkwargs[i].type != NULL){
                strcat(temp, qkwargs[i].type);
                strcat(temp, " ");
            }
            strcat(temp, qkwargs[i].variable);
            strcat(temp, " = ");
            strcat(temp, qkwargs[i].value);
            strcat(temp, ";\n");
        }
    }

    int len = strlen(temp);
    char *final_output = (char *)calloc(len+1, sizeof(char));

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
    free(qarg.type);
    free(qarg.variable);
}

void qb_free_kwarg(quibble_kwarg qkwarg){
    free(qkwarg.type);
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

/*----------------------------------------------------------------------------//
    PRINTING
//----------------------------------------------------------------------------*/

void qb_print_arg(quibble_arg qa){
    printf("%s\n", qa.variable);
}

void qb_print_kwarg(quibble_kwarg qk){
    printf("%s = %s\n", qk.variable, qk.value);
}
