#include <stdio.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <errno.h>//随时给errno赋值
#include <error.h>//
#include <string.h>//
#include <stdlib.h>//
#include "ename.c.inc"
#ifndef STR_SIZE
#define STR_SIZE 50
#endif

void outErr(const char *msg,const char *format,int errNo,int exitNo);

void main(int argc,char *argv[]){
    int inputFd;
    struct iovec iov[3];

    //3个缓冲区
    struct stat myStruct;
    int x;
    char str[STR_SIZE];
    ssize_t numRead,totalRequired;
    
    if(argc !=2||strcmp(argv[1],"--help")==0){
        outErr(argv[0],"%s: %s\n",errno,0);
    }
    inputFd = open(argv[1],O_RDONLY);
    if(inputFd==-1){
        outErr(argv[1],"%s: %s\n",errno,0);
    }
    
    totalRequired =0;

    iov[0].iov_base=&myStruct;
    iov[0].iov_len=sizeof(struct stat);
    totalRequired+=iov[0].iov_len;

    iov[1].iov_base=&x;
    iov[1].iov_len=sizeof(int);
    totalRequired+=iov[1].iov_len;

    iov[2].iov_base=str;
    iov[2].iov_len=STR_SIZE;
    totalRequired+=iov[2].iov_len;

    numRead = readv(inputFd,iov,3);

    if(numRead==-1){
        outErr(argv[1],"%s: %s\n",errno,0);
    }
    if(numRead<totalRequired){
        printf("read fewer\n");
    }

    printf("total bytes requested: %ld; bytes read: %ld \n",(long)totalRequired,(long)numRead);
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
