#include "zmalloc.h"

static int zmalloc_thread_safe = 0;

void zmalloc_enable_thread_safeness(void) {
    zmalloc_thread_safe = 1;
}
