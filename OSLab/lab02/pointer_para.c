#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

int x = 5;

void func(int *t){
    *t = *t + 1;
    printf("%d\n", *t);
    t = &x;
    printf("%d\n", *t);
}
int main(){
    int a = 2;
    int *p = &a;
    func(p);
    printf("%d\n", *p);
    return 0;
}