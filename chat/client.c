
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <zconf.h>
#define BUFFER_SIZE 10000
#define CONN_ALIVE 0
#define CONN_END 1
#define CONN_STOP_SEND 2

int registerNickname(int sockfd, char* nickname){
	//Todo
}
int handleLogic(int sockfd, char* nickname) {

	if(!registerNickname(sockfd, nickname)){
        perror("Failed to register Nickname!");
	}

	while(1){ //Change this loop with new message struct
		char recvline[BUFFER_SIZE];
		char sendline[BUFFER_SIZE];

		bzero(sendline, BUFFER_SIZE);
		bzero(recvline, BUFFER_SIZE);

		fgets(sendline, BUFFER_SIZE, stdin);
		if(strcmp(sendline, "exit\n") == 0) {
		  closeConnection(sockfd);
		}

		write(sockfd, sendline, strlen(sendline));
		read(sockfd, recvline, BUFFER_SIZE);
		printf("\n%s\n$  ",recvline);
	}

}

void connectionCallback(int sockfd, struct sockaddr_in servaddr, char* nickname) {
    handleLogic(sockfd, nickname);
    close(sockfd);
}

void createConnection(char *ip, unsigned int port, char *nickname, void (*callback)(int, struct sockaddr_in), char*) {
    struct sockaddr_in servaddr;
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket Error");
        exit(1);
    }

    //Setting servaddr parameters
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0) {
        perror("inet_pton Error");
        exit(1);
    }

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("Connect Error");
        exit(1);
    }

    callback(sockfd, servaddr, nickname);
}

int main(int argc, char **argv) {
    if (argc < 4) {
        char error[BUFFER_SIZE];
        strcpy(error, "uso: ");
        strcat(error, argv[0]);
        strcat(error, "Server <IPaddress>");
        strcat(error, ":<Port>");
        strcat(error, " <YourNickname>");
        perror(error);
        return 1;
    }

    createConnection(argv[1], atoi(argv[2]), argv[3], connectionCallback);
    return 0;
}