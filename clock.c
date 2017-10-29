/**
计数单位
times 
    _SC_CLK_TCK 每秒包含的时钟基数单元数

clock
    CLOCKS_PER_SEC 

*/
#include <stdio.h>
#include <errno.h>//随时给errno赋值
#include <error.h>//
#include "ename.c.inc"
#include <time.h>
#include <sys/times.h>
#include <unistd.h>//_SC_CLK_TCK未声明
#include <stdlib.h>//EXIT_SUCCESS
void outErr(const char *msg,const char *format,int errNo);
static void displayProcess(const char *msg)
{
    struct tms t;
    clock_t clockTime;
    static long clockTicks =0;
    if(msg !=NULL)
        printf("%s\n",msg);
    if(clockTicks==0){
        clockTicks=sysconf(_SC_CLK_TCK);
        if(clockTicks==-1)
            outErr("sysconf","%s",errno);
    }

    clockTime = clock();

    printf("clock() return is %ld  divide _CLOCKS_PER_SEC is (%.2f secs)\n",(long) clockTime,(double) clockTime/CLOCKS_PER_SEC);

    if(times(&t)==-1)
        outErr("times","%s",errno);
    printf("times() utime is %.2f,stime is %.2f,cutime is %.2f sctime is %.2f \n",
    (double)t.tms_utime,(double)t.tms_stime,(double)t.tms_cutime,(double)t.tms_cstime);

}

void main(void){
    int Calls = 10000000,i;
    printf("_SC_CLK_TCK = %ld and CLOCKS_PER_SEC = %ld \n",(long) sysconf(_SC_CLK_TCK),(long)CLOCKS_PER_SEC);
    displayProcess("==========================begin==========================");
    for(i = 0;i<Calls;i++)
        getppid();
    displayProcess("==========================over===========================");
    exit(EXIT_SUCCESS);
}

void outErr(const char *msg,const char *format,int errNo){
    fflush(stdout); 
    errNo=errno;
    printf(format,msg,strerror(errNo));
    printf(format,msg,ename[errNo]);
    perror(msg);
    fflush(stderr); 
    exit(0);
}