#include <strings.h>
#include "executors.h"

struct ExecutorParameters {
    MessageQueue *queue;
    int sockfd;
    int id;
    int shutdownFlag;
};

void *recvExecutor(void *param) {
    struct ExecutorParameters *parameters = (struct ExecutorParameters *) param;
    while (!parameters->shutdownFlag) {
        Message m = recvMessage(parameters->sockfd);
        printf("%d - %s %d - %s\n", parameters->id, m.ip, m.port, m.body.message);

        pushMessageQueue(parameters->queue, m);
    }
}

void *sendExecutor(void *param) {
    struct ExecutorParameters *parameters = (struct ExecutorParameters *) param;

    while (!parameters->shutdownFlag) {
        Message *messages;
        int size = pullMessageQueue(parameters->queue, &messages);
        if (size != 0) {
            for (int i = 0; i < size; i++) {
                sendMessage(parameters->sockfd, messages[i]);
            }
            free(messages);
        } else {
            sleep(1);
        }
    }
}

pthread_t *initMessageReceiver(int sockfd, MessageQueue *queue, int id) {
    pthread_t *pthread = malloc(sizeof(pthread_t));
    bzero(pthread, sizeof(pthread_t));
    struct ExecutorParameters *parameters = malloc(sizeof(struct ExecutorParameters));
    parameters->sockfd = sockfd;
    parameters->queue = queue;
    parameters->id = id;
    parameters->shutdownFlag = 0;

    if (pthread_create(pthread, NULL, recvExecutor, (void *) parameters)) {
        perror("ERROR: failed to create receiver thread");
        exit(1);
    }

    return pthread;
}

pthread_t *initMessageSender(int sockfd, MessageQueue *queue, int id) {
    pthread_t *pthread = malloc(sizeof(pthread_t));
    bzero(pthread, sizeof(pthread_t));
    struct ExecutorParameters *parameters = malloc(sizeof(struct ExecutorParameters));
    parameters->sockfd = sockfd;
    parameters->queue = queue;
    parameters->id = id;
    parameters->shutdownFlag = 0;

    if (pthread_create(pthread, NULL, sendExecutor, (void *) parameters)) {
        perror("ERROR: failed to create thread");
        exit(1);
    }

    return pthread;
}
