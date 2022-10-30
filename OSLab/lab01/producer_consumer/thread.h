#ifndef THREAD_H
#define THREAD_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "mutex.h"
#include "buffer.h"

void *producer(void *param) { //生产者进程
    buffer_item item;
    srand(time(NULL));

    while (1) {
        sleep(rand() % 5); // 返回0～5之间的随机数
        item = rand() % 100; // 返回0～100之间的随机数

        /* Entry section */
        sem_wait(&empty); // 等待空位(原子操作)
        pthread_mutex_lock(&mutex); // 互斥锁

        /* Critical section */
        if (insert_item(item)) {
            printf("producer insert item error, continue to next loop\n");
            pthread_mutex_unlock(&mutex);
            sem_post(&empty);
            continue;
        }
        printf("%lu producer insert item %2d success. Current buffer is:", pthread_self(), item);
        printBuffer();
        
        /* Exit section */
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

void *consumer(void *param) {
    buffer_item item;
    
    while (1) {
        sleep(rand() % 5);

        /* Entry section */
        sem_wait(&full);
        pthread_mutex_lock(&mutex); // 先看是否有空位再申请锁

        /* Critical section */
        if (remove_item(&item)) {
            //error message
            printf("consumer remove item error, continue to next loop\n");
            pthread_mutex_unlock(&mutex);
            sem_post(&full);
            continue;
        }
        printf("%lu consumer remove item %2d success. Current buffer is:", pthread_self(), item);
        printBuffer();
        
        /* Exit section */
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

#endif