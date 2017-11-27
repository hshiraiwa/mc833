#ifndef UTILS_H
#define UTILS_H

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "../interfaces.h"
#include "../commons.h"

void resolveHostname(char *hostname, char **ip);

void sendGreetings(int sockfd, char *ip, int port, char *nickname);

#endif //UTILS_H
