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
    
    int numDead;       /* Number of children so far waited for */
    pid_t childPid;    /* PID of waited for child */
    int j;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        outErr("usage wait --help ","%s",errno);

    setbuf(stdout, NULL);           /* Disable buffering of stdout */

    for (j = 1; j < argc; j++) {    /* Create one child for each argument */
        switch (fork()) {
        case -1:
            outErr("fork","%s",errno);

        case 0:                     /* Child sleeps for a while then exits */
            printf("[%s] child %d started with PID %ld, sleeping %s "
                    "seconds\n", curTime(), j, (long) getpid(),
                    argv[j]);
            sleep(atoi(argv[j]));
            _exit(EXIT_SUCCESS);

        default:                    /* Parent just continues around loop */
            break;
        }
    }

    numDead = 0;
    for (;;) {                      /* Parent waits for each child to exit */
        childPid = wait(NULL);
        if (childPid == -1) {
            if (errno == ECHILD) {
                printf("No more children - bye!\n");
                exit(EXIT_SUCCESS);
            } else {                /* Some other (unexpected) error */
                outErr("wait","%s",errno);
            }
        }

        numDead++;
        printf("[%s] wait() returned child PID %ld (numDead=%d)\n",
                curTime(), (long) childPid, numDead);
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