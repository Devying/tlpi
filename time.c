#include <stdio.h>
#include <errno.h>//随时给errno赋值
#include <error.h>//
#include "ename.c.inc"
#include <locale.h>
#include <time.h>
#include <sys/time.h>

#include <stdlib.h>//EXIT_SUCCESS
#define SECONDS_IN_TROPICAL_YEAR (365.24219 * 24 * 60 *60)
#define CCT (+8) //东八区

void outErr(const char *msg,const char *format,int errNo);

void main(int argc,char *argv[]){
    time_t t;
    struct tm *gmp,*locp; //带指针的
    struct tm gm,loc;
    struct timeval tv;
    time(&t);
    printf("this rawtime / timestamp is %ld\n",(long)t);//打印时间戳
    printf("about %6.4f years \n",t/SECONDS_IN_TROPICAL_YEAR);//计算大概多少年

    if(gettimeofday(&tv,NULL)==-1)
        outErr("gettimeofday","%s",errno);
    printf("gettimeofday return second is %ld micro second is %lld\n",(long)tv.tv_sec,(long)tv.tv_usec);//返回时间戳带微妙的 类似PHP的microtime(true)

    gmp = gmtime(&t);
    gm = *gmp;
    //年是自1970年以来多少年
    //月0-11
    //日1-31
    printf("gmptime back year = %d mon=%d mday=%d hour=%d min=%d sec=%d \n",gm.tm_year,gm.tm_mon,gm.tm_mday,gm.tm_hour,gm.tm_min,gm.tm_sec);
    
    //本地时间
    locp = localtime(&t);
    if(locp == NULL)
        outErr("localtime","%s",errno);
    loc = *locp;
    printf("localtime back year = %d mon=%d mday=%d hour=%d min=%d sec=%d \n",loc.tm_year,loc.tm_mon,loc.tm_mday,loc.tm_hour,loc.tm_min,loc.tm_sec);

    //
    printf("asctime() formats the gmtime() value as %s\n",asctime(&gm));
    printf("ctime() formats the time() value as %s\n",ctime(&t));

    printf("mktime() formats the gmtime() value is %ld secs\n",mktime(&gm));
    printf("mktime() formats the localtime() value is %ld secs\n",mktime(&loc));


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
this rawtime / timestamp is 1509208154
about 47.8249 years 
gettimeofday return second is 1509208154 micro second is 345486
gmptime back year = 117 mon=9 mday=28 hour=16 min=29 sec=14 
localtime back year = 117 mon=9 mday=29 hour=0 min=29 sec=14 
asctime() formats the gmtime() value as Sat Oct 28 16:29:14 2017

ctime() formats the time() value as Sun Oct 29 00:29:14 2017

mktime() formats the gmtime() value is 1509179354 secs
mktime() formats the localtime() value is 1509208154 secs

*/