#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<string.h>
int main(int argc, char* argv[]){
    int fd = open("./test.txt",O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU);
    int t = 100;
    int rc = fork();

    if(rc==0)//child process
    {
        printf("child t: %d\n",t);
        t=0;
        printf("changed child t %d\n",t);
    }
    else{//parent process
        printf("parent t: %d\n",t);
        t = 1;
        printf("changed parent t %d\n",t);
    }
    
    return 0;
}