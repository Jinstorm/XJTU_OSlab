#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#define NUM_THREAD 10

int sum = 0;
void *runner(void *param);

int main(int argc, char *argv[]){
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
    for (int i = 0; i < 10; i++){
        pthread_attr_init(&attr[i]);
        pthread_create(&tid[i], &attr[i], runner, argv[1]);
    }

    for (int i = 0; i < 10; i++){

        pthread_join(tid[i], NULL);
        printf("sum = %d\n", sum);
    }
    
}

void *runner(void *param){
    int i, upper = atoi(param);
    //sum = 0;
    for(i = 1; i <= upper; i++)
        sum += i;
    pthread_exit(0);
}


/*
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#define NUM_THREAD 10

int sum;
void *runner(void *param);

int main(int argc, char *argv[]){
    pthread_t tid;
    pthread_attr_t attr;

    if(argc != 2){
        fprintf(stderr, "usage: a.out <interger value>\n");
        return -1;
    }
    if(atoi(argv[1]) < 0){
        fprintf(stderr, "%d must be >= 0\n", atoi(argv[1]));
        return -1;
    }

    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, runner, argv[1]);
    pthread_join(tid, NULL);
    printf("sum = %d\n", sum);
}

void *runner(void *param){
    int i, upper = atoi(param);
    sum = 0;
    for(i = 1; i <= upper; i++)
        sum += i;
    pthread_exit(0);
}
*/