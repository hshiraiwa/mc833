#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include "lib/commons.h"
#include "lib/client/utils.h"
#include "lib/client/executors.h"

void messageHandler(Message m) {
    switch (m.body.type) {
        case TEXT:
            //TODO: NonBlocking IO
            printf("[GLOBAL] %s: %s\n", m.body.data.text.nickname, m.body.data.text.body);
            return;
        case PRIVATE_TEXT:
            printf("[PM] %s: %s\n", m.body.data.privateText.nickname, m.body.data.text.body);
            return;
        case NICKNAME_LIST:
            //TODO: NonBlocking IO
            printf("%s is connected\n", m.body.data.nicknameList.nickname);
            return;
        case ACK:
            switch (m.body.data.ack.code) {
                case GREETING:
                    printf("Successfully connected to server\n");
                    return;
                case DISCONNECT:
                    printf("Nickname already in use, try a different one\n");
                    exit(0);
                default:
                    return;
            }
        case DISCONNECT:
            printf("%s has disconnected\n", m.body.data.disconnect.nickname);
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

    char pmNickname[NICKNAME_LEN];
    short isPm = 0;

    char input[MESSAGE_LEN];
    for (;;) {
        //TODO: NonBlocking IO
        scanf("%s", input);
        if (strcmp(input, "/exit") == 0) {
            Message m = message(ip, port, createDisconnectBody(argv[3]));
            sendMessage(sockfd, m);
            return 0;

        } else if (strcmp(input, "/pm") == 0) {
            printf("input the nickname you want to send private messages: ");
            scanf("%s", pmNickname);
            isPm = 1;
        } else if (strcmp(input, "/global") == 0) {
            isPm = 0;
        } else {
            if (isPm) {
                Message m = message(ip, port, createPrivateTextBody(input, pmNickname, 0));
                sendMessage(sockfd, m);
            } else {
                Message m = message(ip, port, createTextBody(input, 0));
                sendMessage(sockfd, m);
            }
        }
    }
}