#include <stdio.h>
// #include <sys/stat.h>
// #include <sys/uio.h>
// #include <fcntl.h>
#include <errno.h>//随时给errno赋值
#include <error.h>//
#include <string.h>//
#include <stdlib.h>//
#include "ename.c.inc"


void outErr(const char *msg,const char *format,int errNo,int exitNo);

void main(int argc,char *argv[]){
    int Fd;
    char template[]="/tmp/huangbyXXXXXX";
    Fd=mkstemp(template);

    if(Fd==-1){
        outErr(template,"%s: %s\n",errno,0);
    }

    printf("the temp file is %s \n",template);
    
    unlink(template);
    if(close(Fd)==-1){
        outErr("close err","%s: %s\n",errno,0);
    }
    exit(EXIT_SUCCESS);

}
void outErr(const char *msg,const char *format,int errNo,int exitNo){
    fflush(stdout); 
    errNo=errno;
    printf(format,msg,strerror(errNo));
    printf(format,msg,ename[errNo]);
    perror(msg);
    fflush(stderr); 
    exit(0);
}
