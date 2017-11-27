#ifndef CHAT_COMMONS_H
#define CHAT_COMMONS_H

#include <strings.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interfaces.h"

/**
 * Receives a UDP message from a open socket
 * It blocks until a message is received
 * @param sockfd - open socket file descriptor
 * @return Received message
 */
Message recvMessage(int sockfd);

/**
 * Sends a message to a open socket
 * It blocks until the message is sent
 * @param sockfd - open socket file descriptor
 * @param message - Message to be sent
 */
void sendMessage(int sockfd, Message message);

/**
 * Initializes a message header to a given recipient
 * @param ip - recipient ip
 * @param port - recipient port
 * @param body - body to be sent
 * @return Built message
 */
Message message(char *ip, uint16_t port, MessageBody body);

/**
 * Initialized a socket and returns a socket file descriptor
 * @return Socket file descriptor
 */
int createSocket();

/**
 * Bind a socket to an port
 * @param sockfd - Socket file descriptor
 * @param port - Port to be bound
 */
void bindSocket(int sockfd, uint16_t port);

Client extractClient(Message m);

MessageBody createTextBody(char *message, char *nickname);

MessageBody createGreetingBody(char *nickname);

MessageBody createAckMessage(DataType type);

#endif //CHAT_COMMONS_H