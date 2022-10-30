#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
using namespace std;

int main()
{
    pid_t pid, pid1;
    pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "fork fail");
        return 1;
    }
    else if (pid == 0)
    {
        pid1 = getpid();
        printf("Hello\n");
        printf("fork_return=%d\n", pid);    //A
        

        execlp("/usr/bin/google-chrome", "google-chrome", "www.baidu.com", NULL); 
        system ("ls -al");       
        printf("pid=%d \n", pid1);  //B
        printf("child's parentpid=%d ", getppid()); //获取父进程的pid
    }
    else
    {
        printf("child:pid=%d \n", pid);   //C
        
        while(1) {sleep(2);}
        wait(NULL);
    }
    return 0;
}