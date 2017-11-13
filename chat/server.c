#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib/commons.h"
#include "lib/message_queue.h"
#include "lib/executors.h"

int handler(Message m, Message **messages) {
    *messages = malloc(sizeof(Message)*2);
    (*messages)[0] = message(m.ip, m.port, m.body);
    (*messages)[1] = message(m.ip, m.port, m.body);
    return 2;
}

int main() {
    printf("starting server:\n");
    int sockfd = createSocket();
    bindSocket(sockfd, 3000);
    printf("socket bound to port 3000\n");
    MessageQueue *queue = initMessageQueue();


    pthread_t *recvExec[2];
    pthread_t *sendExec[2];
    for (int i = 0; i < 2; i++) {
        recvExec[i] = initMessageReceiver(sockfd, queue, i);
        sendExec[i] = initMessageConsumer(sockfd, queue, i, handler);

    }
    for (int i = 0; i < 2; ++i) {
        if (pthread_join(*(recvExec[i]), NULL)) {
            perror("ERROR: could not join thread");
            exit(1);
        }
        if (pthread_join(*(sendExec[i]), NULL)) {
            perror("ERROR: could not join thread");
            exit(1);
        }
    }


    return 0;
}