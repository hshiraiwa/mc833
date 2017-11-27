#ifndef UTILS_H
#define UTILS_H

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "../interfaces.h"
#include "../commons.h"

/**
 * Given a hostname returns its ip address
 * @param hostname - Hostname to be resolved
 * @return Ip address pointer
 */
char * resolveHostname(char *hostname);

/**
 * Send a greeting message to the server
 * @param sockfd - Socket file descriptor
 * @param ip - Server IP
 * @param port - Server Port
 * @param nickname - Desired nickname
 */
void sendGreetings(int sockfd, char *ip, int port, char *nickname);

#endif //UTILS_H
