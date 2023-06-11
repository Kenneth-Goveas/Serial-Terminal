/** @defgroup   console Console
 *
 *  @brief      Console I/O.
 *
 *  This module contains functions for reading and writing console data.
 */

#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <poll.h>

/** @ingroup    console
 *
 *  @brief      Get wakeup event structure.
 *
 *  Writes the console wakeup event into the specified buffer. This event can be
 *  registered with sleep_register_wakeup_evt() to wake up the program from a
 *  sleep when input becomes available on the console.
 *
 *  @param      evt     Pointer to wakeup event structure to be filled in.
 */

void console_get_wakeup_evt (struct pollfd * evt);

/** @ingroup    console
 *
 *  @brief      Read console input data.
 *
 *  Reads the available console input data into the specified buffer. The size
 *  of the buffer is reallocated to exactly fit all the available input data and
 *  a terminating null byte. Thus, it is recommended to initialize the buffer to
 *  `NULL` and subsequently pass a pointer to this buffer to read console input.
 *  The buffer need not be freed before the next call to this function.
 *
 *  @param      data    Pointer to buffer that must be reallocated and filled in
 *                      with available console input data.
 *
 *  @retval     0       Success.
 *  @retval     -1      Failure. Error message is written to `stderr`.
 */

int console_read_data (char ** data);

/** @ingroup    console
 *
 *  @brief      Write console output data.
 *
 *  Writes the specified buffer to console output. The buffer must be a
 *  null-terminated string.
 *
 *  @param      data    Null-terminated buffer to be written to console output.
 *
 *  @retval     0       Success.
 *  @retval     -1      Failure. Error message is written to `stderr`.
 */

int console_write_data (const char * data);

#endif
