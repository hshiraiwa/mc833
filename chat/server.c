#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib/commons.h"
#include "lib/message_queue.h"
#include "lib/executors.h"

#define THREAD_COUNT 5

int handleGreeting(Message m, Message **messages, ClientList *clientList) {
    printf("%s %d: %s\n", m.ip, m.port, m.body.data.greeting);
    pushToList(extractClient(m), clientList);
    *messages = malloc(sizeof(Message));
    **messages = message(m.ip, m.port, createAckMessage(GREETING));
    return 1;
}


int handleMessage(Message m, Message **messages, ClientList *clientList) {
    printf("%s %d: %s\n", m.ip, m.port, m.body.data.message);

    char *nickname;
    int nicknameLen = searchNickname(m.ip, m.port, clientList, &nickname);
    if (nicknameLen > 0) {
        Client *clients;
        int size = getClients(&clients, clientList);
        *messages = malloc(sizeof(Message) * size);
        for (int i = 0; i < size; i++) {
            (*messages)[i] = message(clients[i].ip, clients[i].port, createMessageBody((char *) m.body.data.message));
        }
        free(clients);
        return size;
    }
    return 0;
}

int handler(Message m, Message **messages, ClientList *clientList) {

    switch (m.body.type) {
        case GREETING:
            return handleGreeting(m, messages, clientList);
        case MESSAGE:
            return handleMessage(m, messages, clientList);
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