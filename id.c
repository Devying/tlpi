#include <stdio.h>
// #include <sys/stat.h>
// #include <sys/uio.h>
// #include <fcntl.h>
#include <errno.h>//随时给errno赋值
#include <error.h>//
// #include <string.h>//
// #include <stdlib.h>//
#include "ename.c.inc"
#include <unistd.h>
#include <sys/fsuid.h>
#include <limits.h>

#define SG_SIZE (NGROUPS_MAX + 1)

void outErr(const char *msg,const char *format,int errNo);

void main(int argc,char *argv[]){
    uid_t ruid,euid,suid,fsuid;
    gid_t rgid,egid,sgid,fsgid;
    gid_t suppGroups[SG_SIZE];

    int numGroups,j;
    char *p;
    if(getresuid(&ruid,&euid,&suid)==-1){
        outErr("getresuid","%s: %s\n",errno);
    }

    if(getresgid(&rgid,&egid,&sgid)==-1){
        outErr("getresgid","%s: %s\n",errno);
    }
    fsuid = 

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
