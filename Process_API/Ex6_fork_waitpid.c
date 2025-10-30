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
        //if the below line is included it says child process didn't terminate normally in the parent
        int t = 2/0;
    }
    else{//parent process
        int status;
        int w = waitpid(rc,&status,0);
        if(WIFEXITED(status))
        {
            printf("[parent_%d]wait returned: %d\n",getpid(),w);
        }
        else{
            printf("child process didn't terminate normally\n");
        }     
    }
    return 0;
}