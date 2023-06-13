#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

static int _flag_count = 0;             // Boolean flag count.
static char * _flag_name = NULL;        // Boolean flag names.
static bool ** _flag_value = NULL;      // Boolean flag values.

static int _param_count = 0;            // String parameter count.
static char * _param_name = NULL;       // String parameter names.
static char *** _param_value = NULL;    // String parameter values.

void option_register_flag (char name, bool * value) {
    // Increment boolean flag count.
    _flag_count++;

    // Allocate space for new boolean flag name and value.
    _flag_name = (char *)realloc(
        _flag_name, _flag_count * sizeof(char)
    );
    _flag_value = (bool **)realloc(
        _flag_value, _flag_count * sizeof(bool *)
    );

    // Append new boolean flag to list of registered boolean flags.
    _flag_name[_flag_count - 1] = name;
    _flag_value[_flag_count - 1] = value;

    // Initialise boolean flag value to `false`. This is the default value if
    // the option is not passed.
    *(_flag_value[_flag_count - 1]) = false;
}

void option_register_param (char name, char ** value) {
    // Increment string parameter count.
    _param_count++;

    // Allocate space for new string parameter name and value.
    _param_name = (char *)realloc(
        _param_name, _param_count * sizeof(char)
    );
    _param_value = (char ***)realloc(
        _param_value, _param_count * sizeof(char **)
    );

    // Append new string parameter to list of registered string parameters.
    _param_name[_param_count - 1] = name;
    _param_value[_param_count - 1] = value;

    // Initialise string parameter value to `NULL`. This is the default value if
    // the option is not passed.
    *(_param_value[_param_count - 1]) = NULL;
}

int option_parse_args (int argc, char ** argv) {
    int status;     // Return status for API calls.
    char * opts;    // Accepted option description string.

    // Construct option description string
    opts = (char *)malloc((2 + _flag_count + 2 * _param_count) * sizeof(char));
    memset(opts, '\0', (2 + _flag_count + 2 * _param_count));
    opts[strlen(opts)] = ':';
    for (int i = 0; i < _flag_count; i++) {
        opts[strlen(opts)] = _flag_name[i];
    }
    for (int i = 0; i < _param_count; i++) {
        opts[strlen(opts)] = _param_name[i];
        opts[strlen(opts)] = ':';
    }

    // Parse options recursively.
    do {
        // Parse next option.
        status = getopt(argc, argv, opts);
        if (status == '?') {
            // If unrecognized, exit with failure.
            free(opts);
            for (int i = 0; i < _flag_count; i++) {
                *(_flag_value[i]) = false;
            }
            for (int i = 0; i < _param_count; i++) {
                free(*(_param_value[i]));
                *(_param_value[i]) = NULL;
            }
            fprintf(stderr, "Unrecognized option '-%c'\n", optopt);
            return -1;
        } else if (status == ':') {
            // If required argument is missing, exit with failure.
            free(opts);
            for (int i = 0; i < _flag_count; i++) {
                *(_flag_value[i]) = false;
            }
            for (int i = 0; i < _param_count; i++) {
                free(*(_param_value[i]));
                *(_param_value[i]) = NULL;
            }
            fprintf(stderr, "Missing argument for option '-%c'\n", optopt);
            return -1;
        } else {
            // Find matching boolean flag option if any.
            for (int i = 0; i < _flag_count; i++) {
                if (status == _flag_name[i]) {
                    if (*(_flag_value[i])) {
                        // If option was repeated, exit with failure.
                        free(opts);
                        for (int i = 0; i < _flag_count; i++) {
                            *(_flag_value[i]) = false;
                        }
                        for (int i = 0; i < _param_count; i++) {
                            free(*(_param_value[i]));
                            *(_param_value[i]) = NULL;
                        }
                        fprintf(stderr, "Repeated option '-%c'\n", status);
                        return -1;
                    } else {
                        // Set boolean flag value to `true`.
                        *(_flag_value[i]) = true;
                    }
                }
            }
            // Find matching string parameter option if any.
            for (int i = 0; i < _param_count; i++) {
                if (status == _param_name[i]) {
                    if (*(_param_value[i]) != NULL) {
                        // If option was repeated, exit with failure.
                        free(opts);
                        for (int i = 0; i < _flag_count; i++) {
                            *(_flag_value[i]) = false;
                        }
                        for (int i = 0; i < _param_count; i++) {
                            free(*(_param_value[i]));
                            *(_param_value[i]) = NULL;
                        }
                        fprintf(stderr, "Repeated option '-%c'\n", status);
                        return -1;
                    } else {
                        if (optarg[0] == '-') {
                            // If argument is, in fact, the next option, treat
                            // it as a missing argument case, and exit with
                            // failure.
                            free(opts);
                            for (int i = 0; i < _flag_count; i++) {
                                *(_flag_value[i]) = false;
                            }
                            for (int i = 0; i < _param_count; i++) {
                                free(*(_param_value[i]));
                                *(_param_value[i]) = NULL;
                            }
                            fprintf(
                                stderr, "Missing argument for option '-%c'\n",
                                status
                            );
                            return -1;
                        }
                        // Set string parameter value to the argument passed.
                        *(_param_value[i]) = (char *)malloc(
                            (strlen(optarg) + 1) * sizeof(char)
                        );
                        strcpy(*(_param_value[i]), optarg);
                    }
                }
            }
        }
    } while (status != -1);

    if (optind != argc) {
        // If extra arguments are passed, exit with failure.
        free(opts);
        for (int i = 0; i < _flag_count; i++) {
            *(_flag_value[i]) = false;
        }
        for (int i = 0; i < _param_count; i++) {
            free(*(_param_value[i]));
            *(_param_value[i]) = NULL;
        }
        fprintf(stderr, "Too many arguments\n");
        return -1;
    }

    return 0;
}

int option_assert_flag (char name) {
    // Find matching boolean flag option if any.
    for (int i = 0; i < _flag_count; i++) {
        if (_flag_name[i] == name) {
            if (*(_flag_value[i])) {
                // If option was passed, exit with success.
                return 0;
            } else {
                // Exit with failure.
                fprintf(stderr, "Missing option '-%c'\n", name);
                return -1;
            }
        }
    }
    // If option is not registered, exit with failure.
    fprintf(stderr, "Option '-%c' not recognized but required\n", name);
    return -1;
}

int option_assert_param (char name) {
    // Find matching string parameter option if any.
    for (int i = 0; i < _param_count; i++) {
        if (_param_name[i] == name) {
            if (*(_param_value[i]) != NULL) {
                // If option was passed, exit with success.
                return 0;
            } else {
                // Exit with failure.
                fprintf(stderr, "Missing option '-%c'\n", name);
                return -1;
            }
        }
    }
    // If option is not registered, exit with failure.
    fprintf(stderr, "Option '-%c' not recognized but required\n", name);
    return -1;
}
