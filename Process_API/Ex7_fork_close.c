#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<string.h>
int main(int argc, char* argv[]){
    int w=0;
    int rc = fork();

    if(rc==0)//child process
    {
        close(STDOUT_FILENO);
        printf("[child_%d]\n",getpid());
    }
    else{//parent process
            
        printf("[parent_%d]\n",getpid());
    }
    
    return 0;
}