/**
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the producer process that writes to the shared memory region.
 *
 * Figure 3.17
 *
 * @author Silberschatz, Galvin, and Gagne
 * Operating System Concepts  - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

 #include <unistd.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <fcntl.h>
 #include <sys/msg.h>
 #include <sys/ipc.h>
 #include <sys/shm.h>
 #include <sys/stat.h>
 #include <sys/types.h>
 #include <sys/mman.h>

 #define MSG_SIZE 10


 struct msg_buffer{
    long msg_type;
    char msg_text[MSG_SIZE];
 };


 
 int main()
 {
    const int SIZE = 4096;
    int MAX_COUNT = 1000;
    const char *shm_name = "comc1";
    const char *free_message = "freeeee";
    const char *occupied_message = "OSisFUN";
    
    int shm_fd;
    void *ptr;

    
    /* create the shared memory segment */
    shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    
    /* configure the size of the shared memory segment */
    ftruncate(shm_fd,SIZE);
    
    /* now map the shared memory segment in the address space of the process */
    ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        printf("Map failed\n");
        return -1;
    }

    key_t key;
    int msgid;
    struct msg_buffer message;

    key = ftok("progfile",65);
    if(key==-1){
        perror("ftok error");
        exit(1);
    }

    msgid = msgget(key, 0666 | IPC_CREAT);
    if(msgid==-1){
        perror("msgget");
        exit(1);
    }

    

    
    /**
     * Now write to the shared memory region.
     *
     * Note we must increment the value of ptr after each write.
     */
    
    // mkfifo(writer_read_fifo,O_CREAT, O_RDWR);
    // mkfifo(writer_read_fifo,0666);
    // int writer_read_fd = open(writer_read_fifo,O_RDONLY);

    // mkfifo(deleter_read_fifo,0666);
    // int deleter_read_fd = open(deleter_read_fifo, O_WRONLY);

    for(int i=0; i<SIZE; i+=8){
        sprintf(ptr+i,"%s",free_message);
        message.msg_type = 1;
        char str_to_send[MSG_SIZE];
        sprintf(str_to_send, "%d", i);
        strcpy(message.msg_text, str_to_send);
        if(msgsnd(msgid, &message, sizeof(message.msg_text),0)==-1){
            perror("msgsnd");
            exit(1);
        }
    }

    int count = 0;
    while(count < MAX_COUNT){
        if(msgrcv(msgid,&message,sizeof(message.msg_text),1,0)==-1){
            perror("msgrcv");
            exit(1);
        }
        int offset = atoi(message.msg_text);
        sprintf(ptr+offset,"%s",occupied_message);
        printf("Occupied offset = %d\n", offset);
        message.msg_type=2;
        if(msgsnd(msgid,&message,sizeof(message.msg_text),0)==-1){
            perror("msgsnd");
            exit(1);
        }
        count++;
    }

    // while(count<MAX_COUNT){
    // for(int i=0; i<SIZE; i+=8){
    //     char x[8];
    //     sprintf(x,"%s",(char*)ptr+i);
    //     if(strcmp(x,free_message)==0){
        //         sprintf(ptr+i,"%s",occupied_message);
        //         printf("Filled message at position: %d\n",i);
        //         count++;
        //     }
        //     else if (strcmp(x,occupied_message)!=0) {
            //         printf("Shared memory occupied by unknow string: %s\n", x);
            //     }
            // }
        
    // }
    printf("Writer: Completed my Job\n");
    
    return 0;
}
