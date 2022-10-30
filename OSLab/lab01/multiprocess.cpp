// #include <sys/types.h>
// #include <stdio.h>
// #include <unistd.h>
// #include <sys/wait.h>
// #include <iostream>
// using namespace std;
// int main(){
//     pid_t pid, pid1;
    
//     pid = fork();
//     if (pid < 0){
//         fprintf(stderr, "fork fail");
//         return 1;
//     }else if (pid == 0){
//         pid1 = getpid();
//         printf("fork_return1=%d ", pid);    //A
//         printf("child's pid=%d ", pid1);  //B
//         printf("child's parentpid=%d \n", getppid()); //获取父进程的pid
//     }else{
//         pid1 = getpid();
//         printf("fork_return2=%d ", pid);   //C
//         printf("parent's pid=%d \n", pid1); //D
//         wait(NULL);
//         printf("foobar"); 
//     }
//     return 0;
// }
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
using namespace std;

static int var = 5;
int main()
{
    pid_t pid, pid1;
    printf("printf ");
    pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "fork fail");
        return 1;
    }
    else if (pid == 0)
    {
        // sleep(1);
        //var--;
        pid1 = getpid();
        printf("fork_return1=%d \n", pid);    //A
        printf("child's pid=%d \n", pid1);  //B
        // //execlp("/usr/bin/firefox", "firefox", "www.baidu.com", NULL);
        // //printf("var = %d ", var);
        // //printf("addr = %p ", &var);
        // printf("child's parentpid=%d ", getppid()); //获取父进程的pid
    }
    else
    {
        //var++;
        //sleep(1);
        pid1 = getpid();
        printf("fork_return2=%d \n", pid);   //C
        printf("parent's pid=%d \n", pid1); //D
        //printf("var = %d ", var);
        //printf("addr = %p ", &var);
        wait(NULL);
        printf("foobar"); 
    }
    // var++;
    // printf("var = %d\n", var);
    return 0;
}