#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <poll.h>

void console_get_wakeup_evt (struct pollfd * evt) {
    memset(evt, 0, sizeof(struct pollfd));
    evt->fd = fileno(stdin);
    evt->events = POLLIN;
}

int console_read_data (char ** data) {
    ssize_t status;
    char * buf;
    int count;

    status = ioctl(fileno(stdin), FIONREAD, &count);
    if (status < 0) {
        fprintf(
            stderr, "Failed to obtain console input queue size (%s)\n",
            strerror(errno)
        );
        return -1;
    }

    *data = (char *)realloc(*data, (count + 1) * sizeof(char));
    (*data)[count] = '\0';

    buf = *data;
    while (count > 0) {
        status = read(fileno(stdin), buf, count);
        if (status < 0) {
            fprintf(
                stderr, "Failed to read console data (%s)\n",
                strerror(errno)
            );
            return -1;
        }
        buf += status;
        count -= status;
    }

    return 0;
}

int console_write_data (const char * data) {
    ssize_t status;
    size_t count;

    count = strlen(data);

    while (count > 0) {
        status = write(fileno(stdout), data, count);
        if (status < 0) {
            fprintf(
                stderr, "Failed to write console data (%s)\n",
                strerror(errno)
            );
            return -1;
        }
        data += status;
        count -= status;
    }

    return 0;
}
