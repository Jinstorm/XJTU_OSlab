/*buffer.h*/
#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>
/*声明了缓冲区数据类型，缓冲大小，以及对缓冲区可采取的操作。*/
typedef int buffer_item;
#define BUFFER_SIZE 5

int insert_item(buffer_item);
int remove_item(buffer_item *);
void printBuffer();

static buffer_item buffer[BUFFER_SIZE]; //缓冲区
static int counter, in, out;

int insert_item(buffer_item item) {
    if (counter == BUFFER_SIZE) {
        return -1;
    } else {
        buffer[in] = item;
        in  = (in + 1) % BUFFER_SIZE;
        counter++; //缓冲区数量-1
        return 0;
    }
}

int remove_item(buffer_item *item) {
    if (counter == 0) {
        return -1;
    } else {
        *item = buffer[out]; //取数据
        buffer[out] = 0;
        out = (out + 1) % BUFFER_SIZE; //
        counter--;
        return 0;
    }
}

void printBuffer() {
    int i = 0;
    for (i = 0; i < BUFFER_SIZE; i++) {
        printf("%2d ", buffer[i]);
    }
    printf("end\n");
}

#endif