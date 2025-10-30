#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/time.h>
int main(int argc,char* argv[]){
    int iterations;
    if(argc==1)
        iterations = 10*1000*1000;
    else
        iterations = atoi(argv[1]);

    struct timeval start,end;
    gettimeofday(&(start),NULL);
    int t = 0;
    for(int i =0;i<iterations;i++)
        read(0,NULL,0);
    gettimeofday(&(end),NULL);
    long seconds= end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    if(microseconds<0){
        seconds -= 1;
        microseconds += 1000000;
    }
    microseconds += seconds*1000000;
    double time_per_syscall = ((double)microseconds)/iterations;
    printf("microseconds %d\n",microseconds);
    printf("syscalls     %d\n",iterations);
    printf("time per syscall in microseconds =%f\n",time_per_syscall);

    return 0;
}