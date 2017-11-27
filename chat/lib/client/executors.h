#ifndef EXECUTORS_H
#define EXECUTORS_H

#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <pthread.h>
#include "../interfaces.h"
#include "../commons.h"

/**
 * Initializes a thread responsible to handle received messages
 * @param sockfd - Socket where the messages will arrive
 * @param handler - Handler function that will consume the messages as they arrive
 * @return created pthread pointer
 */
pthread_t *initMessageReceiver(int sockfd, void (*handler)(Message));

#endif //EXECUTORS_H
