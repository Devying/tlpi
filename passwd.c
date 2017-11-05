#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <pwd.h>
#include <shadow.h>
typedef enum { FALSE, TRUE } Boolean;

#include <stdlib.h>
#include <string.h>
#include <crypt.h>  //passwd.c:58: 警告：赋值时将整数赋给指针，未作类型转换

int main(int argc,char * argv[])
{
    char *username,*password,*encrypted,*p;
    struct passwd *pwd;
    struct spwd *spwd;
    Boolean authOk;
    size_t len;
    long lnmax;
    lnmax = sysconf(_SC_LOGIN_NAME_MAX);
    //printf("_SC_LOGIN_NAME_MAX=%ld\n",lnmax);
    if(lnmax==-1)
        lnmax = 256;
    username = malloc(lnmax);
    if(username == NULL){
        exit(EXIT_FAILURE);
    }
    printf("Username:");
    fflush(stdout);
    if(fgets(username,lnmax,stdin)==NULL)
    {
        exit(EXIT_FAILURE);
    }
    //printf("username is %s\n",username);
    len = strlen(username);
    if(username[len-1]=='\n')
        username[len-1]='\0';
    pwd = getpwnam(username);
    if(pwd == NULL)
    {
        printf("can't get password record");
        exit(EXIT_FAILURE);
    }
    spwd = getspnam(username);
    if(spwd == NULL&&errno == EACCES)
    {
        printf("no permission to reade");
        exit(EXIT_FAILURE);
    }

    if(spwd != NULL){
        pwd->pw_passwd = spwd->sp_pwdp;
    }


    password=getpass("Password:");

    encrypted = crypt(password,pwd->pw_passwd);
    //printf("password is %s",password);

    //销毁密码
    for(p = password;*p!='\0';)
        *p++= '\0';
    if(encrypted == NULL)
    {
        printf("can't get shadow record");
        exit(EXIT_FAILURE);
    }

    authOk = strcmp(encrypted,pwd->pw_passwd)==0;
    if(!authOk)
    {
        printf("failed");
        exit(EXIT_FAILURE);
    }

    printf("ok");
    return 0;

}
/*

编译要加 -lcrypt

gcc -o passwd passwd.c -lcrypt

*/