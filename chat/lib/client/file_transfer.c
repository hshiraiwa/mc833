#include "file_transfer.h"

void writePoll(int sockfd, char* input){
    struct pollfd pfd[1];
    pfd->fd = sockfd;
    pfd->events = POLLOUT;
    int rv = poll(pfd, 1, TIMEOUT);

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
	sprintf(filename, "%c.txt", fileName)
	fileToSend = fopen(filename, "r");

	while(getline(&lineToSend, &length, fileToSend) == -1){
	    writePoll(sockfd, lineToSend);
	}
	fclose(fileToSend);
}

