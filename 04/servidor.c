#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>

#define LISTENQ 10
#define BUFFER_SIZE 200

int getPeerPort(struct sockaddr_in s) {
  return ntohs(s.sin_port);
}

char* getPeerIP(struct sockaddr_in s){
    char* ip = malloc(sizeof(char) * INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(s.sin_addr), ip, INET_ADDRSTRLEN);
    return ip;
}

void logString(struct sockaddr_in s, const char* data) {
  time_t ticks = time(NULL);
  char* ip = getPeerIP(s);
  int port = getPeerPort(s);
  FILE* file = fopen("servidor.log", "a");
  fprintf(file, "%.24s - %s:%d - %s\n", ctime(&ticks), ip, port, data);
  fclose(file);
  free(ip);
}

void handleConnection(int listenfd, void (*callback)(int, struct sockaddr_in)) {
  struct sockaddr_in storage;
  socklen_t storage_len = sizeof(storage);

  while(1){
    sleep(1);
    int connfd = accept(listenfd, (struct sockaddr*) &storage, &storage_len);
    if(connfd == -1) {
      perror("Connection Error");
      exit(1);
    }
    if(fork() == 0) {
      close(listenfd);
      logString(storage, "Connected");
      callback(connfd, storage);
      close(connfd);
      logString(storage, "Disconnected");
      exit(0);
    }

    close(connfd);
  }

}

void createServer(unsigned int port, unsigned int backlog, void (*callback)(int, struct sockaddr_in)) {
  struct sockaddr_in servaddr;
  int listenfd;
  if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Socket Error");
    exit(1);
  }
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  if( bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1) {
    perror("Bind Error");
    exit(1);
  }

  if(listen(listenfd, backlog) == -1){
    perror("Listen Error");
    exit(1);
  }

  handleConnection(listenfd, callback);
  return;
}

char* getResponse(const char* request) {
  char* response = malloc(sizeof(char) * BUFFER_SIZE);

  FILE *fpipe = (FILE*)popen(request,"r");
  char line[256];
  int i = 0;
  while ( fgets( line, sizeof(line), fpipe)) {
   snprintf(&response[i], sizeof(char) * (BUFFER_SIZE - i), "%s", line);
   i = strlen(response);
  }
  pclose(fpipe);

  return response;
}

void connectionCallback(int connfd, struct sockaddr_in connaddr) {
    char buf[BUFFER_SIZE];

    while(1){
      char* response;
      bzero(buf, sizeof(buf));
      if(read(connfd,buf,100) != 0 ) {
        char* ip = getPeerIP(connaddr);
        int port = getPeerPort(connaddr);
        printf("%s:%d - %s\n", ip, port, buf);
        free(ip);

        response = getResponse(buf);
        write(connfd, response, strlen(response)+1);
        free(response);
      } else {
        return;
      }
    }
}

int main (int argc, char **argv) {
  unsigned int backlog = argc < 3 ? LISTENQ : atoi(argv[2]);
  createServer(atoi(argv[1]), backlog, connectionCallback);

  return(0);
}
