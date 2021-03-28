//CPEG 450 - PROJECT 1 - ZHANG RUI - FILE AUTHORS: TREVOR ROE, NOAH HODGHSON
//Server.c contains two helper functions, chatfunc and send_file and a main func
//Main function handles creating tcp socket, listening and waiting for client connect and acknowledging it
//chatfunc - facilitates transferring a single string between server and client, and saves this string as filename being requested by client
//send_file - facilitates full file transfer


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
  int n, c, bytetotal, check = 0;
  char data[SIZE] = {0};
  n = 0; bytetotal = 0;

  while((fgets(data, SIZE, fp)) != NULL) {
    bzero(data, SIZE);
    check = NULL; //reset chkval
    check = write(sockfd, data, SIZE, 0);
    if(check > 0){
      n++; //only iterate n when packets are sent
    } else if (check == 0){
      printf("eot packet detected\n");
    }
     c = sizeof(data) / sizeof(data[0]);
     printf("Packet %d transmitted with %d data bytes\n", n, c);
     bytetotal += c;
     }
     printf("End of Transmission Packet with sequence number %d transmitted with %d data bytes\n", ++n, 0);
     printf("Number of data bytes transmitted: %d\n", bytetotal);
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
    printf("[+]File sent.\n");

    // After chatting close the socket
    printf("Closing connection.\n");
    close(sockfd);
}
