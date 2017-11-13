#ifndef CLIENT_LIST_H
#define CLIENT_LIST_H

#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "semaphore.h"
#define NICKNAME_LEN 300

typedef struct {
    unsigned short port;
    char ip[INET_ADDRSTRLEN];
    char nickname[NICKNAME_LEN];
} Client;

typedef struct ClientNode {
    Client c;
    struct ClientNode *next;
} ClientNode;

typedef struct {
    unsigned int size;
    ClientNode *head;
    sem_t mutex;
} ClientList;


int searchNickname(char* ip, unsigned short port, ClientList* clientList, char** nickname);
void pushToList(Client c, ClientList* clientList);
void removeClient(Client c, ClientList *clientList);

#endif //CLIENT_LIST_H