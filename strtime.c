#define _XOPEN_SOURCE //避免strptime 警告
#include <stdio.h>
#include <time.h>
#include <locale.h>
#define SBUF_SIZE 1000
#include <stdlib.h>//EXIT_SUCCESS
#include <error.h>//
#include <errno.h>//errno
#include "ename.c.inc"
#include <string.h>//memset
void outErr(const char *msg,const char *format,int errNo);
void main(int argc,char *argv[])
{   
    struct tm tm;
    char sbuf[SBUF_SIZE];
    char *ofmt;
    if(argc<3||strcmp(argv[1],"--help")==0)
        outErr("use input-data-time in-fmt [out-fmt]\n","%s",errno);
    //设置地域化信息
    if(setlocale(LC_ALL,"")==NULL)
        outErr("setlocale\n","%s",errno);
    memset(&tm,0,sizeof(struct tm));
    if(strptime(argv[1],argv[2],&tm)==NULL)
        outErr("strptime faied\n","%s",0);
    tm.tm_isdst = -1;
    printf(" this input timestamp is %ld\n",(long) mktime(&tm));
    
    ofmt = (argc>3)? argv[3]:"%H:%M:%S %A,%d %B %Y %Z";
    if(strftime(sbuf,SBUF_SIZE,ofmt,&tm)==0)
        outErr("strftime\n","%s",errno);
    printf(" strftime output str is %s\n",sbuf);
    exit(EXIT_SUCCESS);

}


void outErr(const char *msg,const char *format,int errNo)
{
    fflush(stdout); 
    errNo=errno;
    printf(format,msg,strerror(errNo));
    printf(format,msg,ename[errNo]);
    perror(msg);
    fflush(stderr); 
    exit(0);
}

/**

./strtime "5:37:59pm 29 Oct 2017" "%I:%M:%S%p %d %b %Y"  一定要注意你输入的字符（argv[1]）要跟格式argv[2]严格匹配 
 this input timestamp is 1509269879
 strftime output str is 17:37:59 星期日,29 十月 2017 CST

./strtime "17:37:59 29 OCT 2017" "%H:%M:%S %d %b %Y"
 this input timestamp is 1509269879
 strftime output str is 17:37:59 星期日,29 十月 2017 CST

*/