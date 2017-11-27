#include "message_queue.h"

MessageQueue *initMessageQueue() {
    MessageQueue *queue = malloc(sizeof(MessageQueue));
    queue->head = 0;
    queue->tail = 0;
    queue->size = 0;
    sem_init(&queue->mutex, 0, 1);

    return queue;
}

MessageNode *_pushQueue(MessageNode **node, Message m) {
    if (*node == 0) {
        MessageNode *newNode = malloc(sizeof(MessageNode));
        newNode->m = m;
        newNode->next = 0;
        *node = newNode;

        return newNode;
    }

    return _pushQueue(&(*node)->next, m);
}

void pushMessageQueue(MessageQueue *queue, Message m) {
    sem_wait(&queue->mutex);
    queue->size = queue->size + 1;
    if (queue->tail == 0)
        queue->tail = _pushQueue(&queue->head, m);
    else
        queue->tail = _pushQueue(&queue->tail, m);
    sem_post(&queue->mutex);
}

void queueToArray(MessageNode *node, Message *array, int i) {
    if (node == 0)
        return;
    array[i] = node->m;

    MessageNode *next = node->next;
    free(node);
    queueToArray(next, array, i + 1);
}

int pullMessageQueue(MessageQueue *queue, Message **array) {
    sem_wait(&queue->mutex);
    MessageNode *nodes = queue->head;
    int size = queue->size;
    queue->size = 0;
    queue->head = 0;
    queue->tail = 0;
    sem_post(&queue->mutex);

    if (size != 0) {
        *array = malloc(sizeof(Message) * (size));
        queueToArray(nodes, *array, 0);
    }

    return size;
}
