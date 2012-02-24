#ifndef __REDIS_H
#define __REDIS_H

#include "version.h" /*Version macro */
#include "adlist.h"

/*------------------------------------------------------------
 *Global server state
 *------------------------------------------------------------*/

typedef struct redisClient {
    
} redisClient;


struct redisServer {
    int daemonize;                  /* True if running as a daemon */
    int syslog_enabled;             /* Is syslog enabled? */
    char *syslog_ident;             /* Syslog ident */
    int syslog_facility;            /* Syslog facility */
    redisClient *current_client;    /* Current client, only used on crash report*/
    list *clients;                  /* List of active clients */
    list *clients_to_close;         /* Clients to close asynchronously */
    list *slaves;
    list *monitors;                 
    list *unblocked_clients;        /* list of clients to unblock before next loop */
};

#endif

