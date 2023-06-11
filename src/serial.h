/** @defgroup   serial  Serial
 *
 *  @brief      Serial I/O.
 *
 *  This module contains functions for reading and writing serial data.
 */

#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <poll.h>

/** @ingroup    serial
 *
 *  @brief      Open and configure serial port.
 *
 *  Opens the serial port with the specified path and configures it to operate
 *  at the specified baud rate.
 *
 *  @note       The specified baud rate must be supported by the system.
 *
 *  @param      port    Path to the serial port.
 *  @param      baud    String representation of baud rate for communication.
 *
 *  @retval     0       Success.
 *  @retval     -1      Failure. Error message is written to `stderr`.
 */

int serial_open_port (const char * port, const char * baud);

/** @ingroup    serial
 *
 *  @brief      Close serial port.
 *
 *  Closes the serial port and restores its original configuration.
 */

void serial_close_port (void);

/** @ingroup    serial
 *
 *  @brief      Get wakeup event structure.
 *
 *  Writes the serial wakeup event into the specified buffer. This event can be
 *  registered with sleep_register_wakeup_evt() to wake up the program from a
 *  sleep when input becomes available on the serial port.
 *
 *  @note       The serial port must be opened with a successful call to
 *              serial_open_port() before calling this function.
 *
 *  @param      evt     Pointer to wakeup event structure to be filled in.
 */

void serial_get_wakeup_evt (struct pollfd * evt);

/** @ingroup    serial
 *
 *  @brief      Read serial input data.
 *
 *  Reads the available serial input data into the specified buffer. The size of
 *  the buffer is reallocated to exactly fit all the available input data and a
 *  terminating null byte. Thus, it is recommended to initialize the buffer to
 *  `NULL` and subsequently pass a pointer to this buffer to read serial input.
 *  The buffer need not be freed before the next call to this function.
 *
 *  @param      data    Pointer to buffer that must be reallocated and filled in
 *                      with available serial input data.
 *
 *  @retval     0       Success.
 *  @retval     -1      Failure. Error message is written to `stderr`.
 */

int serial_read_data (char ** data);

/** @ingroup    serial
 *
 *  @brief      Write serial output data.
 *
 *  Writes the specified buffer to serial output. The buffer must be a
 *  null-terminated string.
 *
 *  @param      data    Null-terminated buffer to be written to serial output.
 *
 *  @retval     0       Success.
 *  @retval     -1      Failure. Error message is written to `stderr`.
 */

int serial_write_data (const char * data);

#endif
