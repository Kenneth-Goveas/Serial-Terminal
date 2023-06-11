#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    LINE_TERM_LF,
    LINE_TERM_CR,
    LINE_TERM_CRLF
} line_term_t;

static line_term_t _iterm;
static line_term_t _oterm;

int line_set_term (const char * iterm, const char * oterm) {
    if (strcmp(iterm, "lf") == 0) {
        _iterm = LINE_TERM_LF;
    } else if (strcmp(iterm, "cr") == 0) {
        _iterm = LINE_TERM_CR;
    } else if (strcmp(iterm, "crlf") == 0) {
        _iterm = LINE_TERM_CRLF;
    } else {
        fprintf(stderr, "Unrecognized line termination '%s'\n", iterm);
        return -1;
    }

    if (strcmp(oterm, "lf") == 0) {
        _oterm = LINE_TERM_LF;
    } else if (strcmp(oterm, "cr") == 0) {
        _oterm = LINE_TERM_CR;
    } else if (strcmp(oterm, "crlf") == 0) {
        _oterm = LINE_TERM_CRLF;
    } else {
        fprintf(stderr, "Unrecognized line termination '%s'\n", oterm);
        return -1;
    }

    return 0;
}

void line_process_input_data (char ** data) {
    int count = 0;
    char * proc = NULL;

    if (_iterm == LINE_TERM_CR) {
        for (int i = 0; i < strlen(*data); i++) {
            if ((*data)[i] == '\r') {
                (*data)[i] = '\n';
            } 
        }
    } else if (_iterm == LINE_TERM_CRLF) {
        for (int i = 0; i < strlen(*data); i++) {
            if ((*data)[i] != '\r') {
                count++;
                proc = (char *)realloc(proc, count * sizeof(char));
                proc[count - 1] = (*data)[i];
            }
        }

        count++;
        proc = (char *)realloc(proc, count * sizeof(char));
        proc[count - 1] = '\0';

        *data = (char *)realloc(*data, count * sizeof(char));
        strcpy(*data, proc);

        free(proc);
    }
}

void line_process_output_data (char ** data) {
    int count = 0;
    char * proc = NULL;

    if (_oterm == LINE_TERM_CR) {
        for (int i = 0; i < strlen(*data); i++) {
            if ((*data)[i] == '\n') {
                (*data)[i] = '\r';
            }
        }
    } else if (_oterm == LINE_TERM_CRLF) {
        for (int i = 0; i < strlen(*data); i++) {
            if ((*data)[i] == '\n') {
                count += 2;
                proc = (char *)realloc(proc, count * sizeof(char));
                proc[count - 2] = '\r';
                proc[count - 1] = '\n';
            } else {
                count++;
                proc = (char *)realloc(proc, count * sizeof(char));
                proc[count - 1] = (*data)[i];
            }
        }

        count++;
        proc = (char *)realloc(proc, count * sizeof(char));
        proc[count - 1] = '\0';

        *data = (char *)realloc(*data, count * sizeof(char));
        strcpy(*data, proc);

        free(proc);
    }
}
