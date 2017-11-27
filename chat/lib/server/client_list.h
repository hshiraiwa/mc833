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

/**
 * Initializes a new concurrent client list
 * @return Initialized list pointer
 */
ClientList *initClientList();

/**
 * Search for a nickname from a specific user in client list
 * @param ip - Client ip
 * @param port - Client port
 * @param clientList - Client list where is going to be queryed
 * @param nickname - Found nickname, null (0) if none found
 * @return nickname length
 */
int searchNickname(char *ip, unsigned short port, ClientList *clientList, char **nickname);

/**
 * Search for the address from a specific user in client list
 * @param ip - pointer to where the user ip will be stored
 * @param clientList - Client list where is going to be queryed
 * @param nickname - nickname to search for
 * @return client's port
 */
unsigned short searchAddress(char **ip, ClientList *clientList, char *nickname);

/**
 * Add a new client to the clientList
 * @param c - Client to be added
 * @param clientList - Client list where the client will be added
 */
void pushToList(Client c, ClientList *clientList);

/**
 * Returns all clients in a array form
 * @param clients - Client array return, populated with all users (0 if none found)
 * @param clientList - Client list where the operation will happen
 * @return clients size, 0 if none found
 */
int getClients(Client **clients, ClientList *clientList);

/**
 * Removes a client from the client list
 * @param c - Client to be removed
 * @param clientList - Client list where the operation will happen
 */
void removeClient(Client c, ClientList *clientList);

#endif //CLIENT_LIST_H