#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
void chatfunc(int sockfd)
{
    char buff[MAX];
    int n; int control = 1;

    while(control != 0){
        bzero(buff, sizeof(buff));
        printf("Enter filename to request: ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n');//cin input

        write(sockfd, buff, sizeof(buff)); //send buffered input
        if(buff != NULL){
	  bzero(buff, sizeof(buff));
	  control = 0;
	}
      }
}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
  
    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
  
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
  
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
  
    // function for chat
    chatfunc(sockfd);
  
    // close the socket
    close(sockfd);
}
