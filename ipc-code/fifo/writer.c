#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO_NAME "myfifo"

int main() {
    int fd;
    char message[] = "Hello from writer!\n";

    // Create the FIFO if it doesn't exist
    mkfifo(FIFO_NAME, 0666);

    // Open FIFO for writing
    fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Write message to FIFO
    write(fd, message, sizeof(message));
    close(fd);

    printf("Writer sent message: %s", message);
    return 0;
}
