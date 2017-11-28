#include "file_transfer.h"
#include <sys/socket.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <zconf.h>

void writePoll(int sockfd, char* input){
    struct pollfd pfd[1];
    pfd->fd = sockfd;
    pfd->events = POLLOUT;
    int rv = poll(pfd, 1, 100);

    if(pfd[0].revents & POLLOUT) {
        write(sockfd, input, strlen(input));
        return;
    }
}

void sendFile(int sockfd, char* fileName){
	char* lineToSend = NULL;
	size_t length = 0;

	char filename[64];

	FILE *fileToSend;
	sprintf(filename, "%s.txt", fileName);
	fileToSend = fopen(filename, "r");

	while(getline(&lineToSend, &length, fileToSend) == -1){
	    writePoll(sockfd, lineToSend);
	}
	fclose(fileToSend);
}

