/*
 *the start of the game
 *liuhaobupt 2012-2-23
 */

#include "redis.h"

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <syslog.h>
#include <stdarg.h>

struct redisServer server;

/*
 * 
 */
void initServerConfig() {
    server.syslog_enabled = 0;
    server.daemonize = 0;
    server.logfile = NULL;
    server.syslog_ident = zstrdup("redis");
    server.syslog_facility = LOG_LOCAL0;
    server.verbosity = REDIS_NOTICE;
    server.port = REDIS_SERVERPORT;
    server.bindaddr = NULL;
    server.unixsocket = NULL;
    server.unixsocketperm = 0;
    server.ipfd = -1;
    server.sofd = -1;
    server.pidfile = zstrdup("/var/run/redis.pid");
}

void version() {
    printf("Redis server version %s\n",REDIS_VERSION);
    exit(0);
}

void usage() {
    exit(1);
}

void daemonize(void) {
    int fd;

    if( fork() != 0) exit(0);   /*parent exits */
    setsid();   /* create a new session */

    if((fd = open("/dev/null", O_RDWR, 0)) != -1) {
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        if(fd > STDERR_FILENO) close(fd);
    }
}

void createPidFile(void) {
    FILE *fp = fopen(server.pidfile,"w");
    if (fp) {
        fprintf(fp,"%d\n",(int)getpid());
        fclose(fp);
    }
}


void setupSignalHandlers(void) {
    struct sigaction act;

    //to do
}


void initServer() {
    int j;

    signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    setupSignalHandlers();
    
    if (server.syslog_enabled) {
        openlog(server.syslog_ident, LOG_PID | LOG_NDELAY | LOG_NOWAIT,
                server.syslog_facility);
    }

    server.current_client = NULL;
    server.clients = listCreate();
    server.clients_to_close = listCreate();
    server.slaves = listCreate();
    server.monitors = listCreate();
    server.unblocked_clients = listCreate();

    if (server.port != 0)
    {
        server.ipfd = anetTcpServer(server.neterr, server.port,server.bindaddr);
        if(server.ipfd == ANET_ERR)
        {
            redisLog(REDIS_WARNING, "Opening port %d:%s", server.port,server.neterr);
            exit(1);
        }
    }
}

#ifdef __linux__

int linuxOvercommitMemoryValue(void) {
    FILE *fp = fopen("/proc/sys/vm/overcommit_memory","r");
    char buf[64];

    if (!fp) return -1;
    if (fgets(buf,64,fp) == NULL) {
        fclose(fp);
        return -1;
    }
    fclose(fp);

    return atoi(buf);
}

void linuxOvercommitMemoryWarning() {
    if(linuxOvercommitMemoryValue() == 0) {
        redisLog(REDIS_WARNING, "WARNING overcommit_momory is set to 0! Background save may fail under low memory condition. To fix this issue add 'vm.overcommmit_memory = 1' to /etc/sysctl.conf and then reboot or run the command 'sysctl vm.overcommit_memory=1' for this to take effect");
    }
}

#endif /* __linux__ */

int main(int argc,char **argv)
{
    //where to use?
    long long start;

    struct timeval tv;

    //initialize libraries,and the server configuration
    zmalloc_enable_thread_safeness();
    srand(time(NULL)^getpid());
    gettimeofday(&tv,NULL);
    initServerConfig();

    if (argc >= 2) {
        int j = 1;
        char *configfile = NULL;

        if (strcmp(argv[1], "-v") == 0 ||
            strcmp(argv[1], "--version") == 0) version();
        if (strcmp(argv[1], "--help") == 0 ||
            strcmp(argv[1], "-h") == 0) usage();
        if (argv[j][0] != '-' || argv[j][1] != '-')
            configfile = argv[j++];

    } else {
        redisLog(REDIS_WARNING,"Warning: no config file specified, using the default config. In order to specify a config file use 'redis-server /path/to/redis.conf'");
    }
    if (server.daemonize) daemonize();
    initServer();
    if (server.daemonize) createPidFile();
    redisLog(REDIS_WARNING,"Server started, Redis version %d",REDIS_VERSION);
#ifdef __linux__
    linuxOvercommitMemoryWarning();
#endif

}

/*========================================Utility functions ==============*/

void redisLog(int level,const char *fmt, ...) {
    va_list ap;
    char msg[REDIS_MAX_LOGMSG_LEN];

    if ((level&0xff) < server.verbosity) return;

    va_start(ap,fmt);
    vsnprintf(msg,sizeof(msg),fmt,ap);
    va_end(ap);

    redisLogRaw(level,msg);
}

void redisLogRaw(int level,const char *msg) {
    const int syslogLevelMap[] = { LOG_DEBUG, LOG_INFO, LOG_NOTICE, LOG_WARNING };
    const char *c=".-*#";
    time_t now = time(NULL);
    FILE *fp;
    char buf[64];
    int rawmode = (level & REDIS_LOG_RAW);

    level &= 0xff;
    if (level < server.verbosity) return;

    fp = (server.logfile == NULL) ? stdout : fopen(server.logfile, "a");
    if (!fp) return;
    if (rawmode) {
        fprintf(fp,"%s",msg);
    } else {
        strftime(buf,sizeof(buf),"%d %b %H:%M:%S",localtime(&now));
        fprintf(fp,"[%d] %s %c %s\n",(int)getpid(),buf,c[level],msg);
    }
    fflush(fp);

    if (server.logfile) fclose(fp);

    if (server.syslog_enabled) syslog(syslogLevelMap[level],"%s",msg);
}


