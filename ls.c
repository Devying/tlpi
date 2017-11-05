#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>

#include <unistd.h>
void main(int argc,char *argv[] ){
    DIR *dp;
    struct dirent *dirp;
    if(argc!=2){
        puts("usage:ls dir");
        exit(1);
    }
    if((dp=opendir(argv[1]))==NULL){
        puts("dir not exitst");
        exit(1);
    }
    while((dirp=readdir(dp))!=NULL)
        printf("%s\n",dirp->d_name);
    closedir(dp);
    printf("pid is %ld\n",(long)getpid());
    exit(0);
}