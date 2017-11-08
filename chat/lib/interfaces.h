#ifndef CHAT_INTERFACES_H_H
#define CHAT_INTERFACES_H_H

#include <netinet/in.h>

typedef struct {
    //TEMPORARY FIELD, MOSTLY FOR DEBUG REASONS
    uint8_t message[20];
} MessageBody;

typedef struct {
    unsigned short port;
    char ip[INET_ADDRSTRLEN];

    MessageBody body;
} Message;


#endif //CHAT_INTERFACES_H_H
