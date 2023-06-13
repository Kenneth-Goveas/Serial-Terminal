/** @mainpage   Serial Terminal
 *
 *  @brief      Simple command line tool for serial communication over a UART.
 *
 *  *Serial Terminal* is a simple command line tool for Linux, for communicating
 *  with devices connected over a UART. Received data is printed on screen and
 *  user input is transmitted to the connected device.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <poll.h>

#include "option.h"
#include "line.h"
#include "serial.h"
#include "console.h"
#include "sleep.h"

volatile bool intr = false; // Flag indicating if user interrupt was received.

// Interrupt signal handler.
void handler (int signum) {
    // If interrupt signal was received, set flag to `true`.
    if (signum == SIGINT) {
        intr = true;
    }
}

void main (int argc, char ** argv) {
    int status;                             // Return status for API calls.
    bool help;                              // Command line boolean flags.
    char * port, * baud, * iterm, * oterm;  // Command line string parameters.
    struct pollfd evt;                      // Wakeup event structure.
    char * data = NULL;                     // Data buffer.

    // Register command line options.
    option_register_flag('h', &help);       // Help page.
    option_register_param('p', &port);      // Path to serial port.
    option_register_param('b', &baud);      // Baud rate for communication.
    option_register_param('i', &iterm);     // Input line termination.
    option_register_param('o', &oterm);     // Output line termination.

    // Parse command line arguments.
    status = option_parse_args(argc, argv);
    if (status < 0) {
        // On error, exit with failure.
        fprintf(stderr, "Try '%s -h' for more information\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // If requested, display help page and exit with success.
    if (help) {
        printf(
            "\n"
            "Usage: %s [-h] [-p <port>] [-b <baud>] [-i <iterm>] [-o <oterm>]\n"
            "\n"
            "Options:\n"
            "\n"
            "  -h           Show this help page and exit.\n"
            "\n"
            "  -p <port>    Path to serial port. Here, <port> must be a valid\n"
            "               device path corresponding to a serial port.\n"
            "\n"
            "  -b <baud>    Baud rate for communication. Here, <baud> must be\n"
            "               a baud rate supported by Linux.\n"
            "\n"
            "  -i <iterm>   Input line termination. Here, <iterm> must be\n"
            "               'cr', 'lf', or 'crlf', whichever correctly\n"
            "               represents the input line termination character.\n"
            "\n"
            "  -o <oterm>   Output line termination. Here, <oterm> must be\n"
            "               'cr', 'lf', or 'crlf', whichever correctly\n"
            "               represents the output line termination character.\n"
            "\n",
            argv[0]
        );
        exit(EXIT_SUCCESS);
    }

    // Assert that path to serial port is specified.
    status = option_assert_param('p');
    if (status < 0) {
        // On error, exit with failure.
        fprintf(stderr, "Try '%s -h' for more information\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Assert that baud rate for communication is specified.
    status = option_assert_param('b');
    if (status < 0) {
        // On error, exit with failure.
        fprintf(stderr, "Try '%s -h' for more information\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Assert that input line termination is specified.
    status = option_assert_param('i');
    if (status < 0) {
        // On error, exit with failure.
        fprintf(stderr, "Try '%s -h' for more information\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Assert that output line termination is specified.
    status = option_assert_param('o');
    if (status < 0) {
        // On error, exit with failure.
        fprintf(stderr, "Try '%s -h' for more information\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Register interrupt signal handler.
    if (signal(SIGINT, handler) == SIG_ERR) {
        // On error, exit with failure.
        fprintf(
            stderr, "Failed to register signal handler (%s)\n",
            strerror(errno)
        );
        exit(EXIT_FAILURE);
    }

    // Configure line terminations.
    status = line_set_term(iterm, oterm);
    if (status < 0) {
        // On error, exit with failure.
        exit(EXIT_FAILURE);
    }

    // Open serial port.
    status = serial_open_port(port, baud);
    if (status < 0) {
        // On error, exit with failure.
        exit(EXIT_FAILURE);
    }

    // Register serial wakeup event.
    serial_get_wakeup_evt(&evt);
    sleep_register_wakeup_evt(evt);

    // Reister console wakeup event.
    console_get_wakeup_evt(&evt);
    sleep_register_wakeup_evt(evt);

    // Run serial terminal until interrupted.
    while (!intr) {
        // Wait for wakeup events.
        status = sleep_wait_for_wakeup_evt();
        if (status < 0) {
            // On error, close serial port and exit with failure.
            serial_close_port();
            exit(EXIT_FAILURE);
        }

        // Read serial data.
        status = serial_read_data(&data);
        if (status < 0) {
            // On error, close serial port and exit with failure.
            serial_close_port();
            exit(EXIT_FAILURE);
        }

        // Translate line terminations.
        line_process_input_data(&data);

        // Write data to console.
        status = console_write_data(data);
        if (status < 0) {
            // On error, close serial port and exit with failure.
            serial_close_port();
            exit(EXIT_FAILURE);
        }

        // Read console data.
        status = console_read_data(&data);
        if (status < 0) {
            // On error, close serial port and exit with failure.
            serial_close_port();
            exit(EXIT_FAILURE);
        }

        // Translate line terminations.
        line_process_output_data(&data);

        // Write data to serial port.
        status = serial_write_data(data);
        if (status < 0) {
            // On error, close serial port and exit with failure.
            serial_close_port();
            exit(EXIT_FAILURE);
        }
    }

    // Close serial port.
    serial_close_port();

    // Ensure that shell prompt string appears at the beginning of a new line.
    printf("\n");

    exit(EXIT_SUCCESS);
}
