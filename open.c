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
    int inputFd,outputFd,openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];
    //打开文件并复制
    if(argc !=3||strcmp(argv[1],"--help")==0){
        printf("error num is %d",errno);
        outErr("Usage error","%s: %s\n",errno,0);//注意了这个错误并不会打印出来，因为没有系统调用
    }
    inputFd = open(argv[1],O_RDONLY);
    if(inputFd==-1){
        outErr(argv[1],"%s: %s\n",errno,0);
    }
    openFlags = O_CREAT|O_WRONLY|O_TRUNC;//O_EXCL 文件存在就不会打开
    filePerms = S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH;//read user write user read group write group read other write other//rw-rw-rw权限
    //打开第二个要写的文件
    outputFd = open(argv[2],openFlags,filePerms);
    if(outputFd==-1){
        outErr(argv[2],"%s: %s\n",errno,0);
    }
    //写数据
    while((numRead=read(inputFd,buf,BUF_SIZE))>0){
        //不妨报告下位置
        //printf("inputFd location is %p\n",lseek(inputFd,0,SEEK_CUR));
        if(write(outputFd,buf,numRead)!=numRead){
            outErr(argv[2],"%s: %s\n",errno,0);
        }
    }


    if(numRead==-1){
        outErr(argv[1],"%s: %s\n",errno,0);
    }
    
    if(close(inputFd)==-1){
        outErr(argv[1],"%s: %s\n",errno,0);
    }
    if(close(outputFd)==-1){
        outErr(argv[2],"%s: %s\n",errno,0);
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
