#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include <stream.h>

// Flag to indicate if interrupt signal was caught
volatile bool intr = false;

// Interrupt signal handler
void handler (int signum) {
    // Check signal code
    // If interrupt signal, set flag
    if (signum == SIGINT) {
        intr = true;
    }
}

// Main program
void main (int argc, char ** argv) {
    char * path, * baud;    // Serial port path and baud rate
    void * data;            // Data buffer
    ssize_t count;          // Data buffer size

    // Check syntax
    // If invalid, print usage string and exit indicating failure
    if (argc != 3) {
        fprintf(
            stderr, "Usage: %s [serial port path] [baud rate]\n",
            argv[0]
        );
        exit(EXIT_FAILURE);
    }

    // Register interrupt handler
    // On error, exit indicating failure
    if (signal(SIGINT, handler) == SIG_ERR) {
        fprintf(
            stderr, "Failed to register signal handler (%s)\n",
            strerror(errno)
        );
        exit(EXIT_FAILURE);
    }

    // Read serial port path from command line argument
    path = (char *)malloc(strlen(argv[1]) + 1);
    strcpy(path, argv[1]);

    // Read baud rate from command line argument
    baud = (char *)malloc(strlen(argv[2]) + 1);
    strcpy(baud, argv[2]);

    // Open streams
    // On error, exit indicating failure
    if (stream_open(path, baud) < 0) {
        exit(EXIT_FAILURE);
    }

    // Free up memory
    free(path);
    free(baud);

    // Run main loop until interrupted by user
    while (!intr) {
        // Wait for incoming data
        // On error, close streams and exit indicating failure
        if (stream_wait() < 0) {
            stream_close();
            exit(EXIT_FAILURE);
        }

        // Read number of available bytes on serial port
        // On error, close streams and exit indicating failure
        count = stream_available(STREAM_TYPE_SERIAL);
        if (count < 0) {
            stream_close();
            exit(EXIT_FAILURE);
        }

        // If data is available, read from serial port and write to console
        // On error, close streams and exit indicating failure
        if (count > 0) {
            // Allocate data buffer
            data = (char *)malloc(count);

            // Read data from serial port
            // On error, close streams and exit indicating failure
            if (stream_read(STREAM_TYPE_SERIAL, data, count) < 0) {
                stream_close();
                exit(EXIT_FAILURE);
            }

            // Write data to console
            // On error, close streams and exit indicating failure
            if (stream_write(STREAM_TYPE_STDIO, data, count) < 0) {
                stream_close();
                exit(EXIT_FAILURE);
            }

            // Free up memory
            free(data);
        }

        // Read number of available bytes on console
        // On error, close streams and exit indicating failure
        count = stream_available(STREAM_TYPE_STDIO);
        if (count < 0) {
            stream_close();
            exit(EXIT_FAILURE);
        }

        // If data is available, read from console and write to serial port
        // On error, close streams and exit indicating failure
        if (count > 0) {
            // Allocate data buffer
            data = (char *)malloc(count);

            // Read data from console
            // On error, close streams and exit indicating failure
            if (stream_read(STREAM_TYPE_STDIO, data, count) < 0) {
                stream_close();
                exit(EXIT_FAILURE);
            }

            // Write data to serial port
            // On error, close streams and exit indicating failure
            if (stream_write(STREAM_TYPE_SERIAL, data, count) < 0) {
                stream_close();
                exit(EXIT_FAILURE);
            }

            // Free up memory
            free(data);
        }
    }

    // Close streams
    stream_close();

    // Ensure that next prompt is printed at the beginning of a line
    printf("\n");

    // Exit indicating success
    exit(EXIT_SUCCESS);
}
