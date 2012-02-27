#ifndef __REDIS_H
#define __REDIS_H

#include "version.h" /*Version macro */
#include "adlist.h"
#include "anet.h"

#include <sys/types.h>

typedef struct redisObject {
    unsigned type:4;
    unsigned notused:2;
    unsigned encoding:4;
    unsigned lru:22;
    int refcount;
    void *ptr;
} robj;


/*------------------------------------------------------------
 *Global server state
 *------------------------------------------------------------*/

typedef struct redisClient {
    
} redisClient;


struct redisServer {
    int daemonize;                  /* True if running as a daemon */
    
    char *logfile;
    int syslog_enabled;             /* Is syslog enabled? */
    char *syslog_ident;             /* Syslog ident */
    int syslog_facility;            /* Syslog facility */
    redisClient *current_client;    /* Current client, only used on crash report*/

    /* Configuration */
    int verbosity;
    char* pidfile;

    /* Networking */
    int port;                       /* TCP listening port */
    char *bindaddr;                 /* Bind address or NULL */
    char *unixsocket;               /* UNIX socket path */
    mode_t unixsocketperm;          /* UNIX socket permission */
    int ipfd;
    int sofd;
    list *clients;                  /* List of active clients */
    list *clients_to_close;         /* Clients to close asynchronously */
    list *slaves;
    list *monitors;                 
    list *unblocked_clients;        /* list of clients to unblock before next loop */
    char neterr[ANET_ERR_LEN];

    /* AOF persistence */
    int aof_state;                  /* REDIS_AOF_(ON|OFF|WAIT_REWRITE) */
};

typedef void redisCommandProc(redisClient *c);
typedef int *redisGetKeysProc(struct redisCommand *cmd, robj **argv, int argc, int *numkeys, int flags);
struct redisCommand {
    char *name;
    redisCommandProc *proc;
    int arbity;
    char *sflags;
    int flags;
    redisGetKeysProc *getkeys_proc;
    int firstkey;
    int lastkey;
    int keystep;
    long long microseconds;
    long long calls;
};

void redisLog(int level,const char *fmt, ...);
void redisLogRaw(int level,const char *msg);

void createPidFile(void);
void daemonize(void);
void getCommand(redisClient *c);

/* Log levels */
#define REDIS_DEBUG 0
#define REDIS_VERBOSE 1
#define REDIS_NOTICE 2
#define REDIS_WARNING 3
#define REDIS_LOG_RAW (1<<10)

#define REDIS_MAX_LOGMSG_LEN 1024

/* Static server configuration */
#define REDIS_SERVERPORT 6379

/* AOF states */
#define REDIS_AOF_OFF 0
#define REDIS_AOF_ON 1
#define REDIS_AOF_WAIT_REWRITE 2

#endif

