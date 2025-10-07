#include<stdio.h>
#include<sys/types.h>

int main(int argc, char* argv[]){
    if (argc != 3){
        printf("ERROR: Incorrect number of arguments. 2 required\n");
        return 1;
    }

    pid_t child = fork();
    if(child < 0){
        perror("Something went wrong");
    }
    else if(child ==0){
        char* args[] = {argv[1],argv[2],NULL};
        execvp(argv[1],args);
        perror("Error executing the command");
    }
    else{
        int x = wait(NULL);
        if(x>=0){
            printf("Command executed succesfully\n");
        }
        else{
            perror("Child returned with error");
        }
    }
    

}