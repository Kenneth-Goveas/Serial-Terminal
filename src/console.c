#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <poll.h>

void console_get_wakeup_evt (struct pollfd * evt) {
    // Initialize wakeup event structure with zeros.
    memset(evt, 0, sizeof(struct pollfd));

    // Configure wakeup event to trigger when input arrives on standard input.
    evt->fd = fileno(stdin);
    evt->events = POLLIN;
}

int console_read_data (char ** data) {
    ssize_t status; // Return status for API calls.
    char * buf;     // Pointer to current location in buffer.
    int count;      // Number of characters to read.

    // Get console input queue size.
    status = ioctl(fileno(stdin), FIONREAD, &count);
    if (status < 0) {
        // On error, exit with failure.
        fprintf(
            stderr, "Failed to obtain console input queue size (%s)\n",
            strerror(errno)
        );
        return -1;
    }

    // Allocate buffer to fit input data and terminating null byte.
    *data = (char *)realloc(*data, (count + 1) * sizeof(char));
    (*data)[count] = '\0';

    // Read input recursively until buffer is full.
    buf = *data;
    while (count > 0) {
        // Read input.
        status = read(fileno(stdin), buf, count);
        if (status < 0) {
            // On error, exit with failure.
            fprintf(
                stderr, "Failed to read console data (%s)\n",
                strerror(errno)
            );
            return -1;
        }
        // Update current location in buffer, and number of remaining input
        // characters to read.
        buf += status;
        count -= status;
    }

    return 0;
}

int console_write_data (const char * data) {
    ssize_t status; // Return status for API calls.
    size_t count;   // Number of characters to write.

    // Get buffer size.
    count = strlen(data);

    // Write output recursively until buffer is empty.
    while (count > 0) {
        // Write output.
        status = write(fileno(stdout), data, count);
        if (status < 0) {
            // On error, exit with failure.
            fprintf(
                stderr, "Failed to write console data (%s)\n",
                strerror(errno)
            );
            return -1;
        }
        // Update current location in buffer, and number of remaining output
        // characters to write.
        data += status;
        count -= status;
    }

    return 0;
}
