/*
 *the start of the game
 *liuhaobupt 2012-2-23
 */

#include "redis.h"

#include <stdlib.h>
#include <time.h>
#include <string.h>

/*
 * to do
 */
void initServerConfig() {

}

void version() {
    printf("Redis server version %s\n",REDIS_VERSION);
    exit(0);
}

void usage() {
    exit(1);
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

    }

}
