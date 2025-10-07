#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_PATH "file_transfer"
#define CHUNK_SIZE 256

void error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sockfd, ffd;
    struct sockaddr_un srvr_addr;

    // Create socket
    if ((sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0)
        error("socket");

    memset(&srvr_addr, 0, sizeof(srvr_addr));
    srvr_addr.sun_family = AF_UNIX;
    strcpy(srvr_addr.sun_path, SOCK_PATH);

    // Open the file
    ffd = open(argv[1], O_RDONLY);
    if (ffd < 0)
        error("open");

    printf("Reading and sending file: %s\n", argv[1]);

    char chunk[CHUNK_SIZE];
    ssize_t n;
    socklen_t lensock = sizeof(srvr_addr);

    // Read and send in chunks
    while ((n = read(ffd, chunk, CHUNK_SIZE)) > 0)
    {
        if (sendto(sockfd, chunk, n, 0, (struct sockaddr *)&srvr_addr, lensock) < 0)
            error("sendto");
    }

    if (n < 0)
        error("read");

    // Send termination signal
    strcpy(chunk, "END");
    if (sendto(sockfd, chunk, strlen(chunk) + 1, 0,
               (struct sockaddr *)&srvr_addr, lensock) < 0)
        error("sendto END");

    printf("File sent successfully.\n");

    close(ffd);
    close(sockfd);
    return 0;
}
