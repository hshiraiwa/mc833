#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib/commons.h"
#include "lib/message_queue.h"
#include "lib/executors.h"
#include "lib/interfaces.h"

#define THREAD_COUNT 5

int handler(Message m, Message **messages, ClientList *clientList) {
    int nicknameLen;
    char *nickname;
    switch (m.body.type) {
        case GREETING:
            printf("%s %d: %s\n", m.ip, m.port, m.body.data.greeting);
            pushToList(extractClient(m), clientList);
            *messages = malloc(sizeof(Message));
            **messages = message(m.ip, m.port, createAckMessage(GREETING));
            return 1;
        case MESSAGE:
            printf("%s %d: %s\n", m.ip, m.port, m.body.data.message);
            nicknameLen = searchNickname(m.ip, m.port, clientList, &nickname);
            if (nicknameLen > 0) {
                *messages = malloc(sizeof(Message));
                **messages = m;
                return 1;
            }
            return 0;
    }
    return 0;
}

int main() {
    printf("starting server:\n");
    int sockfd = createSocket();
    bindSocket(sockfd, 3000);
    printf("socket bound to port 3000\n");

    ClientList *clientList = initClientList();
    MessageQueue *queue = initMessageQueue();


    pthread_t *recvExec[THREAD_COUNT];
    pthread_t *sendExec[THREAD_COUNT];
    for (int i = 0; i < THREAD_COUNT; i++) {
        recvExec[i] = initMessageReceiver(sockfd, queue, i);
        sendExec[i] = initMessageConsumer(sockfd, queue, i, handler, clientList);

    }
    for (int i = 0; i < THREAD_COUNT; ++i) {
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