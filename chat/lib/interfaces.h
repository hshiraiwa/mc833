#ifndef CHAT_INTERFACES_H_H
#define CHAT_INTERFACES_H_H

#include <netinet/in.h>

#define NICKNAME_LEN 30
#define MESSAGE_LEN 300

typedef enum {
    GREETING_ACK = 1,
    NICKNAME_IN_USE_ACK = 2
} AckType;

typedef enum {
    TEXT = 0,
    PRIVATE_TEXT = 1,
    GREETING = 2,
    NICKNAME_LIST = 3,
    ACK = 4,
    DATA_TRANSFER = 5,
    DISCONNECT = 6
} DataType;


typedef struct Text {
    uint8_t body[MESSAGE_LEN];
    uint8_t nickname[NICKNAME_LEN];
} Text;

typedef struct PrivateText {
    uint8_t body[MESSAGE_LEN];
    uint8_t nickname[NICKNAME_LEN];
    uint8_t recipient[NICKNAME_LEN];
} PrivateText;

typedef struct Greeting {
    uint8_t nickname[NICKNAME_LEN];
} Greeting;

typedef struct NicknameList {
    uint8_t nickname[NICKNAME_LEN];
} NicknameList;

typedef struct Ack {
    uint8_t code;
} Ack;

typedef struct DataTransfer {
    unsigned short port;
    char ip[INET_ADDRSTRLEN];
    char nickname[NICKNAME_LEN];
} DataTransfer;

typedef struct Disconnect {
    uint8_t nickname[NICKNAME_LEN];
} Disconnect;

typedef union {
    Text text;
    PrivateText privateText;
    Greeting greeting;
    NicknameList nicknameList;
    Ack ack;
    Disconnect disconnect;
    DataTransfer dataTransfer;
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
