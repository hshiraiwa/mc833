#ifndef EXECUTORS_H
#define EXECUTORS_H

#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <pthread.h>
#include "../interfaces.h"
#include "../commons.h"

pthread_t *initMessageReceiver(int sockfd, void (*handler)(Message));

#endif //EXECUTORS_H
