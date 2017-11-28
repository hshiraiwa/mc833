#ifndef FILE_TRANSFER_H
#define FILE_TRANSFER_H

#endif //FILE_TRANSFER_H

//Writes input in a poll to the given socked
void writePoll(int sockfd, char* input);

//Sends a file with fileName to the given sockfd 
void sendFile(int sockfd, char* fileName);
