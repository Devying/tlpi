#define _GNU_SOURCE
#include <stdio.h>
#include <errno.h>//随时给errno赋值
#include <error.h>//
#include "ename.c.inc"
#include <signal.h>
#include <stdlib.h>//EXIT_FAILURE

void outErr(const char *msg,const char *format,int errNo);

int printSigMask(FILE *of, const char *msg);

int printPendingSigs(FILE *of, const char *msg);

void printSigset(FILE *of, const char *ldr, const sigset_t *mask);

static int sigCnt[NSIG];  //NSIG  信号的总的数量我这里是65           
static volatile sig_atomic_t gotSigint = 0;
//sig_atomic_t 不希望被中断

static void
handler(int sig)
{
    if (sig == SIGINT)
        gotSigint = 1;
    else
        sigCnt[sig]++;
}

int
main(int argc, char *argv[])
{
    
    int n, numSecs=35;
    sigset_t pendingMask, blockingMask, emptyMask;

    printf("%s: PID is %ld\n", argv[0], (long) getpid());


    for (n = 1; n < NSIG; n++)          /* Same handler for all signals */
        (void) signal(n, handler); 
    
    
    //初始化所有信号的信号集
    sigfillset(&blockingMask);
    //信号集赋给信号掩码中
    if (sigprocmask(SIG_SETMASK, &blockingMask, NULL) == -1)
        outErr("sigprocmask","%s",errno);

    printf("%s: sleeping for %d seconds\n", argv[0], numSecs);
    //睡眠5秒钟
    sleep(numSecs);

    //处理pengding的信号取出来
    if (sigpending(&pendingMask) == -1)
        outErr("sigpending","%s",errno);

    printf("%s: pending signals are: \n", argv[0]);
    printSigset(stdout, "\t\t", &pendingMask);

    //初始化空的信号集
    sigemptyset(&emptyMask);
    if (sigprocmask(SIG_SETMASK, &emptyMask, NULL) == -1)
        outErr("sigprocmask","%s",errno);
    
    // 只要没收到SIGINT就一直轮训  所以必须用send 发送一个2（SIGINT）信号
    while (!gotSigint)
        continue;

    for (n = 1; n < NSIG; n++)
        if (sigCnt[n] != 0)
            printf("%s: signal %d caught %d time%s\n", argv[0], n,
                    sigCnt[n], (sigCnt[n] == 1) ? "" : "s");

    exit(EXIT_SUCCESS);
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


void                    /* Print list of signals within a signal set */
printSigset(FILE *of, const char *prefix, const sigset_t *sigset)
{
    int sig, cnt;

    cnt = 0;
    for (sig = 1; sig < NSIG; sig++) {
        if (sigismember(sigset, sig)) {
            cnt++;
            //一定要注意strsignal 并不是兼容所有系统的，我的centos就报错了
            fprintf(of, "%s%d (%s)\n", prefix, sig, strsignal(sig));
            //fprintf(of, "%s%d\n", prefix, sig);
        }
    }

    if (cnt == 0)
        fprintf(of, "%s<empty signal set>\n", prefix);
}

int                     /* Print mask of blocked signals for this process */
printSigMask(FILE *of, const char *msg)
{
    sigset_t currMask;

    if (msg != NULL)
        fprintf(of, "%s", msg);

    if (sigprocmask(SIG_BLOCK, NULL, &currMask) == -1)
        return -1;

    printSigset(of, "\t\t", &currMask);

    return 0;
}

int                     /* Print signals currently pending for this process */
printPendingSigs(FILE *of, const char *msg)
{
    sigset_t pendingSigs;

    if (msg != NULL)
        fprintf(of, "%s", msg);

    if (sigpending(&pendingSigs) == -1)
        return -1;

    printSigset(of, "\t\t", &pendingSigs);

    return 0;
}