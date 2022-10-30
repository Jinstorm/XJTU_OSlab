#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#define NUM_THREAD 2

/* NUM_THREAD个线程竞争访问其进程的公共资源sum，利用pthread_mutex_t提供互斥访问
编译： gcc -o mutex -pthread mutex.c
运行： ./mutex arg1
arg1表示每个线程要累加到的数字

*/

pthread_mutex_t *m;//指向互斥量变量的指针
int sum = 0;

void *runner(void *param);

int main(int argc, char *argv[]){

    m = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));//为互斥量变量分配内存空间
    pthread_mutex_init(m, NULL);//初始化互斥量

    pthread_t tid[NUM_THREAD];
    pthread_attr_t attr[NUM_THREAD];

    if(argc != 2){
        fprintf(stderr, "usage: a.out <interger value>\n");
        return -1;
    }
    if(atoi(argv[1]) < 0){
        fprintf(stderr, "%d must be >= 0\n", atoi(argv[1]));
        return -1;
    }
    for (int i = 0; i < NUM_THREAD; i++){
        pthread_attr_init(&attr[i]);
        pthread_create(&tid[i], &attr[i], runner, argv[1]);
    }

    for (int i = 0; i < NUM_THREAD; i++){
        pthread_join(tid[i], NULL); 
    }
    printf("sum = %d\n", sum);
    
}

void *runner(void *param){
    int i, upper = atoi(param);
    //sum = 0;

    for(i = 1; i <= upper; i++){
        pthread_mutex_lock(m);//上锁
        sum ++;
        printf("threadid = %lu, sum = %d\n", pthread_self(), sum);
        pthread_mutex_unlock(m);//解锁
        sleep(1);
    }
    
    pthread_exit(0);
}
