#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define SIZE 80
#define PORT 8080
#define SA struct sockaddr

char filename[SIZE];

// Function designed for chat between client and server.
void chatfunc(int sockfd)
{
    char buff[SIZE];
    int n, control;
    control = 1;
    while(control != 0) {
        bzero(buff, SIZE);

        // read the message from client and copy it in buffer
        read(sockfd, buff, sizeof(buff));
	strcpy(filename, buff);

	//copy buffer contents then clear buffer
        bzero(buff, SIZE);
	if(filename != NULL){
	  control = 0;
	}
    }
}


void send_file(FILE *fp, int sockfd){
  int n;
  char data[SIZE] = {0};

  while(fgets(data, SIZE, fp) != NULL) {
    if (send(sockfd, data, sizeof(data), 0) == -1) {
      perror("[-]Error in sending file.");
      exit(1);
    }
    bzero(data, SIZE);
  }
}


int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;
    FILE *fp;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("[!]Socket creation failed\n");
        exit(0);
    }
    else
        printf("[+]Socket successfully created\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("[+]Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("[!]Listen failed...\n");
        exit(0);
    }
    else
        printf("[+]Server listening...\n");
    len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("[!]Server acccept failed\n");
        exit(0);
    }
    else
        printf("[+]Client has connected\n");

    // Chat with client to get filename
    chatfunc(connfd);
    printf("File '%s' requested by client.\n", filename);

    fp = fopen(filename, "r");//parse file
    if(fp != NULL){ printf("Responding...\n"); } else { printf("Could not find %s", filename); }//give feedback
    send_file(fp, connfd);//send file
    printf("[+]File sent.");

    // After chatting close the socket
    close(sockfd);
}
