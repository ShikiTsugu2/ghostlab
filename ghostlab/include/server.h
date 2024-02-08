#ifndef SERVER_H
#define SERVER_H
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include "list_game.h"
#include "send.h"
#include <poll.h>
#include <unistd.h>
#include <signal.h>
#include "protocol_check.h"
static int VERB = 0;
#endif // SERVER
