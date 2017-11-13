#ifndef CHAT_MESSAGE_QUEUE_H
#define CHAT_MESSAGE_QUEUE_H

#include "semaphore.h"
#include "interfaces.h"
#include <stdlib.h>

typedef struct MessageNode {
    Message m;
    struct MessageNode *next;
} MessageNode;

typedef struct {
    MessageNode *head;
    sem_t mutex;
    unsigned int size;
} MessageQueue;

/**
 * Initializes a new concurrent message queue
 * @return a pointer to initialized message queue
 */
MessageQueue *initMessageQueue();

/**
 * Add a message to a message queue
 * Blocks while others are operating on queue
 * @param queue - Queue where the message is sent
 * @param m - Message to be sent to the queue
 */
void pushMessageQueue(MessageQueue *queue, Message m);

/**
 * Retrieves all messages from the queue
 * Blocks while others are operating on queue
 * @param queue - Queue where the messages are retrieved from
 * @param array - Array where the messages are going to be
 * @return Size of the array of messages retrieved
 */
int pullMessageQueue(MessageQueue *queue, Message **array);

#endif //CHAT_MESSAGE_QUEUE_H
