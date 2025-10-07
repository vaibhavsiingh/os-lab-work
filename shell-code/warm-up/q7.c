#include<stdio.h>
#include<sys/types.h>

int main(){
    pid_t child = fork();
    if(child == 0){
        printf("Started executing child\n");
        sleep(10);
    }
    else{
        sleep(1);
        printf("Killing my son of a bitch child\n");
        int x = kill(child,9);
        if(!x){
            printf("Killed Successfully\n");
        }
        else{
            perror("Couldnt kill");
            return 1;
        }
        printf("Now reaping\n");
        if(wait(NULL)==0) printf("Reaped successfully\n");
        return 0;


    }
}