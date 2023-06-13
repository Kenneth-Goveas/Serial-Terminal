#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <poll.h>

static int _count = 0;              // Wakeup event count.
static struct pollfd * _evt = NULL; // Wakeup event structures.

void sleep_register_wakeup_evt (struct pollfd evt) {
    // Increment wakeup event count.
    _count++;

    // Allocate space for new wakeup event.
    _evt = (struct pollfd *)realloc(_evt, _count * sizeof(struct pollfd));

    // Append new wakeup event to list of registered events.
    _evt[_count - 1].fd = evt.fd;
    _evt[_count - 1].events = evt.events;
    _evt[_count - 1].revents = evt.revents;
}

int sleep_wait_for_wakeup_evt (void) {
    int status; // Return status for API calls.

    // Wait indefinitely for any of the registered events.
    status = poll(_evt, _count, -1);
    if (status < 0) {
        // On errors other than user interrupt, exit with failure.
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
