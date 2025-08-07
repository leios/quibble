/*-------------args-----------------------------------------------------------//

 Purpose: To define all functions to deal with string -> args and prologue gen

//----------------------------------------------------------------------------*/

#include "../include/quibble.h"

void qb_find_type_arg(char *arg, char **type, char **variable){
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
        type[0] = NULL;
        char *tmp_variable = (char *)malloc((strlen(arg)+1)*sizeof(char));
        strcpy(tmp_variable, arg);
        variable[0] = tmp_variable;
    }
    else {
        index++;
        type[0] = qb_strip_spaces(arg, 0, index);
        variable[0] = qb_strip_spaces(arg, index+1, len);
    }

}

void qb_parse_pixel_kwarg(quibble_kwarg *qk, char *type){

    char *var_name = qb_strip_spaces(qk[0].variable, 0, strlen(qk[0].variable));
    char *val_name = qb_strip_spaces(qk[0].value, 0, strlen(qk[0].value));

    qb_free_kwarg(qk[0]);
    qk[0].type = qb_strip_spaces(type, 0, strlen(type));
    qk[0].variable = qb_strip_spaces(var_name,0, strlen(var_name));
    qk[0].value = qb_strip_spaces(val_name,0, strlen(val_name));

    char curr_var[128];
    char curr_val[128];
    int len = 0;
    int len_val = 0;

    sprintf(curr_var, "%s_color_type", var_name);
    sprintf(curr_var, "%s_color_type", val_name);
    len = strlen(curr_var);
    len_val = strlen(curr_val);
    
    qk[1].type = qb_strip_spaces("int", 0, strlen("int"));
    qk[1].variable = qb_strip_spaces(curr_var, 0, len);
    qk[1].value = qb_strip_spaces(curr_val, 0, len_val);

    sprintf(curr_var, "%s_width", var_name);
    sprintf(curr_var, "%s_width", val_name);
    len = strlen(curr_var);
    len_val = strlen(curr_val);

    qk[2].type = qb_strip_spaces("int", 0, strlen("int"));
    qk[2].variable = qb_strip_spaces(curr_var, 0, len);
    qk[2].value = qb_strip_spaces(curr_val, 0, len_val);

    sprintf(curr_var, "%s_height", var_name);
    sprintf(curr_var, "%s_height", val_name);
    len = strlen(curr_var);
    len_val = strlen(curr_val);

    qk[3].type = qb_strip_spaces("int", 0, strlen("int"));
    qk[3].variable = qb_strip_spaces(curr_var, 0, len);
    qk[3].value = qb_strip_spaces(curr_val, 0, len_val);

    free(var_name);
    free(val_name);
}


void qb_parse_pixel_arg(quibble_arg *qa, char *type){

    char *var_name = qb_strip_spaces(qa[0].variable, 0, strlen(qa[0].variable));
    qb_free_arg(qa[0]);
    qa[0].type = qb_strip_spaces(type, 0, strlen(type));
    qa[0].variable = qb_strip_spaces(var_name,0, strlen(var_name));

    char curr_var[128];
    int len = 0;

    sprintf(curr_var, "%s_color_type", var_name);
    len = strlen(curr_var);
    
    qa[1].type = qb_strip_spaces("int", 0, strlen("int"));
    qa[1].variable = qb_strip_spaces(curr_var, 0, len);

    sprintf(curr_var, "%s_width", var_name);
    len = strlen(curr_var);

    qa[2].type = qb_strip_spaces("int", 0, strlen("int"));
    qa[2].variable = qb_strip_spaces(curr_var, 0, len);

    sprintf(curr_var, "%s_height", var_name);
    len = strlen(curr_var);

    qa[3].type = qb_strip_spaces("int", 0, strlen("int"));
    qa[3].variable = qb_strip_spaces(curr_var, 0, len);

    free(var_name);
}

void qb_parse_arg(quibble_arg *qa, char *arg){
    qb_find_type_arg(arg, &qa->type, &qa->variable);
    free(arg);

}

void qb_parse_kwarg(quibble_kwarg *qk, char *lhs, char *rhs){

    qb_find_type_arg(lhs, &qk->type, &qk->variable);
    free(lhs);

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
    while (next_comma > 0 && i < config_size){

        if (next_comma > i){
            ++num_entries;
            i = next_comma+1;
        }
        next_comma = qb_find_next_char(config, i, ',');
    }

    // dealing with commas in kwargs
    if (i > config_size){
        --num_entries;
    }

    int num_pixels = qb_find_limited_occurrences("quibble_pixels", config_size, config);

    return num_entries+1+3*num_pixels;
}


int qb_find_number_of_kwargs(char *config){

    int i = 0;
    int num_entries = 0;

    if (qb_find_next_char(config, i, '=') < 0){
        return 0;
    }

    int pipe_loc = 0;
    if (qb_find_next_char(config, i, '|') > 0){
        pipe_loc = qb_find_next_char(config, i, '|');
        i += pipe_loc + 1;
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

    int num_pixels = qb_find_occurrences("quibble_pixels", &config[pipe_loc]);

    return num_entries+3*num_pixels;

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

            if (next_comma < 0 || next_comma > config_size){
                next_comma = config_size;
            }
            qb_parse_arg(&final_args[curr_entry],
                         qb_strip_spaces(config, i, next_comma-1));

            if (final_args[curr_entry].type != NULL &&
                strcmp(final_args[curr_entry].type,
                       "quibble_pixels_rgba8888") == 0){
                qb_parse_pixel_arg(&final_args[curr_entry],
                                   "__global quibble_color_rgba8888 *");
                curr_entry += 3;
            }
            else if (final_args[curr_entry].type != NULL &&
                     strcmp(final_args[curr_entry].type,
                            "quibble_pixels_rgb888") == 0){
                qb_parse_pixel_arg(&final_args[curr_entry],
                                   "__global quibble_color_rgb888 *");
                curr_entry += 3;
            }
            else if (final_args[curr_entry].type != NULL &&
                     strcmp(final_args[curr_entry].type,
                            "quibble_pixels_prgb888") == 0){
                qb_parse_pixel_arg(&final_args[curr_entry],
                                   "__global quibble_pcolor_rgb888 *");
                curr_entry += 3;
            }
            else if (final_args[curr_entry].type != NULL &&
                     strcmp(final_args[curr_entry].type,
                            "quibble_pixels_prgba8888") == 0){
                qb_parse_pixel_arg(&final_args[curr_entry],
                                   "__global quibble_pcolor_rgba8888 *");
                curr_entry += 3;
            }


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

            if (final_kwargs[curr_entry].type != NULL &&
                strcmp(final_kwargs[curr_entry].type,
                       "quibble_pixels_rgba8888") == 0){
                qb_parse_pixel_kwarg(&final_kwargs[curr_entry],
                                     "__global quibble_color_rgba8888 *");
                curr_entry += 3;
            }
            else if (final_kwargs[curr_entry].type != NULL &&
                     strcmp(final_kwargs[curr_entry].type,
                            "quibble_pixels_rgb888") == 0){
                qb_parse_pixel_kwarg(&final_kwargs[curr_entry],
                                     "__global quibble_color_rgb888 *");
                curr_entry += 3;
            }


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

    char *temp = (char *)calloc(QB_MAX_PROLOGUE_SIZE, sizeof(char));

    int num_config_args = qb_find_number_of_args(config);

    if (num_config_args == num_args){
        quibble_arg *temp_args = qb_parse_args(config, num_args);
        for (int i = 0; i < num_args; ++i){
            if (strcmp(qargs[i].variable, temp_args[i].variable) != 0){
                if (qargs[i].type != NULL){
                    strcat(temp, qargs[i].type);
                    strcat(temp, " ");
                }
                strcat(temp, qargs[i].variable);
                strcat(temp, " = ");
                strcat(temp, temp_args[i].variable);
                strcat(temp, ";\n");
            }
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
                if (strcmp(temp_kwargs[j].variable,
                       qkwargs[i].variable) == 0){
                    value = temp_kwargs[j].value;
                }
            }

            if (strcmp(qkwargs[i].variable, value) != 0){
                if (qkwargs[i].type != NULL){
                    strcat(temp, qkwargs[i].type);
                    strcat(temp, " ");
                }
                strcat(temp, qkwargs[i].variable);
                strcat(temp, " = ");
                strcat(temp, value);
                strcat(temp, ";\n");
            }
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
    printf("%s %s\n", qa.type, qa.variable);
}

void qb_print_kwarg(quibble_kwarg qk){
    printf("%s %s = %s\n", qk.type, qk.variable, qk.value);
}
