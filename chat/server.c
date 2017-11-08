#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib/commons.h"
#include "lib/message_queue.h"

struct ExecutorParameters {
    MessageQueue *queue;
    int sockfd;
    int id;
};

void *recvExecutor(void *parameters) {
    MessageQueue *queue = ((struct ExecutorParameters *) parameters)->queue;
    int sockfd = ((struct ExecutorParameters *) parameters)->sockfd;
    int id = ((struct ExecutorParameters *) parameters)->id;
    for (;;) {
        Message m = recvMessage(sockfd);
        printf("%d - %s %d - %s\n", id, m.ip, m.port, m.body.message);

        pushMessageQueue(queue, m);
    }
}

void *sendExecutor(void *parameters) {
    MessageQueue *queue = ((struct ExecutorParameters *) parameters)->queue;
    int sockfd = ((struct ExecutorParameters *) parameters)->sockfd;
    int id = ((struct ExecutorParameters *) parameters)->id;

    for (;;) {
        Message *messages;
        int size = pullMessageQueue(queue, &messages);
        if (size != 0) {
            for (int i = 0; i < size; i++) {
                sendMessage(sockfd, messages[i]);
            }
            free(messages);
        } else {
            sleep(1);
        }
    }
}

int main() {
    printf("starting server:\n");
    int sockfd = createSocket();
    bindSocket(sockfd, 3000);
    printf("socket bound to port 3000\n");
    MessageQueue *queue = initMessageQueue();


    pthread_t recvExec[2];
    pthread_t sendExec[2];
    for (int i = 0; i < 2; i++) {
        struct ExecutorParameters parameters;
        parameters.sockfd = sockfd;
        parameters.id = i;
        parameters.queue = queue;
        if (pthread_create(&recvExec[i], NULL, recvExecutor, (void *) &parameters)) {
            perror("ERROR: failed to create thread");
            exit(1);
        }

        if (pthread_create(&sendExec[i], NULL, sendExecutor, (void *) &parameters)) {
            perror("ERROR: failed to create thread");
            exit(1);
        }
    }

    if (pthread_join(recvExec[0], NULL)) {
        perror("ERROR: could not join thread");
        exit(1);
    }

    return 0;
}