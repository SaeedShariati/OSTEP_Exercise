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
        int w = wait(NULL);
        printf("[child_%d]wait returned: %d\n",getpid(),w);
    }
    else{//parent process
        int w = wait(NULL);     
        printf("[parent_%d]wait returned: %d\n",getpid(),w);
    }
    
    return 0;
}