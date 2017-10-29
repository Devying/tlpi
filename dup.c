#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>//随时给errno赋值
#include <error.h>//
#include <string.h>//
#include <stdlib.h>//
#include "ename.c.inc"
#ifndef BUF_SIZE
#define BUF_SIZE 50
#endif

void outErr(const char *msg,const char *format,int errNo,int exitNo);

void main(int argc,char *argv[]){
    int inputFd,newFd;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE]="abcdefghijklmnopqrst\n";
    //打开文件并复制
    if(argc !=2||strcmp(argv[1],"--help")==0){
        printf("error num is %d",errno);
        outErr("Usage error","%s: %s\n",errno,0);//注意了这个错误并不会打印出来，因为没有系统调用
    }
    inputFd = open(argv[1],O_RDONLY);
    if(inputFd==-1){
        outErr(argv[1],"%s: %s\n",errno,0);
    }
    //newFd = dup(1);//0标准输入、1标准输出、2标准错误
    newFd = dup(1);
    if(write(newFd,buf,BUF_SIZE)==-1){
        outErr(argv[1],"%s: %s\n",errno,0);
    }
    
    if(close(inputFd)==-1){
        outErr(argv[1],"%s: %s\n",errno,0);
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
