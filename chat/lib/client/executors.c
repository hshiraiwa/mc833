#include "executors.h"

struct RecvExecutorParameters {
    int sockfd;
    int shutdownFlag;

    void (*handler)(Message);
};

void *recvExecutor(void *param) {
    struct RecvExecutorParameters *parameters = (struct RecvExecutorParameters *) param;
    while (!parameters->shutdownFlag) {
        Message m = recvMessage(parameters->sockfd);
        parameters->handler(m);
    }
}

pthread_t *initMessageReceiver(int sockfd, void (*handler)(Message)) {
    pthread_t *pthread = malloc(sizeof(pthread_t));
    bzero(pthread, sizeof(pthread_t));
    struct RecvExecutorParameters *parameters = malloc(sizeof(struct RecvExecutorParameters));
    parameters->sockfd = sockfd;
    parameters->shutdownFlag = 0;
    parameters->handler = handler;

    if (pthread_create(pthread, NULL, recvExecutor, (void *) parameters)) {
        perror("ERROR: failed to create receiver thread");
        exit(1);
    }

    return pthread;
}
