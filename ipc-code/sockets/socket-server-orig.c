#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_PATH "unix_socket_example"
#define CHUNK_SIZE 256

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd;
     char buffer[CHUNK_SIZE];
     struct sockaddr_un serv_addr, cli_addr;
     int n;

     /* create socket */

     sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");

     /* fill in socket addres */
     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sun_family = AF_UNIX;
     strcpy(serv_addr.sun_path, SOCK_PATH);

     /* bind socket to this address */
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     
     /* read message from client */

     bzero(buffer,256);
     int len = sizeof(cli_addr);

     printf("Server ready\n");
     
     n = recvfrom(sockfd, buffer, 255, 0, (struct sockaddr *)&cli_addr, &len);
     
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n",buffer);

     unlink(SOCK_PATH);
     return 0; 
}
