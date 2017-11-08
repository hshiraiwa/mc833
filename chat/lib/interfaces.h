#ifndef CHAT_INTERFACES_H_H
#define CHAT_INTERFACES_H_H

#include <netinet/in.h>

typedef struct {
    //TEMPORARY FIELD, MOSTLY FOR DEBUG REASONS
    char message[20];
} MessageBody;

typedef struct {
    uint16_t port;
    uint8_t ip[INET_ADDRSTRLEN];

    MessageBody body;
} Message;


#endif //CHAT_INTERFACES_H_H
