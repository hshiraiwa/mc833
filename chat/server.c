#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib/commons.h"
#include "lib/server/message_queue.h"
#include "lib/server/executors.h"

#define THREAD_COUNT 1

/**
 * Validates if nickname is already in use
 * @param nickname - Nickname to lookup
 * @param clientList - ClientList to lookup
 * @return 1 if is available 0 if in use
 */
int validateNickname(char *nickname, ClientList *clientList) {
    char *ip;
    unsigned short port = searchAddress(&ip, clientList, nickname);

    if (port == 0) return 1;
    return 0;
}

int handleGreeting(Message m, Message **messages, ClientList *clientList) {
    Client client = extractClient(m);

    if (validateNickname((char *) m.body.data.greeting.nickname, clientList)) {
        Client *clients;
        int clientSize = getClients(&clients, clientList);
        int responseSize = ((clientSize * 2) + 1);

        *messages = malloc(sizeof(Message) * responseSize);
        (*messages)[0] = message(client.ip, client.port, createAckMessage(GREETING_ACK));
        for (int i = 0; i < clientSize; i++) {
            (*messages)[(i * 2) + 1] = message(clients[i].ip, clients[i].port, createNicknameListMessage(client));
            (*messages)[(i * 2) + 2] = message(client.ip, client.port, createNicknameListMessage(clients[i]));
        }


        pushToList(client, clientList);
        return responseSize;
    } else {
        *messages = malloc(sizeof(Message));
        **messages = message(client.ip, client.port, createAckMessage(NICKNAME_IN_USE_ACK));
        return 1;
    }
}


int handleMessage(Message m, Message **messages, ClientList *clientList) {
    char *nickname;
    int nicknameLen = searchNickname(m.ip,
                                     m.port,
                                     clientList,
                                     &nickname);
    if (nicknameLen > 0) {
        Client *clients;
        int size = getClients(&clients, clientList);
        *messages = malloc(sizeof(Message) * size);
        for (int i = 0; i < size; i++) {
            (*messages)[i] = message(clients[i].ip,
                                     clients[i].port,
                                     createTextBody((char *) m.body.data.text.body,
                                                    nickname));
        }
        free(nickname);
        free(clients);
        return size;
    }
    return 0;
}

int handlePrivateMessage(Message m, Message **messages, ClientList *clientList) {
    char *nickname;
    int nicknameLen = searchNickname(m.ip,
                                     m.port,
                                     clientList,
                                     &nickname);
    if (nicknameLen > 0) {
        char *ip;
        unsigned short port = searchAddress(&ip,
                                            clientList,
                                            (char *) m.body.data.privateText.recipient);
        if (ip == 0)
            return 0;

        *messages = malloc(sizeof(Message));
        **messages = message(ip, port, createPrivateTextBody((char *) m.body.data.privateText.body,
                                                             (char *) m.body.data.privateText.recipient,
                                                             nickname));
        free(nickname);
        free(ip);
        return 1;
    }
    return 0;
}

int handleDisconnect(Message m, Message **messages, ClientList *clientList) {
    Client c = extractClient(m);
    removeClient(c, clientList);
    Client *clients;
    int size = getClients(&clients, clientList);

    *messages = malloc(sizeof(Message) * size);
    for (int i = 0; i < size; i++) {
        (*messages)[i] = message(clients[i].ip,
                                 clients[i].port,
                                 createDisconnectBody((char *) m.body.data.disconnect.nickname));
    }

    return size;
}

int handleDataTransfer(Message m, Message **messages, ClientList *clientList) {
    char *nickname;
    int nicknameLen = searchNickname(m.ip,
                                     m.port,
                                     clientList,
                                     &nickname);
    if (nicknameLen > 0) {
        char *ip;
        unsigned short port = searchAddress(&ip,
                                            clientList,
                                            (char *) m.body.data.dataTransfer.nickname);
        if (ip == 0)
            return 0;

        *messages = malloc(sizeof(Message));
        **messages = message(ip, port, createDataTransferBody(nickname,
                                                              m.body.data.dataTransfer.ip,
                                                              m.body.data.dataTransfer.port));
        free(nickname);
        free(ip);
        return 1;
    }
    return 0;
}

int handler(Message m, Message **messages, ClientList *clientList) {

    switch (m.body.type) {
        case GREETING:
            return handleGreeting(m, messages, clientList);
        case TEXT:
            return handleMessage(m, messages, clientList);
        case PRIVATE_TEXT:
            return handlePrivateMessage(m, messages, clientList);
        case DISCONNECT:
            return handleDisconnect(m, messages, clientList);
        case DATA_TRANSFER:
            return handleDataTransfer(m, messages, clientList);
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