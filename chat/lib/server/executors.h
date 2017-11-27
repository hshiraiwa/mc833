#ifndef CHAT_EXECUTORS_H
#define CHAT_EXECUTORS_H

#include <pthread.h>
#include <stdio.h>
#include <zconf.h>
#include "../commons.h"
#include "message_queue.h"
#include <strings.h>
#include "client_list.h"

/**
 * Init a thread that will receive messages from a open socket
 * and add them to a MessageQueue
 * @param sockfd - Socket file descriptor
 * @param queue - Queue where the messages are sent
 * @param id - thread id
 * @return created thread pointer
 */
pthread_t *initMessageReceiver(int sockfd, MessageQueue *queue, int id);

/**
 * Init a thread that will consume messages from a queue
 * and send responses to a open socket
 * @param sockfd - Socket file descriptor
 * @param queue - Queue where the messages are retrieved
 * @param id - thread id
 * @param handler - Function that handles the responses, must return the response array size
 * @return
 */
pthread_t *
initMessageConsumer(int sockfd, MessageQueue *queue, int id, int (*handler)(Message, Message **, ClientList *),
                    ClientList *clientList);

#endif //CHAT_EXECUTORS_H
