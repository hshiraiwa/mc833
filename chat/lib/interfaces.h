#ifndef CHAT_INTERFACES_H_H
#define CHAT_INTERFACES_H_H

#include <netinet/in.h>

#define NICKNAME_LEN 300
#define MESSAGE_LEN 300

typedef enum {
    MESSAGE = 0,
    GREETING = 1,
    ACK = 2
} DataType;

typedef union {
    uint8_t message[MESSAGE_LEN];
    uint8_t greeting[NICKNAME_LEN];
    uint8_t ack;
} Data;

typedef struct {
    DataType type;
    Data data;
} MessageBody;

typedef struct {
    unsigned short port;
    char ip[INET_ADDRSTRLEN];

    MessageBody body;
} Message;

typedef struct {
    unsigned short port;
    char ip[INET_ADDRSTRLEN];
    char nickname[NICKNAME_LEN];
} Client;

#endif //CHAT_INTERFACES_H_H
