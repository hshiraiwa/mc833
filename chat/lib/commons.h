#ifndef CHAT_COMMONS_H
#define CHAT_COMMONS_H

#include "interfaces.h"

Message recvMessage(int sockfd);

void sendMessage(int sockfd, Message message);

Message message(char *ip, uint16_t port);

int createSocket();

void bindSocket(int sockfd, uint16_t port);

#endif //CHAT_COMMONS_H