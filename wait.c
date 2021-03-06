#include <stdio.h>
#include <errno.h>//随时给errno赋值
#include <error.h>//
#include "ename.c.inc"
#include <stdlib.h>//EXIT_SUCCESS
#include <unistd.h>//警告：隐式声明与内建函数‘_exit’不兼容
#include <sys/wait.h>


#include <time.h>
#define SBUF_SIZE 1000
#include <string.h>//memset

void outErr(const char *msg,const char *format,int errNo);
char * curTime(void);
void main(int argc,char *argv[]){
    
    int numChildren,j,numDead;
    pid_t childPid,pw;
    if(argc>1 && strcmp(argv[1],"--help")==0)
    {
        printf("usage %s numChildren...\n",argv[0]);
        exit(EXIT_SUCCESS);  
    }
    numChildren = (argc>1)?atoi(argv[1]):1;
    setbuf(stdout,NULL);

    for(j=1;j<numChildren;j++){   
        switch(fork()){
        case -1:
            outErr(argv[0],"%s",errno);
        case 0:
            printf("%s child %d  started - pid is %d - sleep %d seconds\n",curTime(),j,(long)getpid(),j+3);
            sleep(j-1);
                //子进程干活了
            _exit(EXIT_SUCCESS);
        default:
            break;
        }
    }

    numDead = 0;
    for(;;){
        childPid = wait(NULL);
        if(childPid == -1){
            if(errno == ECHILD){ 
                printf("No more--bye");
                exit(EXIT_SUCCESS);
            }else{
                outErr("wait","%s",errno);
            }
        }
        numDead++;
        printf("%s wait() returned child PID %d  numDead is %d\n",curTime(),getpid(),numDead);
    }

}
char * curTime(void){
    struct timeval timev;
    static char sbuf[SBUF_SIZE];
    char mi[10];
    time_t t;
    struct tm *tm;
    t=time(NULL);
    tm=localtime(&t);
    

    if(tm == NULL){
        return NULL;
    }

    if(strftime(sbuf,SBUF_SIZE,"%H:%M:%S",tm)==0){
        return NULL;
        //outErr("strftime\n","%s",errno);
    }

    if(gettimeofday(&timev,NULL)==-1)
        return NULL;
    sprintf(mi,"%lld",(long)timev.tv_usec);
    strcat (sbuf,".");
    strcat (sbuf,mi);
    return sbuf;
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