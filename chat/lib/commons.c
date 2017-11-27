#include "commons.h"

Message recvMessage(int sockfd) {
    MessageBody body;
    bzero(&body, sizeof body);

    struct sockaddr_storage storage;
    struct sockaddr_in *sin = (struct sockaddr_in *) &storage;

    socklen_t sin_len = sizeof(storage);
    bzero(&storage, sizeof(storage));

    recvfrom(sockfd, &body, sizeof body, 0, (struct sockaddr *) sin, &sin_len);

    Message message;
    message.body = body;
    inet_ntop(AF_INET, &sin->sin_addr, message.ip, INET_ADDRSTRLEN);
    message.port = ntohs(sin->sin_port);

    return message;
}

void sendMessage(int sockfd, Message message) {
    struct sockaddr_storage storage;
    struct sockaddr_in *sin = (struct sockaddr_in *) &storage;
    bzero(&storage, sizeof storage);

    sin->sin_family = AF_INET;
    if (inet_pton(AF_INET, message.ip, &sin->sin_addr) <= 0) {
        perror("ERROR: message could not be sent");
        return;
    }
    sin->sin_port = htons(message.port);

    sendto(sockfd, &message.body, sizeof message.body, 0, (struct sockaddr *) sin, sizeof(storage));
}

Message message(char *ip, unsigned short port, MessageBody body) {
    Message m;
    bzero(&m, sizeof m);

    bcopy(ip, m.ip, INET_ADDRSTRLEN);
    m.port = port;
    m.body = body;

    return m;
}

int createSocket() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("ERROR: Socket could not be created");
        exit(1);
    }

    return sockfd;
}

void bindSocket(int sockfd, uint16_t port) {
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof servaddr);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof servaddr) < 0) {
        perror("ERROR: Socket could not bind to port: " + port);
        exit(1);
    }
}

Client extractClient(Message m) {
    Client c;
    c.port = m.port;
    strcpy(c.ip, m.ip);
    if (m.body.type == GREETING)
        strcpy(c.nickname, (char *) m.body.data.greeting.nickname);
    return c;
}

MessageBody createDisconnectBody(char *nickname) {
    MessageBody body;
    bzero(&body, sizeof body);
    body.type = DISCONNECT;
    strcpy((char *) body.data.disconnect.nickname, nickname);

    return body;
}

MessageBody createPrivateTextBody(char *message, char *recipient, char *nickname) {
    MessageBody body;
    bzero(&body, sizeof body);
    body.type = PRIVATE_TEXT;
    strcpy((char *) body.data.privateText.body, message);
    if (nickname != 0)
        strcpy((char *) body.data.privateText.nickname, nickname);

    if (recipient != 0)
        strcpy((char *) body.data.privateText.recipient, recipient);

    return body;
}

MessageBody createTextBody(char *message, char *nickname) {
    MessageBody body;
    bzero(&body, sizeof body);
    body.type = TEXT;
    strcpy((char *) body.data.text.body, message);
    if (nickname != 0)
        strcpy((char *) body.data.text.nickname, nickname);

    return body;
}

MessageBody createGreetingBody(char *nickname) {
    MessageBody body;
    bzero(&body, sizeof body);
    body.type = GREETING;
    strcpy((char *) body.data.greeting.nickname, nickname);

    return body;
}

MessageBody createAckMessage(DataType type) {
    MessageBody body;
    bzero(&body, sizeof body);
    body.type = ACK;
    body.data.ack.code = type;

    return body;
}

MessageBody createNicknameListMessage(Client client) {
    MessageBody body;
    bzero(&body, sizeof body);
    body.type = NICKNAME_LIST;
    strcpy((char *) body.data.nicknameList.nickname, client.nickname);

    return body;
}
