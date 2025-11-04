#define _GNU_SOURCE
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<string.h>
#include<sched.h>
#include<pthread.h>
#include<sys/time.h>


double get_interval(struct timeval* start,struct timeval* end){
    long seconds= end->tv_sec - start->tv_sec;
    long microseconds = end->tv_usec - start->tv_usec;
    if(microseconds<0){
        seconds -= 1;
        microseconds += 1000000;
    }
    microseconds += seconds*1000000;
    return microseconds;
}
double measure_syscall_time(){
    int iterations = 100000;

    struct timeval start,end;
    gettimeofday(&(start),NULL);
    int t = 0;
    for(int i =0;i<iterations;i++)
        read(0,NULL,0);
    gettimeofday(&(end),NULL);

    double time_per_syscall = ((double)get_interval(&start,&end))/iterations;
    return time_per_syscall;
}
int main(int argc, char* argv[]){
    struct timeval start,end;
    int iterations = 100000;
    if(argc>1)
        iterations = atoi(argv[1]);
    
    pid_t pids[2];
    int fd1[2];
    int fd2[2];
    int fd3[2]; //for communication between parent and Process 2.
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(0,&cpuset);
    if(pipe(fd1)<0 || pipe(fd2)){
        fprintf(stderr,"pipe unsuccessful\n");
        exit(-1);
    }
    for(int i =0;i<2;i++){
        //fd3 is created after Process one has been created,
        //so only process 2 and the parent process have access to it
        if(i==1 && pipe(fd3)<0){
            fprintf(stderr,"fd3 pipe failed\n");
            exit(-1);
        }
        if((pids[i] = fork()) < 0){
            fprintf(stderr,"fork failed\n");
            exit(-1);
        }
        if(pids[i] == 0){
            //child processes
            //IMPORTANT: for child 1 i is 0, for child 2 i is 1
            if(i==0){
                sched_setaffinity(getpid(),sizeof(cpuset),&cpuset); //allow process to only run on CPU 0
                //child 1 (read from fd1 and write to fd2)
                char buffer;
                close(fd1[1]);//close write end
                close(fd2[0]);//close read end
                for(int j =0;j<iterations;j++){   
                    write(fd2[1],"1",1);
                    read(fd1[0],&buffer,1);
                }
                close(fd2[1]);
                close(fd1[0]);
            }
            else if(i==1){
                sched_setaffinity(getpid(),sizeof(cpuset),&cpuset);//allow process to only run on CPU 0
                //child 2 (write to fd1 and read from fd2)
                char buffer;
                close(fd1[0]);//close read end
                close(fd2[1]);//close write end
                close(fd3[0]);//close read end
                gettimeofday(&start,NULL);
                for(int j =0;j<iterations;j++){   
                    read(fd2[0],&buffer,1);
                    write(fd1[1],"2",1);
                }
                gettimeofday(&end,NULL);
                long time = get_interval(&start,&end);

                //rate is for a round trip, so 2 context switches and 4 system calls
                double rate = (double)time/((double)iterations);
                write(fd3[1],(void*)&rate,sizeof(double));

                close(fd3[1]);
                close(fd2[0]);
                close(fd1[1]);
            }
            exit(0);
        }


    }
    //parent process(child processes have exited)
    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);
    close(fd3[1]);
    double total_time;
    int bytes_read = 0;
    while (bytes_read < sizeof(double)) {
        bytes_read += read(fd3[0],((void*)&total_time)+bytes_read,sizeof(double)-bytes_read);
    }
    close(fd3[0]);
    double syscall_time = measure_syscall_time();
    double context_switch = (total_time - 4*syscall_time)/2;
    for(int i =0;i<2;i++){
        int status;
        wait(&status);
    }
    printf("context_switch_time: %4.2f\n",context_switch);

    return 0;

}