/* anet.c -- Basic TCP socket stuff made a bit less boring
 *
 */

#include "anet.h"

#include <stdarg.h>

static void anetSetError(char *err, const char *fmt, ...)
{
    va_list ap;

    if (!err) return;

    va_start(ap, fmt);
    vsnprintf(err, ANET_ERR_LEN, fmt, ap);
    va_end(ap);
}

static int anetCreateSocket(char *err, int domain) 
{
    int s, on = 1;
    if ((s = socket(domain, SOCK_STREAM, 0)) == -1 )
    {
        anetSetError(err, "creating socket :%s", strerror(errno));
        return ANET_ERR;
    }

    if (setsocket(s,SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
    {
        anetSetError(err, "setsocketopt SO_REUSEADDR :%s", strerror(errno));
        return ANET_ERR;
    }

    return s;
}

