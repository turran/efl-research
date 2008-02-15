#ifndef _LIBIRC_PORTABLE_H
#define _LIBIRC_PORTABLE_H

#include <pthread.h>

typedef pthread_mutex_t   port_mutex_t;

int libirc_mutex_init (port_mutex_t * mutex);
void libirc_mutex_destroy (port_mutex_t * mutex);
void libirc_mutex_lock (port_mutex_t * mutex);
void libirc_mutex_unlock (port_mutex_t * mutex);

#endif
