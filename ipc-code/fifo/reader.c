#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FIFO_NAME "myfifo"

int main() {
    int fd;
    char buffer[100];

    // Open FIFO for reading
    fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Read from FIFO
    read(fd, buffer, sizeof(buffer));
    printf("Reader received message: %s", buffer);
    
    close(fd);
    return 0;
}
