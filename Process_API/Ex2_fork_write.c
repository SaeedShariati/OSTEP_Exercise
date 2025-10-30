#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<string.h>
int main(int argc, char* argv[]){
    int fd = open("./test.txt",O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU);
    int rc = fork();
    if(rc==0)//child process
    {
        for(int i=0;i<500;i++){

            if(write(fd,"child",5)<5){
                fprintf(stderr,"child process write");
            }
        }
    }
    else{//parent process
        for(int i=0;i<500;i++){

            if(write(fd,"parent",6)<6){
                fprintf(stderr,"parent process write");
            }
        }
    }
    
    return 0;
}