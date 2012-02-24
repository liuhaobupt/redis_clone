#ifndef __ZMALLOC_H
#define __ZMALLOC_H

#include <sys/types.h>

void zmalloc_enable_thread_safeness(void);

void *zmalloc(size_t size);

#endif
