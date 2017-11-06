#include <stdio.h>
#include <errno.h>//随时给errno赋值
#include <error.h>//
#include <stdlib.h>//EXIT_SUCCESS

#include <sys/inotify.h>
#include <limits.h> //NAME_MAX
#define BUF_LEN (10 * sizeof(struct inotify_event) + NAME_MAX+1 )

void outErr(const char *msg,const char *format,int errNo);
void main(int argc,char *argv[]){
    int inotifyFd,wd,j;
    char buf[BUF_LEN];
    ssize_t numRead;
    char *p;
    struct inotify_event *event;
    if(argc<2||strcmp(argv[1],"--help")==0)
    {
        printf("usage %s pathname...\n",argv[0]);
        exit(EXIT_SUCCESS);  
    }
    inotifyFd = inotify_init();
    if(inotifyFd==-1)
        outErr("inotify_init","%s",errno);
    for(j=1;j<argc;j++){
        wd = inotify_add_watch(inotifyFd,argv[j],IN_ALL_EVENTS);
        if(wd == -1)
            outErr("inotify_add_watch","%s",errno);
        printf("added item  path:%s using wd:%d",argv[j],wd);
    }
    for(;;){
        //读取文件描述符
        numRead = read(inotifyFd,buf,BUF_LEN);
        if(numRead==0)
            printf("read inotifyFd %d return 0",inotifyFd);
        if(numRead==-1)
            outErr("read inotifyFd failed ","%s",errno);
        printf("read %ld bytes from inotify fd\n",(long) numRead);

        //buf中有一个或者多个 inotify_event 结构体
        for(p = buf;p<buf+numRead;){
            // p在buf上移动，属于指向char类型的指针,
            //每次移动后 地址 正好对应的inotify_event 地址
            //所以这里将 字符类型的指针强制转换指向 inotify_event的指针
            event = (struct inotify_event *)p;
            // wd 是监控描述符
            printf(" wd = %2d; ",event->wd);
            // cookie 用来表示相关的两个事件
            if(event->cookie>0)
                printf("cookie = %4d; ",event->cookie);
            printf("action is ");
            if (event->mask & IN_ACCESS)         printf("IN_ACCESS");
            if (event->mask & IN_ATTRIB)        printf("IN_ATTRIB ");
            if (event->mask & IN_CLOSE_NOWRITE) printf("IN_CLOSE_NOWRITE ");
            if (event->mask & IN_CLOSE_WRITE)   printf("IN_CLOSE_WRITE ");
            if (event->mask & IN_CREATE)        printf("IN_CREATE ");
            if (event->mask & IN_DELETE)        printf("IN_DELETE ");
            if (event->mask & IN_DELETE_SELF)   printf("IN_DELETE_SELF ");
            if (event->mask & IN_IGNORED)       printf("IN_IGNORED ");
            if (event->mask & IN_ISDIR)         printf("IN_ISDIR ");
            if (event->mask & IN_MODIFY)        printf("IN_MODIFY ");
            if (event->mask & IN_MOVE_SELF)     printf("IN_MOVE_SELF ");
            if (event->mask & IN_MOVED_FROM)    printf("IN_MOVED_FROM ");
            if (event->mask & IN_MOVED_TO)      printf("IN_MOVED_TO ");
            if (event->mask & IN_OPEN)          printf("IN_OPEN ");
            if (event->mask & IN_Q_OVERFLOW)    printf("IN_Q_OVERFLOW ");
            if (event->mask & IN_UNMOUNT)       printf("IN_UNMOUNT ");
            printf("\n");
            if (event->len > 0)
                printf("name = %s\n", event->name);

            //
            p+=sizeof(struct inotify_event)+event->len;
        }
    }


    exit(EXIT_SUCCESS);

}
void outErr(const char *msg,const char *format,int errNo){
    fflush(stdout); 
    errNo=errno;
    printf(format,msg,strerror(errNo));
    //printf(format,msg,ename[errNo]);
    perror(msg);
    fflush(stderr); 
    exit(0);
}

/**
./inotify /home/www
*/