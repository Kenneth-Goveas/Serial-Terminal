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

volatile bool intr = false;

void handler (int signum) {
    if (signum == SIGINT) {
        intr = true;
    }
}

void main (int argc, char ** argv) {
    int status;
    bool help;
    char * port, * baud, * iterm, * oterm;
    struct pollfd evt;
    char * data = NULL;

    option_register_flag('h', &help);
    option_register_param('p', &port);
    option_register_param('b', &baud);
    option_register_param('i', &iterm);
    option_register_param('o', &oterm);

    status = option_parse_args(argc, argv);
    if (status < 0) {
        fprintf(stderr, "Try '%s -h' for more information\n", argv[0]);
        exit(EXIT_FAILURE);
    }

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

    status = option_assert_param('p');
    if (status < 0) {
        fprintf(stderr, "Try '%s -h' for more information\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    status = option_assert_param('b');
    if (status < 0) {
        fprintf(stderr, "Try '%s -h' for more information\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    status = option_assert_param('i');
    if (status < 0) {
        fprintf(stderr, "Try '%s -h' for more information\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    status = option_assert_param('o');
    if (status < 0) {
        fprintf(stderr, "Try '%s -h' for more information\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (signal(SIGINT, handler) == SIG_ERR) {
        fprintf(
            stderr, "Failed to register signal handler (%s)\n",
            strerror(errno)
        );
        exit(EXIT_FAILURE);
    }

    status = line_set_term(iterm, oterm);
    if (status < 0) {
        exit(EXIT_FAILURE);
    }

    status = serial_open_port(port, baud);
    if (status < 0) {
        exit(EXIT_FAILURE);
    }

    serial_get_wakeup_evt(&evt);
    sleep_register_wakeup_evt(evt);

    console_get_wakeup_evt(&evt);
    sleep_register_wakeup_evt(evt);

    while (!intr) {
        status = sleep_wait_for_wakeup_evt();
        if (status < 0) {
            serial_close_port();
            exit(EXIT_FAILURE);
        }

        status = serial_read_data(&data);
        if (status < 0) {
            serial_close_port();
            exit(EXIT_FAILURE);
        }

        line_process_input_data(&data);

        status = console_write_data(data);
        if (status < 0) {
            serial_close_port();
            exit(EXIT_FAILURE);
        }

        status = console_read_data(&data);
        if (status < 0) {
            serial_close_port();
            exit(EXIT_FAILURE);
        }

        line_process_output_data(&data);

        status = serial_write_data(data);
        if (status < 0) {
            serial_close_port();
            exit(EXIT_FAILURE);
        }
    }

    serial_close_port();

    printf("\n");

    exit(EXIT_SUCCESS);
}
