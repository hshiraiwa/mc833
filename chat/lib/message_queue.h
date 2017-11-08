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

MessageQueue *initMessageQueue();

void pushMessageQueue(MessageQueue *queue, Message m);

int pullMessageQueue(MessageQueue *queue, Message **array);

#endif //CHAT_MESSAGE_QUEUE_H
