#ifndef CLIENT_LIST_H
#define CLIENT_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "semaphore.h"
#include "../interfaces.h"


typedef struct ClientNode {
    Client c;
    struct ClientNode *next;
} ClientNode;

typedef struct {
    unsigned int size;
    ClientNode *head;
    sem_t mutex;
} ClientList;


ClientList *initClientList();

int searchNickname(char *ip, unsigned short port, ClientList *clientList, char **nickname);

void pushToList(Client c, ClientList *clientList);

int getClients(Client **clients, ClientList *clientList);

void removeClient(Client c, ClientList *clientList);

#endif //CLIENT_LIST_H