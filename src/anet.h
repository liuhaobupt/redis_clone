/* anet.h -- Basic TCP socket stuff made a bit less boring
 *
 */

#ifndef ANET_H
#define ANET_H

#define ANET_OK 0
#define ANET_ERR -1
#define ANET_ERR_LEN 256

int anetTcpServer(char *err, int port, char *bindaddr);

#endif
