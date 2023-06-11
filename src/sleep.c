#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <poll.h>

static int _count = 0;
static struct pollfd * _evt = NULL;

void sleep_register_wakeup_evt (struct pollfd evt) {
    _count++;
    _evt = (struct pollfd *)realloc(_evt, _count * sizeof(struct pollfd));
    _evt[_count - 1].fd = evt.fd;
    _evt[_count - 1].events = evt.events;
    _evt[_count - 1].revents = evt.revents;
}

int sleep_wait_for_wakeup_evt (void) {
    int status;

    status = poll(_evt, _count, -1);
    if (status < 0) {
        if (errno != EINTR) {
            fprintf(
                stderr, "Failed to wait for wakeup event (%s)\n",
                strerror(errno)
            );
            return -1;
        }
    }

    return 0;
}
