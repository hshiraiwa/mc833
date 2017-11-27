#include "client_list.h"

ClientList *initClientList() {
    ClientList *clientList = malloc(sizeof(ClientList));
    clientList->head = 0;
    clientList->size = 0;
    sem_init(&clientList->mutex, 0, 1);
    return clientList;
}


int _searchNickname(char *ip, unsigned short port, ClientNode *clientNode, char **nickname) {
    if (clientNode == 0)
        return 0;
    if (strcmp(clientNode->c.ip, ip) == 0 && clientNode->c.port == port) {
        size_t len = strlen(clientNode->c.nickname);
        *nickname = malloc(sizeof(char) * (len + 1));
        strcpy(*nickname, clientNode->c.nickname);
        return (int) len;
    }
    return _searchNickname(ip, port, clientNode->next, nickname);
}

int searchNickname(char *ip, unsigned short port, ClientList *clientList, char **nickname) {
    sem_wait(&clientList->mutex);
    int result = _searchNickname(ip, port, clientList->head, nickname);
    sem_post(&clientList->mutex);
    return result;
}

unsigned short _searchAddress(char **ip, ClientNode *clientNode, char *nickname) {
    if (clientNode == 0)
        return 0;
    if (strcmp(nickname, clientNode->c.nickname) == 0) {
        *ip = malloc(sizeof(char) * INET_ADDRSTRLEN);
        strcpy(*ip, clientNode->c.ip);
        return clientNode->c.port;
    }
    return _searchAddress(ip, clientNode->next, nickname);
}

unsigned short searchAddress(char **ip, ClientList *clientList, char *nickname) {
    sem_wait(&clientList->mutex);
    int result = _searchAddress(ip, clientList->head, nickname);
    sem_post(&clientList->mutex);
    return result;
}

void pushToList(Client c, ClientList *clientList) {
    sem_wait(&clientList->mutex);
    clientList->size = clientList->size + 1;
    ClientNode *next = clientList->head;
    ClientNode *clientNode = malloc(sizeof(ClientNode));
    clientNode->c = c;
    clientNode->next = next;
    clientList->head = clientNode;
    sem_post(&clientList->mutex);
}

int getClients(Client **clients, ClientList *clientList) {
    sem_wait(&clientList->mutex);
    int size = clientList->size;
    if (size == 0) {
        *clients = 0;
        sem_post(&clientList->mutex);
        return 0;
    }
    *clients = malloc(sizeof(Client) * size);
    ClientNode *aux = clientList->head;
    int i = 0;
    while (aux != 0) {
        (*clients)[i] = aux->c;
        aux = aux->next;
        i++;
    }
    sem_post(&clientList->mutex);
    return size;
}

int _removeClient(Client c, ClientNode **node) {
    if (node == 0)
        return 0;
    if (strcmp((*node)->c.ip, c.ip) == 0 && (*node)->c.port == c.port) {
        (*node) = (*node)->next;
        return 1;
    }
    return _removeClient(c, &((*node)->next));
}

void removeClient(Client c, ClientList *clientList) {
    sem_wait(&clientList->mutex);
    if (_removeClient(c, &clientList->head))
        clientList->size = clientList->size - 1;
    sem_post(&clientList->mutex);
}