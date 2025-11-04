#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<string.h>
int main(int argc, char* argv[]){

    pid_t pids[2];
    int fd[2]; //pipe
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    if(pipe(fd)<0)
        fprintf(stderr,"pipe unsuccessful\n");
    for(int i =0;i<2;i++){
        if((pids[i] = fork()) < 0)
            fprintf(stderr,"fork failed\n");
        else if(pids[i] == 0){
            //child processes
            //IMPORTANT: for child 1 i is 0, for child 2 i is 1

            if(i==0){
                //child 1(for reading)
                close(fd[1]);//close the write end
                char buffer[100];
                fprintf(stderr,"%d this is child 1(fd0_%d)\n",getpid(),fd[0]);
                int bytes_read;
                while ((scanf("%s",buffer)==1)){
                    fprintf(stderr,"%s\n",buffer);
                }
                close(fd[0]);
            }
            else if(i==1){
                //child 2(for writing)
                close(fd[0]);//close the read end
                fprintf(stderr,"%d this is child 2(fd1_%d)\n",getpid(),fd[1]);

                for(int j=0;j<20;j++){
                    sleep(1);
                    printf("apple peach carrot\n");
                    fflush(stdout);
                }
                close(fd[1]);
            }
            exit(0);
        }
    }
    //parent process(child processes have exited)
    fprintf(stderr,"parent\n");
    close(fd[0]);
    close(fd[1]);
    for(int i =0;i<2;i++){
        int status;
        wait(&status);
    }
    fprintf(stderr,"closed children\n");

    return 0;

}