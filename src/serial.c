#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <termios.h>

static int _fd = 0;             // Serial port file descriptor.

static struct termios _cnf_old; // Old serial port configuration.
static struct termios _cnf_new; // New serial port configuration.

int serial_open_port (const char * port, const char * baud) {
    int status;     // Return status for API calls.
    speed_t speed;  // Buad rate specifier.

    // Get baud rate specifier.
    if (strcmp(baud, "50") == 0) {
        speed = B50;
    } else if (strcmp(baud, "75") == 0) {
        speed = B75;
    } else if (strcmp(baud, "110") == 0) {
        speed = B110;
    } else if (strcmp(baud, "134") == 0) {
        speed = B134;
    } else if (strcmp(baud, "150") == 0) {
        speed = B150;
    } else if (strcmp(baud, "200") == 0) {
        speed = B200;
    } else if (strcmp(baud, "300") == 0) {
        speed = B300;
    } else if (strcmp(baud, "600") == 0) {
        speed = B600;
    } else if (strcmp(baud, "1200") == 0) {
        speed = B1200;
    } else if (strcmp(baud, "1800") == 0) {
        speed = B1800;
    } else if (strcmp(baud, "2400") == 0) {
        speed = B2400;
    } else if (strcmp(baud, "4800") == 0) {
        speed = B4800;
    } else if (strcmp(baud, "9600") == 0) {
        speed = B9600;
    } else if (strcmp(baud, "19200") == 0) {
        speed = B19200;
    } else if (strcmp(baud, "38400") == 0) {
        speed = B38400;
    } else if (strcmp(baud, "57600") == 0) {
        speed = B57600;
    } else if (strcmp(baud, "115200") == 0) {
        speed = B115200;
    } else if (strcmp(baud, "230400") == 0) {
        speed = B230400;
    } else if (strcmp(baud, "460800") == 0) {
        speed = B460800;
    } else if (strcmp(baud, "500000") == 0) {
        speed = B500000;
    } else if (strcmp(baud, "576000") == 0) {
        speed = B576000;
    } else if (strcmp(baud, "921600") == 0) {
        speed = B921600;
    } else if (strcmp(baud, "1000000") == 0) {
        speed = B1000000;
    } else if (strcmp(baud, "1152000") == 0) {
        speed = B1152000;
    } else if (strcmp(baud, "1500000") == 0) {
        speed = B1500000;
    } else if (strcmp(baud, "2000000") == 0) {
        speed = B2000000;
    } else if (strcmp(baud, "2500000") == 0) {
        speed = B2500000;
    } else if (strcmp(baud, "3000000") == 0) {
        speed = B3000000;
    } else if (strcmp(baud, "3500000") == 0) {
        speed = B3500000;
    } else if (strcmp(baud, "4000000") == 0) {
        speed = B4000000;
    } else {
        // If unsupported, exit with failure.
        fprintf(stderr, "Unsupported baud rate '%s'\n", baud);
        return -1;
    }

    // Open serial port.
    _fd = open(port, O_NOCTTY | O_RDWR);
    if (_fd < 0) {
        // On error, exit with failure.
        fprintf(
            stderr, "Failed to open serial port (%s)\n",
            strerror(errno)
        );
        return -1;
    }

    // Get old serial port configuration.
    status = tcgetattr(_fd, &_cnf_old);
    if (status < 0) {
        // On error, close serial port and exit with failure.
        fprintf(
            stderr, "Failed to obtain serial port configuration (%s)\n",
            strerror(errno)
        );
        status = close(_fd);
        if (status < 0) {
            fprintf(
                stderr, "Closed serial port but error occurred (%s)\n",
                strerror(errno)
            );
        }
        return -1;
    }

    // Prepare new serial port configuration structure.

    _cnf_new.c_iflag = _cnf_old.c_iflag;
    _cnf_new.c_oflag = _cnf_old.c_oflag;
    _cnf_new.c_cflag = _cnf_old.c_cflag;
    _cnf_new.c_lflag = _cnf_old.c_lflag;

    for (int i = 0; i < NCCS; i++) {
        _cnf_new.c_cc[i] = _cnf_old.c_cc[i];
    }

    _cnf_new.c_iflag &= ~(
        INPCK | ISTRIP | INLCR | IGNCR | ICRNL | IXON | IXOFF | IXANY
    );
    _cnf_new.c_iflag |= (
        IGNBRK | IGNPAR
    );
    _cnf_new.c_oflag &= ~(
        OPOST | ONLCR | OCRNL | ONOCR | ONLRET | OFILL
    );
    _cnf_new.c_cflag &= ~(
        CSIZE | CSTOPB | PARENB
    );
    _cnf_new.c_cflag |= (
        CS8 | CREAD | CLOCAL
    );
    _cnf_new.c_lflag &= ~(
        ISIG | ICANON | ECHO
    );

    cfsetispeed(&_cnf_new, speed);
    cfsetospeed(&_cnf_new, speed);

    // Set new serial port configuration.
    status = tcsetattr(_fd, TCSAFLUSH, &_cnf_new);
    if (status < 0) {
        // On error, close serial port and exit with failure.
        fprintf(
            stderr, "Failed to apply serial port configuration (%s)\n",
            strerror(errno)
        );
        status = close(_fd);
        if (status < 0) {
            fprintf(
                stderr, "Closed serial port but error occurred (%s)\n",
                strerror(errno)
            );
        }
        return -1;
    }

    return 0;
}

void serial_close_port (void) {
    int status; // Return status for API calls.

    // Set old serial port configuration.
    status = tcsetattr(_fd, TCSAFLUSH, &_cnf_old);
    if (status < 0) {
        fprintf(
            stderr, "Failed to revert serial port configuration (%s)\n",
            strerror(errno)
        );
    }

    // Close serial port.
    status = close(_fd);
    if (status < 0) {
        fprintf(
            stderr, "Closed serial port but error occurred (%s)\n",
            strerror(errno)
        );
    }
}

void serial_get_wakeup_evt (struct pollfd * evt) {
    // Initialize wakeup event structure with zeros.
    memset(evt, 0, sizeof(struct pollfd));

    // Configure wakeup event to trigger when input arrives on serial port.
    evt->fd = _fd;
    evt->events = POLLIN;
}

int serial_read_data (char ** data) {
    ssize_t status; // Return status for API calls.
    char * buf;     // Pointer to current location in buffer.
    int count;      // Number of characters to read.

    // Get serial input queue size.
    status = ioctl(_fd, FIONREAD, &count);
    if (status < 0) {
        // On error, exit with failure.
        fprintf(
            stderr, "Failed to obtain serial input queue size (%s)\n",
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
        status = read(_fd, buf, count);
        if (status < 0) {
            // On error, exit with failure.
            fprintf(
                stderr, "Failed to read serial data (%s)\n",
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

int serial_write_data (const char * data) {
    ssize_t status; // Return status for API calls.
    size_t count;   // Number of characters to write.

    // Get buffer size.
    count = strlen(data);

    // Write output recursively until buffer is empty.
    while (count > 0) {
        // Write output.
        status = write(_fd, data, count);
        if (status < 0) {
            // On error, exit with failure.
            fprintf(
                stderr, "Failed to write serial data (%s)\n",
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
