#include <stdio.h>
#include <errno.h>//随时给errno赋值
#include <error.h>//
#include "ename.c.inc"
#include <stdlib.h>//EXIT_SUCCESS
#include <unistd.h>//警告：隐式声明与内建函数‘_exit’不兼容
#include <sys/wait.h>
void outErr(const char *msg,const char *format,int errNo);

void main(int argc,char *argv[]){
    
    int numChildren,j;
    pid_t childPid,pw;
    if(argc>1 && strcmp(argv[1],"--help")==0)
    {
        printf("usage %s numChildren...\n",argv[0]);
        exit(EXIT_SUCCESS);  
    }
    numChildren = (argc>1)?atoi(argv[1]):1;
    setbuf(stdout,NULL);
    for(j=0;j<numChildren;j++)
    {   
        printf("pid is %d\n",getpid());
        switch(childPid=fork()){
            case -1:
                outErr(argv[0],"%s",errno);
            case 0:
                printf("%d child pid is %d\n",j,getpid());
                sleep(5);
                //子进程干活了
                _exit(EXIT_SUCCESS);
            default:
                printf("%d parent pid is %d\n",j,getpid());
                pw=wait(NULL);//我们把wait函数参数设置为null的时候，就可以把这个僵尸进程销毁
                //wait等待子进程结束
                printf("pw is %d\n",pw);
                break;
        }
    }
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
/**


*/