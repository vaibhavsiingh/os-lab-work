#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_PATH "file_transfer"
#define BUF_SIZE 256

void error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int main()
{
    int sockfd;
    struct sockaddr_un srvr_addr, client_addr;
    char buf[BUF_SIZE];

    // Create datagram socket
    if ((sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0)
        error("socket");

    // Remove existing socket file (important)
    unlink(SOCK_PATH);

    memset(&srvr_addr, 0, sizeof(srvr_addr));
    srvr_addr.sun_family = AF_UNIX;
    strcpy(srvr_addr.sun_path, SOCK_PATH);

    // Bind socket to address
    if (bind(sockfd, (struct sockaddr *)&srvr_addr, sizeof(srvr_addr)) < 0)
        error("bind");

    printf("Server ready. Waiting for data...\n");

    while (1)
    {
        memset(buf, 0, sizeof(buf));
        socklen_t len = sizeof(client_addr);

        ssize_t n = recvfrom(sockfd, buf, sizeof(buf) - 1, 0,
                             (struct sockaddr *)&client_addr, &len);
        if (n < 0)
            error("recvfrom");

        buf[n] = '\0'; // ensure null termination

        // Check for end signal
        if (strcmp(buf, "END") == 0)
        {
            printf("\nFile transfer completed.\n");
            break;
        }

        printf("%s", buf);
        fflush(stdout);
    }

    close(sockfd);
    unlink(SOCK_PATH);
    return 0;
}
