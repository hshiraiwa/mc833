#ifndef CHAT_INTERFACES_H_H
#define CHAT_INTERFACES_H_H

#include <netinet/in.h>

#define NICKNAME_LEN 300
#define MESSAGE_LEN 300

typedef enum {
    TEXT = 0,
    GREETING = 1,
    NICKNAME_LIST = 2,
    ACK = 3
} DataType;

typedef struct Text {
    uint8_t body[MESSAGE_LEN];
    uint8_t nickname[NICKNAME_LEN];
} Text;

typedef struct Greeting {
    uint8_t nickname[NICKNAME_LEN];
} Greeting;

typedef struct NicknameList {
    uint8_t nickname[NICKNAME_LEN];
} NicknameList;

typedef struct Ack {
    uint8_t code;
} Ack;

typedef union {
    Text text;
    Greeting greeting;
    NicknameList nicknameList;
    Ack ack;
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
