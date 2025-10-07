/**
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the consumer process
 *
 * Figure 3.18
 *
 * @author Gagne, Galvin, Silberschatz
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <fcntl.h>
 #include <sys/shm.h>
 #include <sys/stat.h>
 #include <sys/mman.h>
  #include <sys/msg.h>
 #include <sys/ipc.h>
 #include <unistd.h>
 #include<string.h>

 #define MSG_SIZE 10

 struct msg_buffer{
    long msg_type;
    char msg_text[MSG_SIZE];
 };
 
 int main()
 {
    const int SIZE = 4096;
    int MAX_COUNT = 1000;
    const char *name = "comc1";
    const char *free_message = "freeeee";
    //const char *occupied_message = "OSisFUN";
    // const char *writer_read_fifo = "comc2";
    // const char *deleter_read_fifo = "comc3"
 
     int shm_fd;
     void *ptr;
 
     /* open the shared memory segment */
     shm_fd = shm_open(name, O_RDWR, 0666);
     if (shm_fd == -1) {
         printf("shared memory failed\n");
         exit(-1);
     }
 
     /* now map the shared memory segment in the address space of the process */
     ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
     if (ptr == MAP_FAILED) {
         printf("Map failed\n");
         exit(-1);
     }

     struct msg_buffer message;
     key_t key;
     int msgid;

     key = ftok("progfile",65);
     if(key==-1){
        perror("ftok");
        exit(1);
     }

     msgid = msgget(key, 0666 | IPC_CREAT);
     if(msgid==-1){
        perror("msgget");
        exit(1);
     }

    // int writer_read_fd = open(writer_read_fifo,O_WRONLY);
    // int deleter_read_fd = open(deleter_read_fifo, O_RDONLY);
    int count = 0;

    while(count < MAX_COUNT){
        if(msgrcv(msgid, &message, sizeof(message.msg_text),2,0)==-1){
            perror("msgrcv");
            exit(1);
        }      
        int offset = atoi(message.msg_text);
        //char string_deleted[10];
        printf("string deleted: %s at i=%d\n", (char*)(ptr+offset), offset);
        sprintf(ptr+offset,"%s",free_message);
        message.msg_type = 1;
        if(msgsnd(msgid, &message, sizeof(message.msg_text),0)==-1){
            perror("msgsnd");
        
        } 
        count++;
      //  sleep(1);
    }

 
//while(count<MAX_COUNT){
        // for(int i=0; i<SIZE; i+=8){
        //     char x[8];
        //     sprintf(x,"%s",(char*) ptr+i);
        //     if(strcmp(x,occupied_message)==0){
        //         printf("%s found at offset = %d \n",x,i);
        //         sprintf(ptr+i,"%s",free_message);
        //         sleep(1);
        //        count++;
        //     }
        //     else if (strcmp(x,free_message)!=0) {
        //         printf("Shared memory occupied by unknow string: %s\n", x);
        //     }
        // }
     //}


     printf("Delter: My job done \n");
 
     /* remove the shared memory segment */
     if (shm_unlink(name) == -1) {
         printf("Error removing %s\n",name);
         exit(-1);
     }
 
     return 0;
 }
 