#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib/commons.h"
#include "lib/server/message_queue.h"
#include "lib/server/executors.h"

#define THREAD_COUNT 1

int handleGreeting(Message m, Message **messages, ClientList *clientList) {
    Client client = extractClient(m);

    Client *clients;
    int clientSize = getClients(&clients, clientList);
    int responseSize = ((clientSize * 2) + 1);

    *messages = malloc(sizeof(Message) * responseSize);
    for (int i = 0; i < clientSize; i++) {
        (*messages)[i * 2] = message(clients[i].ip, clients[i].port, createNicknameListMessage(client));
        (*messages)[(i * 2) + 1] = message(client.ip, client.port, createNicknameListMessage(clients[i]));
    }

    (*messages)[responseSize - 1] = message(client.ip, client.port, createAckMessage(GREETING));

    pushToList(client, clientList);
    return responseSize;
}


int handleMessage(Message m, Message **messages, ClientList *clientList) {
    char *nickname;
    int nicknameLen = searchNickname(m.ip, m.port, clientList, &nickname);
    if (nicknameLen > 0) {
        Client *clients;
        int size = getClients(&clients, clientList);
        *messages = malloc(sizeof(Message) * size);
        for (int i = 0; i < size; i++) {
            if (strcmp(nickname, clients[i].nickname) != 0) {
                (*messages)[i] = message(clients[i].ip,
                                         clients[i].port,
                                         createTextBody((char *) m.body.data.text.body,
                                                        nickname));
            } else {
                (*messages)[i] = message(clients[i].ip,
                                         clients[i].port,
                                         createAckMessage(TEXT));
            }
        }
        free(nickname);
        free(clients);
        return size;
    }
    return 0;
}

int handler(Message m, Message **messages, ClientList *clientList) {

    switch (m.body.type) {
        case GREETING:
            return handleGreeting(m, messages, clientList);
        case TEXT:
            return handleMessage(m, messages, clientList);
        default:
            return 0;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("usage: server <port>\n");
        exit(1);
    }

    int sockfd = createSocket();
    bindSocket(sockfd, atoi(argv[1]));
    printf("socket bound to port %s\n", argv[1]);

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