#include <stdio.h>

#define _GNU_SOURCE
#include <errno.h>


// #include <limits.h>
#include <unistd.h>

void main(void)
{
    extern char *program_invocation_name;
    extern char *program_invocation_short_name;
    printf("this program name is %s\n",program_invocation_name);
    printf("this program short name is %s\n",program_invocation_short_name);
    long arg_max = sysconf(_SC_ARG_MAX);//#include <unistd.h>
    printf("this arg max is %ld\n",arg_max);
}