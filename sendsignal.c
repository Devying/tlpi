#include <stdio.h>
#include <errno.h>//随时给errno赋值
#include <error.h>//
#include "ename.c.inc"
#include <signal.h>
#include <stdlib.h>//EXIT_FAILURE

void outErr(const char *msg,const char *format,int errNo);


int
main(int argc, char *argv[])
{
    int numSigs, sig, j;
    pid_t pid;

    if (argc < 4 || strcmp(argv[1], "--help") == 0){
        printf("%s pid num-sigs sig-num [sig-num-2]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    pid = atoi(argv[1]);
    numSigs = atoi(argv[2]);
    sig = atoi(argv[3]);

    printf("%s: sending signal %d to process %ld %d times\n",
            argv[0], sig, (long) pid, numSigs);

    for (j = 0; j < numSigs; j++)
        if (kill(pid, sig) == -1)
            outErr("kill","%s",errno);

    /* If a fourth command-line argument was specified, send that signal */

    if (argc > 4)
        if (kill(pid, atoi(argv[4])) == -1)
            outErr("kill","%s",errno);

    printf("%s: exiting\n", argv[0]);
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

