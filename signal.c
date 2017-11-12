#include <stdio.h>
#include <errno.h>//随时给errno赋值
#include <error.h>//
#include "ename.c.inc"
#include <signal.h>
#include <stdlib.h>//EXIT_FAILURE

void outErr(const char *msg,const char *format,int errNo);

static void sigHand()
{
    printf("what the fuck, you just press Ctrl + c\n");
}

void main(int argc,char *argv[]){
    int j;

    if(signal(SIGINT,sigHand)==SIG_ERR){
        outErr("signal","%s",errno);
    }

    for(j=0;;j++)
    {
        printf("j is %d\n",j);
        sleep(2);
    }

}
void outErr(const char *msg,const char *format,int errNo){
    fflush(stdout); 
    errNo=errno;
    printf(format,msg,strerror(errNo));
    printf(format,msg,ename[errNo]);
    perror(msg);
    fflush(stderr); 
    exit(EXIT_FAILURE);
}
