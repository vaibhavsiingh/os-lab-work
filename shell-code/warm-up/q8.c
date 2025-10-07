#include<stdio.h>
#include<sys/types.h>
#include<signal.h>

void sigint_handler(int signal){
    printf("Recieved signal %d\n", signal);
    printf("Me run forever\n");
}

int main(){
    signal(SIGINT,sigint_handler);
    while(1){

    }
}