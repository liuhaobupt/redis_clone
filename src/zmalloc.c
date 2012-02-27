#include "zmalloc.h"

static int zmalloc_thread_safe = 0;

void zmalloc_enable_thread_safeness(void) {
    zmalloc_thread_safe = 1;
}

char *zstrdup(const char *s) {
    size_t l = strlen(s)+1;
    char *p = malloc(l);

    memcpy(p,s,l);
    return p;
}
