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

struct redisServer server;

/*
 * to do
 */
void initServerConfig() {
    server.syslog_enabled = 0;
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

}

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
        //log something        
    }
    if (server.daemonize) daemonize();
    initServer();

}
