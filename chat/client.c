#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include "lib/commons.h"
#include "lib/client/utils.h"
#include "lib/client/executors.h"

void messageHandler(Message m) {
    if (m.body.type == TEXT)
        printf("%s: %s\n", m.body.data.text.nickname, m.body.data.text.body);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("usage: main <hostname> <port> <nickname>\n");
        return 0;
    }

    char *ip = resolveHostname(argv[1]);
    uint16_t port = (uint16_t) atoi(argv[2]);
    int sockfd = createSocket();

    sendGreetings(sockfd, ip, port, argv[3]);

    initMessageReceiver(sockfd, messageHandler);

    char input[MESSAGE_LEN];
    for(;;) {
        scanf("%s", input);
        Message m = message(ip, port, createTextBody(input, 0));
        sendMessage(sockfd, m);
    }

    return 0;
}