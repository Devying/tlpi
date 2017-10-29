#include <stdio.h>
#include <stdlib.h>
extern char **environ;

void main(int argc,char *argv[],char *envp[]){
//    char **ep;
//    for(ep = environ;*ep!=NULL;ep++)
//    {
//         puts(*ep);
//    }
    int i =0;
    for(i=0;envp[i]!=NULL;i++){
        puts(envp[i]);
    }
    //设置环境变量  进程结束后环境变量消失
    if(setenv("HUANGBY","huangbaoying",0)==0){
        puts("set env success");
    }

    char **ep;
    for(ep = environ;*ep!=NULL;ep++)
    {
        puts(*ep);
    }

}
