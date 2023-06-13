#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Line termination type.
typedef enum {
    LINE_TERM_LF,           // LF termination.
    LINE_TERM_CR,           // CR termination.
    LINE_TERM_CRLF          // CR+LF termination.
} line_term_t;

static line_term_t _iterm;  // Input line termination.
static line_term_t _oterm;  // Output line termination.

int line_set_term (const char * iterm, const char * oterm) {
    // Set input line termination.
    if (strcmp(iterm, "lf") == 0) {
        _iterm = LINE_TERM_LF;
    } else if (strcmp(iterm, "cr") == 0) {
        _iterm = LINE_TERM_CR;
    } else if (strcmp(iterm, "crlf") == 0) {
        _iterm = LINE_TERM_CRLF;
    } else {
        // If line termination is invalid, exit with failure.
        fprintf(stderr, "Unrecognized line termination '%s'\n", iterm);
        return -1;
    }

    // Set output line termination.
    if (strcmp(oterm, "lf") == 0) {
        _oterm = LINE_TERM_LF;
    } else if (strcmp(oterm, "cr") == 0) {
        _oterm = LINE_TERM_CR;
    } else if (strcmp(oterm, "crlf") == 0) {
        _oterm = LINE_TERM_CRLF;
    } else {
        // If line termination is invalid, exit with failure.
        fprintf(stderr, "Unrecognized line termination '%s'\n", oterm);
        return -1;
    }

    return 0;
}

void line_process_input_data (char ** data) {
    int count = 0;      // Character count in tepmorary buffer.
    char * proc = NULL; // Temporary buffer for processed serial input data.

    // Process serial input data in-place. No processing is required for
    // LF-terminated input.
    if (_iterm == LINE_TERM_CR) {
        // For CR termination, replace CR by LF. Temporary buffer is not
        // required for this operation.
        for (int i = 0; i < strlen(*data); i++) {
            if ((*data)[i] == '\r') {
                (*data)[i] = '\n';
            }
        }
    } else if (_iterm == LINE_TERM_CRLF) {
        // For CR+LF termination, append all characters from serial input data
        // except CR, one by one into temporary buffer.
        for (int i = 0; i < strlen(*data); i++) {
            if ((*data)[i] != '\r') {
                count++;
                proc = (char *)realloc(proc, count * sizeof(char));
                proc[count - 1] = (*data)[i];
            }
        }

        // Append null byte to temporary buffer.
        count++;
        proc = (char *)realloc(proc, count * sizeof(char));
        proc[count - 1] = '\0';

        // Replace serial input data with contents of temporary buffer.
        *data = (char *)realloc(*data, count * sizeof(char));
        strcpy(*data, proc);

        // Free up space allocated to temporary buffer.
        free(proc);
    }
}

void line_process_output_data (char ** data) {
    int count = 0;      // Character count in temporary buffer.
    char * proc = NULL; // Temporary buffer for processed serial output data.

    // Process serial output data in-place. No processing is required for
    // LF-terminated output.
    if (_oterm == LINE_TERM_CR) {
        // For CR termination, replace LF by CR. Temporary buffer is not
        // required for this operation.
        for (int i = 0; i < strlen(*data); i++) {
            if ((*data)[i] == '\n') {
                (*data)[i] = '\r';
            }
        }
    } else if (_oterm == LINE_TERM_CRLF) {
        // For CR+LF termination, append all characters from serial output data,
        // one by one into temporary buffer, appending an additional CR before
        // each LF character.
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

        // Append null byte to temporary buffer.
        count++;
        proc = (char *)realloc(proc, count * sizeof(char));
        proc[count - 1] = '\0';

        // Replace serial output data with contents of temporary buffer.
        *data = (char *)realloc(*data, count * sizeof(char));
        strcpy(*data, proc);

        // Free up space allocated to temporary buffer.
        free(proc);
    }
}
