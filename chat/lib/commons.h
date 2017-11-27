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

/**
 * Extracts a new client from a GREETING message
 * It does not check for wrong message type, may have weird results
 * @param m - Message to be extracted
 * @return Client extracted from the message
 */
Client extractClient(Message m);

/**
 * Create a message body for a text message
 * @param message - message main text
 * @param nickname - source's nickname (when sending to server may be filled with 0)
 * @return A built message body
 */
MessageBody createTextBody(char *message, char *nickname);

/**
 * Create a message body for a greeting message
 * @param nickname - nickname the user wants to register to itself
 * @return A built message body
 */
MessageBody createGreetingBody(char *nickname);

/**
 * Create a message body for a ack message
 * @param type - Message type for this ack
 * @return A built message body
 */
MessageBody createAckMessage(DataType type);

/**
 * Create a message body for sending the nickname list
 * @param client - Client to be added as nickname list
 * @return A built message body
 */
MessageBody createNicknameListMessage(Client client);

#endif //CHAT_COMMONS_H