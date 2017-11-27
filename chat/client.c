#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include "lib/commons.h"
#include "lib/client/utils.h"
#include "lib/client/executors.h"
#include "lib/interfaces.h"

void messageHandler(Message m) {
    switch (m.body.type) {
        case TEXT:
            //TODO: NonBlocking IO
            printf("%s: %s\n", m.body.data.text.nickname, m.body.data.text.body);
            return;
        case NICKNAME_LIST:
            //TODO: NonBlocking IO
            printf("%s is connected\n", m.body.data.nicknameList.nickname);
            return;
        default:
            return;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("usage: main <hostname> <port> <nickname>\n");
        return 0;
    }

    char *ip = resolveHostname(argv[1]);
    uint16_t port = (uint16_t) atoi(argv[2]);
    int sockfd = createSocket();

    initMessageReceiver(sockfd, messageHandler);
    sendGreetings(sockfd, ip, port, argv[3]);

    char input[MESSAGE_LEN];
    for (;;) {
        //TODO: NonBlocking IO
        scanf("%s", input);
        Message m = message(ip, port, createTextBody(input, 0));
        sendMessage(sockfd, m);
    }

    return 0;
}