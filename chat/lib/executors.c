#include <strings.h>
#include "executors.h"

struct RecvExecutorParameters {
    MessageQueue *queue;
    int sockfd;
    int id;
    int shutdownFlag;
};

struct ConsumerExecutorParameters {
    MessageQueue *queue;

    int (*handler)(Message, Message **);

    int sockfd;
    int id;
    int shutdownFlag;
};

void *recvExecutor(void *param) {
    struct RecvExecutorParameters *parameters = (struct RecvExecutorParameters *) param;
    while (!parameters->shutdownFlag) {
        Message m = recvMessage(parameters->sockfd);
        printf("%d - %s %d - %s\n", parameters->id, m.ip, m.port, m.body.message);

        pushMessageQueue(parameters->queue, m);
    }
}

void *consumerExecutor(void *param) {
    struct ConsumerExecutorParameters *parameters = (struct ConsumerExecutorParameters *) param;

    while (!parameters->shutdownFlag) {
        Message *messages;
        int size = pullMessageQueue(parameters->queue, &messages);
        if (size != 0) {
            for (int i = 0; i < size; i++) {
                Message *responses;
                int responseCount = parameters->handler(messages[i], &responses);
                for (int j = 0; j < responseCount; j++) {
                    sendMessage(parameters->sockfd, responses[j]);
                }
                if (responseCount > 0)
                    free(responses);
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
    struct RecvExecutorParameters *parameters = malloc(sizeof(struct RecvExecutorParameters));
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

pthread_t *initMessageConsumer(int sockfd, MessageQueue *queue, int id, int (*handler)(Message, Message **)) {
    pthread_t *pthread = malloc(sizeof(pthread_t));
    bzero(pthread, sizeof(pthread_t));
    struct ConsumerExecutorParameters *parameters = malloc(sizeof(struct ConsumerExecutorParameters));
    parameters->sockfd = sockfd;
    parameters->queue = queue;
    parameters->id = id;
    parameters->handler = handler;
    parameters->shutdownFlag = 0;

    if (pthread_create(pthread, NULL, consumerExecutor, (void *) parameters)) {
        perror("ERROR: failed to create thread");
        exit(1);
    }

    return pthread;
}
