#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

static int _flag_count = 0;
static char * _flag_name = NULL;
static bool ** _flag_value = NULL;

static int _param_count = 0;
static char * _param_name = NULL;
static char *** _param_value = NULL;

void option_register_flag (char name, bool * value) {
    _flag_count++;
    _flag_name = (char *)realloc(
        _flag_name, _flag_count * sizeof(char)
    );
    _flag_value = (bool **)realloc(
        _flag_value, _flag_count * sizeof(bool *)
    );
    _flag_name[_flag_count - 1] = name;
    _flag_value[_flag_count - 1] = value;
    *(_flag_value[_flag_count - 1]) = false;
}

void option_register_param (char name, char ** value) {
    _param_count++;
    _param_name = (char *)realloc(
        _param_name, _param_count * sizeof(char)
    );
    _param_value = (char ***)realloc(
        _param_value, _param_count * sizeof(char **)
    );
    _param_name[_param_count - 1] = name;
    _param_value[_param_count - 1] = value;
    *(_param_value[_param_count - 1]) = NULL;
}

int option_parse_args (int argc, char ** argv) {
    int status;
    char * opts;

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

    do {
        status = getopt(argc, argv, opts);
        if (status == '?') {
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
            for (int i = 0; i < _flag_count; i++) {
                if (status == _flag_name[i]) {
                    if (*(_flag_value[i])) {
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
                        *(_flag_value[i]) = true;
                    }
                }
            }
            for (int i = 0; i < _param_count; i++) {
                if (status == _param_name[i]) {
                    if (*(_param_value[i]) != NULL) {
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
    for (int i = 0; i < _flag_count; i++) {
        if (_flag_name[i] == name) {
            if (*(_flag_value[i])) {
                return 0;
            } else {
                fprintf(stderr, "Missing option '-%c'\n", name);
                return -1;
            }
        }
    }
    fprintf(stderr, "Option '-%c' not recognized but required\n", name);
    return -1;
}

int option_assert_param (char name) {
    for (int i = 0; i < _param_count; i++) {
        if (_param_name[i] == name) {
            if (*(_param_value[i]) != NULL) {
                return 0;
            } else {
                fprintf(stderr, "Missing option '-%c'\n", name);
                return -1;
            }
        }
    }
    fprintf(stderr, "Option '-%c' not recognized but required\n", name);
    return -1;
}
