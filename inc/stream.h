#ifndef __STREAM_H__
#define __STREAM_H__

#include <sys/types.h>

typedef enum {
    STREAM_TYPE_STDIO,
    STREAM_TYPE_SERIAL
} stream_type_t;

int stream_open (const char * path, const char * baud);
void stream_close (void);
int stream_wait (void);
ssize_t stream_available (stream_type_t type);
int stream_read (stream_type_t type, void * data, size_t count);
int stream_write (stream_type_t type, const void * data, size_t count);

#endif
