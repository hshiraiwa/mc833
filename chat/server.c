#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib/commons.h"
#include "lib/message_queue.h"
#include "lib/executors.h"

struct ExecutorParameters {
    MessageQueue *queue;
    int sockfd;
    int id;
};

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
        sendExec[i] = initMessageSender(sockfd, queue, i);

    }

    if (pthread_join(*(recvExec[0]), NULL)) {
        perror("ERROR: could not join thread");
        exit(1);
    }

    return 0;
}