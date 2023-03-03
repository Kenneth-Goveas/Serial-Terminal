/** @defgroup   stream Stream
 *
 *  @brief      Data stream handling.
 *
 *  This module contains functions for reading and writing data to both, the
 *  console as well as the serial port.
 */

#ifndef __STREAM_H__
#define __STREAM_H__

#include <sys/types.h>

/** @ingroup    stream
 *
 *  @brief      Data stream type.
 */

typedef enum {
    STREAM_TYPE_STDIO,  //!< Data stream connected to console.
    STREAM_TYPE_SERIAL  //!< Data stream connected to serial port.
} stream_type_t;

/** @ingroup    stream
 *
 *  @brief      Open data streams and configure serial port.
 *
 *  Opens the data streams for both, the console and the specified serial port,
 *  and configures the serial port.
 *
 *  @param      path    Device file path to the serial port to use.
 *  @param      baud    String representation of the serial baud rate with which
 *                      to configure the serial port.
 *
 *  @retval     0       Success.
 *  @retval     -1      Failure.
 */

int stream_open (const char * path, const char * baud);

/** @ingroup    stream
 *
 *  @brief      Revert serial port configuration and close data streams.
 *
 *  Restores the original serial port configuration and closes the data streams.
 */

void stream_close (void);

/** @ingroup    stream
 *
 *  @brief      Sleep until input becomes available.
 *
 *  Puts the program to sleep until data becomes available for read on either
 *  the console or the serial port, or if an interrupt is caught. The call may
 *  also return before any of the aforementioned events occurs, in the event of
 *  a failure.
 *
 *  @retval     0       Success.
 *  @retval     -1      Failure.
 */

int stream_wait (void);

/** @ingroup    stream
 *
 *  @brief      Get number of bytes available for read.
 *
 *  Queries the specified data stream to get the number of bytes currently
 *  available for read.
 *
 *  @param      type    The type of data stream to query.
 *
 *  @return             The number of bytes available for read.
 *  @retval     -1      Failure.
 */

ssize_t stream_available (stream_type_t type);

/** @ingroup    stream
 *
 *  @brief      Read data from stream.
 *
 *  Reads the requested number of bytes from the specified stream. If the
 *  requested number of bytes cannot immediately be read, then the call waits
 *  until the requested number of bytes have successfully been read. Thus, this
 *  call will always read the exact number of bytes requested.
 *
 *  @param      type    The type of data stream to read from.
 *  @param      data    Buffer into which data is to be read.
 *  @param      count   Number of bytes to be read.
 *
 *  @retval     0       Success.
 *  @retval     -1      Failure.
 */

int stream_read (stream_type_t type, void * data, size_t count);

/** @ingroup    stream
 *
 *  @brief      Write data to stream.
 *
 *  Writes the requested number of bytes to the specified stream. If the
 *  requested number of bytes cannot immediately be written, then the call waits
 *  until the requested number of bytes have successfully been written. Thus,
 *  this call will always write the exact number of bytes requested.
 *
 *  @param      type    The type of data stream to write to.
 *  @param      data    Buffer from which data is to be written.
 *  @param      count   Number of bytes to write.
 *
 *  @retval     0       Success.
 *  @retval     -1      Failure.
 */

int stream_write (stream_type_t type, const void * data, size_t count);

#endif
