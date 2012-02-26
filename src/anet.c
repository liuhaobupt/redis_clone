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

static int anetListen(char *err, int s, struct sockaddr *sa, socklen_t len)
{
    if(bind(s,sa,len) == -1)
    {
        anetSetError(err, "bind: %s", strerror(errno));
        close(s);
        return ANET_ERR;
    }
    if (listen(s, 511) == -1)
    {
        anetSetError(err, "listen: %s", strerror(errno));
        close(s);
        return ANET_ERR;
    }
    return ANET_OK;
}

int anetTcpServer(char *err, int port, char *bindaddr)
{
    int s;
    struct sockaddr_in sa;

    if ((s = anetCreateSocket(err, AF_INET)) == ANET_ERR)
        return ANET_ERR;
    
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bindaddr && inet_aton(bindaddr, &sa.sin_addr) == 0)
    {
        anetSetError(err, "invalid bind address");
        close(s);
        return ANET_ERR;
    }
    if (anetListen(err,s,(struct sockaddr *)&sa,sizeof(sa)) == ANET_ERR )
        return ANET_ERR;
    return s;
}
