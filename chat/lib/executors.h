#ifndef CHAT_EXECUTORS_H
#define CHAT_EXECUTORS_H

#include <pthread.h>
#include <stdio.h>
#include <zconf.h>
#include "commons.h"
#include "message_queue.h"

pthread_t *initMessageReceiver(int sockfd, MessageQueue *queue, int id);

pthread_t *initMessageSender(int sockfd, MessageQueue *queue, int id);

#endif //CHAT_EXECUTORS_H
