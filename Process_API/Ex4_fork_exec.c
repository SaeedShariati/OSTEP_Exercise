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
        char* args[3];
        args[0] = "/bin/ls";
        args[1] = "/";
        args[2] = NULL;
        execvp("ls",args);
        //execl(args[0],"ls",args[1],args[2]);
        //execle(args[0],"ls",args[1],args[2],NULL);
        //execve(args[0],args,NULL);
        //execlp("ls","ls",args[1],args[2]);
        //execv(args[0],args);

    }
    else//parent process
    {
        wait(NULL);
    }
    
    return 0;
}
