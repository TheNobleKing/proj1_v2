#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#define SIZE 80
#define PORT 8080
#define SA struct sockaddr


void chatfunc(int sockfd)
{
    char buff[SIZE];
    int n; int control = 1;

    while(control != 0){
        bzero(buff, sizeof(buff));
        printf("Enter filename to request: ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n');//cin input

	buff[strcspn(buff, "\n")] = 0;

        write(sockfd, buff, sizeof(buff)); //send buffered input
        if(buff != NULL){
	  bzero(buff, sizeof(buff));
	  control = 0;
	}
      }
}

void write_file(int sockfd){
  int n;
  FILE *fp;
  char *filename = "out.txt";
  char buffer[SIZE];

  fp = fopen(filename, "w"); //create out.txt
  while (1) {
    n = recv(sockfd, buffer, SIZE, 0); //copy buffer each time it is updated
    printf("Recieving packet %n \n", n);
    if (n <= 0){
      break;
      return;
    }
    fprintf(fp, "%s", buffer); //o_o
    bzero(buffer, SIZE);
  }
  return;
}


int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
   // struct sockaddr_in servaddr, new_addr;
    int new_sock;
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
  
    // function for chat, send file request
    chatfunc(sockfd);

    //struct sockaddr_in serv_addr, new_addr;
    //socklen_t addr_size = sizeof(new_addr);

    new_sock = accept(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	printf("new socket created");
    //write_file(new_sock);
    //printf("[+]Data written in the file successfully.\n");

    // close the socket
    close(sockfd);
}
