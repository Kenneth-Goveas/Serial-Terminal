#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <termios.h>
#include <stream.h>

static int _fd_stdin;               // Console input file descriptor
static int _fd_stdout;              // Console output file descriptor
static int _fd_serial;              // Serial port file descriptor

static struct termios _config_old;  // Old serial port configuration
static struct termios _config_new;  // New serial port configuration

int stream_open (const char * path, const char * baud) {
    speed_t speed;  // Serial port baud rate code

    // Check baud rate
    // If valid, set appropriate baud rate code
    // If invalid, exit indicating failure
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
        fprintf(stderr, "Invalid baud rate\n");
        return -1;
    }

    // Set up console input and output file descriptors
    _fd_stdin = fileno(stdin);
    _fd_stdout = fileno(stdout);

    // Open serial port
    // On error, exit indicating failure
    _fd_serial = open(path, O_NOCTTY | O_RDWR);
    if (_fd_serial < 0) {
        fprintf(
            stderr, "Failed to open serial port (%s)\n",
            strerror(errno)
        );
        return -1;
    }

    // Retrieve old serial port configuration
    // On error, close serial port and exit indicating failure
    if (tcgetattr(_fd_serial, &_config_old) < 0) {
        fprintf(
            stderr, "Failed to retrieve serial port configuration (%s)\n",
            strerror(errno)
        );
        if (close(_fd_serial) < 0) {
            fprintf(
                stderr, "Closed serial port but error occurred (%s)\n",
                strerror(errno)
            );
        }
        return -1;
    }

    // Set up new serial port configuration

    _config_new.c_iflag = _config_old.c_iflag;
    _config_new.c_oflag = _config_old.c_oflag;
    _config_new.c_cflag = _config_old.c_cflag;
    _config_new.c_lflag = _config_old.c_lflag;

    for (int i = 0; i < NCCS; i++) {
        _config_new.c_cc[i] = _config_old.c_cc[i];
    }

    _config_new.c_iflag &= ~(
        INPCK | ISTRIP | INLCR | IGNCR | ICRNL | IXON | IXOFF | IXANY
    );
    _config_new.c_iflag |= (
        IGNBRK | IGNPAR
    );
    _config_new.c_oflag &= ~(
        OPOST | ONLCR | OCRNL | ONOCR | ONLRET | OFILL
    );
    _config_new.c_cflag &= ~(
        CSIZE | CSTOPB | PARENB
    );
    _config_new.c_cflag |= (
        CS8 | CREAD | CLOCAL
    );
    _config_new.c_lflag &= ~(
        ISIG | ICANON | ECHO
    );

    cfsetispeed(&_config_new, speed);
    cfsetospeed(&_config_new, speed);

    // Apply new serial port configuration
    // On error, close serial port and exit indicating failure
    if (tcsetattr(_fd_serial, TCSAFLUSH, &_config_new) < 0) {
        fprintf(
            stderr, "Failed to apply serial port configuration (%s)\n",
            strerror(errno)
        );
        if (close(_fd_serial) < 0) {
            fprintf(
                stderr, "Closed serial port but error occurred (%s)\n",
                strerror(errno)
            );
        }
        return -1;
    }

    return 0;
}

void stream_close (void) {
    // Revert to old serial port configuration
    if (tcsetattr(_fd_serial, TCSAFLUSH, &_config_old) < 0) {
        fprintf(
            stderr, "Failed to revert serial port configuration (%s)\n",
            strerror(errno)
        );
    }

    // Close serial port
    if (close(_fd_serial) < 0) {
        fprintf(
            stderr, "Closed serial port but error occurred (%s)\n",
            strerror(errno)
        );
    }
}

int stream_wait (void) {
    struct pollfd pfd[2];   // Poll structures for console input and serial port

    // Initialize poll structures with zeros
    memset(&(pfd[0]), 0, sizeof(struct pollfd));
    memset(&(pfd[1]), 0, sizeof(struct pollfd));

    // Enable input event in console input poll structure
    pfd[0].fd = _fd_stdin;
    pfd[0].events = POLLIN;

    // Enable input event in serial port poll structure
    pfd[1].fd = _fd_serial;
    pfd[1].events = POLLIN;

    // Wait for input on console input or serial port
    // On error other than interrupt, exit indicating failure
    if (poll(pfd, 2, -1) < 0) {
        if (errno != EINTR) {
            fprintf(
                stderr, "Failed to wait for input (%s)\n",
                strerror(errno)
            );
            return -1;
        }
    }

    return 0;
}

ssize_t stream_available (stream_type_t type) {
    int fd;     // Stream file descriptor
    int bytes;  // Number of bytes available

    // Check stream type
    // If valid, set appropriate stream file descriptor
    // If invalid, exit indicating failure
    switch (type) {
        case STREAM_TYPE_STDIO:
            fd = _fd_stdin;
            break;
        case STREAM_TYPE_SERIAL:
            fd = _fd_serial;
            break;
        default:
            fprintf(stderr, "Invalid stream type\n");
            return -1;
    }

    // Retrieve number of bytes available
    // On error, exit indicating failure
    if (ioctl(fd, FIONREAD, &bytes) < 0) {
        fprintf(
            stderr, "Failed to get input queue size (%s)\n",
            strerror(errno)
        );
        return -1;
    }

    return (ssize_t)bytes;
}

int stream_read (stream_type_t type, void * data, size_t count) {
    int fd;                     // Stream file descriptor
    ssize_t status;             // Read status code
    char * buf = (char *)data;  // Data buffer as character array

    // Check stream type
    // If valid, set appropriate stream file descriptor
    // If invalid, exit indicating failure
    switch (type) {
        case STREAM_TYPE_STDIO:
            fd = _fd_stdin;
            break;
        case STREAM_TYPE_SERIAL:
            fd = _fd_serial;
            break;
        default:
            fprintf(stderr, "Invalid stream type\n");
            return -1;
    }

    // Keep reading until buffer is full
    while (count > 0) {
        // Attempt to read remaining buffer
        // On error, exit indicating failure
        status = read(fd, buf, count);
        if (status < 0) {
            fprintf(
                stderr, "Failed to read data (%s)\n",
                strerror(errno)
            );
            return -1;
        }

        // Update pointer to remaining data and number of bytes left
        buf += status;
        count -= status;
    }

    return 0;
}

int stream_write (stream_type_t type, const void * data, size_t count) {
    int fd;                                 // Stream file descriptor
    ssize_t status;                         // Write status code
    const char * buf = (const char *)data;  // Data buffer as character array

    // Check stream type
    // If valid, set appropriate stream file descriptor
    // If invalid, exit indicating failure
    switch (type) {
        case STREAM_TYPE_STDIO:
            fd = _fd_stdout;
            break;
        case STREAM_TYPE_SERIAL:
            fd = _fd_serial;
            break;
        default:
            fprintf(stderr, "Invalid stream type\n");
            return -1;
    }

    // Keep writing until buffer is empty
    while (count > 0) {
        // Attempt to write remaining buffer
        // On error, exit indicating failure
        status = write(fd, buf, count);
        if (status < 0) {
            fprintf(
                stderr, "Failed to write data (%s)\n",
                strerror(errno)
            );
            return -1;
        }

        // Update pointer to remaining data and number of bytes left
        buf += status;
        count -= status;
    }

    return 0;
}
