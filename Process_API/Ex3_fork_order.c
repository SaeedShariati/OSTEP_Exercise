#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<string.h>
int main(int argc, char* argv[]){
    int rc = fork();

    if(rc==0)//child process
    {
        printf("hello\n");
    }
    else{//parent process
        sleep(1);
        printf("goodbye\n");
    }
    
    return 0;
}
