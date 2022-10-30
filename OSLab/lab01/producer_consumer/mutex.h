/*mutex.h*/
#ifndef MUTEX_H
#define MUTEX_H
#include <pthread.h>
#include <semaphore.h>

/*empty（记录多少个空位），full（记录有多少满位）以及mutex（二进制信号量以保护对缓冲的插入与删除操作）*/
pthread_mutex_t mutex; // 互斥
sem_t empty, full; // 信号量


#endif // MUTEX_H