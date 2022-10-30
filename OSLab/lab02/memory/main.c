#include <stdio.h> 
#include <malloc.h>
#include <unistd.h>     
#include <stdlib.h>
#include <sys/wait.h>                                                                                                  
#include <signal.h>                                                                                                       

#define PROCESS_NAME_LEN 32   /*进程名长度*/
#define MIN_SLICE    10             /*最小碎片的大小*/
#define DEFAULT_MEM_SIZE 1024     /*内存大小*/
#define DEFAULT_MEM_START 0       /*起始位置*/
/* 内存分配算法 */
#define MA_FF 1
#define MA_BF 2
#define MA_WF 3
int mem_size=DEFAULT_MEM_SIZE; /*内存大小*/
int current_free_mem_size= 0;     //当前空闲内存大小
int ma_algorithm = MA_FF;           /*当前分配算法*/
static int pid = 0;                                      /*初始pid*/
int flag = 0;                             /*设置内存大小标志*/

/*描述每一个空闲块的数据结构*/
struct free_block_type{
    int size;   //当前空闲块的大小
    int start_addr; //起始地址
    struct free_block_type *next;   //指向下一个空闲块的结构体指针
}; 
/*指向内存中空闲块链表的首指针*/
struct free_block_type *free_block;


/*每个进程分配到的内存块的描述*/
struct allocated_block{
    int pid;    // 进程id
    int size;   //分配的内存块的大小
    int start_addr; //起始地址
    char process_name[PROCESS_NAME_LEN];    // 进程名称
    struct allocated_block *next;   //指向下一个分配出去的内存块
    };
/*进程分配内存块链表的首指针*/
struct allocated_block *allocated_block_head = NULL;


// 函数声明
void display_menu();
int set_mem_size();

void set_algorithm();
void free_memory_rearrage(int memory_leftsize, int allocated_size);
void rearrange(int algorithm);
void rearrange_FF();
void rearrange_BF();
void rearrange_WF();

int new_process();
int allocate_mem(struct allocated_block *ab);

void kill_process();
int free_mem(struct allocated_block *ab);
int dispose(struct allocated_block *free_ab);

int display_mem_usage();
struct allocated_block *find_process(int pid);
void do_exit();


/*初始化空闲块，默认为一块，可以指定大小及起始地址*/ // init_free_block函数
struct free_block_type* init_free_block(int mem_size){
    struct free_block_type *fb;

    fb = (struct free_block_type *)malloc(sizeof(struct free_block_type));
    if(fb == NULL){
        printf("No mem\n");
        return NULL;
        }
    fb->size = mem_size;    // mem_size=DEFAULT_MEM_SIZE
    fb->start_addr = DEFAULT_MEM_START;
    fb->next = NULL;
    return fb;
}


/*显示菜单*/
void display_menu(){
    printf("\n");
    printf("1 - Set memory size (default=%d)\n", DEFAULT_MEM_SIZE);
    printf("2 - Select memory allocation algorithm\n");
    printf("3 - New process \n");
    printf("4 - Terminate a process \n");
    printf("5 - Display memory usage \n");
    printf("0 - Exit\n");
}


/*设置内存的大小*/
int set_mem_size(){
    int size;
    if(flag!=0){  //防止重复设置
        printf("Cannot set memory size again\n");
        return 0;
    }
    printf("Total memory size =");
    scanf("%d", &size);
    if(size>0) {
        mem_size = size;
        free_block->size = mem_size;
    }
    flag = 1;  
    return 1;
}


/* 设置当前的分配算法 */
void set_algorithm(){
    int algorithm;
    printf("\t1 - First Fit\n");
    printf("\t2 - Best Fit \n");
    printf("\t3 - Worst Fit \n");

    scanf("%d", &algorithm);

    if(algorithm >=1 && algorithm <= 3) {

        ma_algorithm = algorithm;
    } 
	//按指定算法重新排列空闲区链表
    //printf("Hello shit~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    rearrange(ma_algorithm); 
}


/*按指定的算法整理内存空闲块链表*/
void rearrange(int algorithm){
    switch(algorithm){
        case MA_FF:  rearrange_FF(); break;
        case MA_BF:  rearrange_BF(); break;
        case MA_WF:  rearrange_WF(); break;
    }
}

// /*按FF算法重新整理内存空闲块链表*/
void rearrange_FF()
{
	struct free_block_type *p, *p1, *p2;
	struct free_block_type *last_flag;
	p1 = (struct free_block_type *)malloc(sizeof(struct free_block_type));
	p1->next = free_block;
    free_block = p1;
	if(free_block != NULL){
		for (last_flag = NULL; last_flag != free_block; last_flag = p)
		{
			for (p = p1 = free_block; p1->next != NULL && p1->next->next != NULL && p1->next->next != last_flag; p1 = p1->next)
			{
			   if (p1->next->start_addr > p1->next->next->start_addr) 
			   {
				p2 = p1->next->next;  
				p1->next->next = p2->next;
				p2->next = p1->next; 
				p1->next = p2; 
				
				p = p1->next->next; 
			   }
			}
		}
	}
	p1 = free_block; 
	free_block = free_block->next; 
	free(p1); 
	p1 = NULL; 
}
/*按BF算法重新整理内存空闲块链表*/
void rearrange_BF()
{
    struct free_block_type *p, *p1, *p2;
    struct free_block_type *last_flag;
    p1 = (struct free_block_type *)malloc(sizeof(struct free_block_type));
    p1->next = free_block;
    free_block = p1; 
    if (free_block != NULL)
    {
        for (last_flag = NULL; last_flag != free_block; last_flag = p)
        //每一轮循环last_flag从最后往前移动一个位置（起冒泡排序的外循环计数作用），直到last_flag移动到头部free_block处
        {
            for (p = p1 = free_block; p1->next != NULL && p1->next->next != NULL && p1->next->next != last_flag; p1 = p1->next)
            {
                if (p1->next->size > p1->next->next->size)
                {
                    p2 = p1->next->next;
                    p1->next->next = p2->next;
                    p2->next = p1->next;
                    p1->next = p2;

                    p = p1->next->next;
                }
            }
        }
    }
    p1 = free_block;
    free_block = free_block->next;
    free(p1);
    p1 = NULL;
}
/*按WF算法重新整理内存空闲块链表*/
void rearrange_WF()
{
    struct free_block_type *p, *p1, *p2;
    struct free_block_type *last_flag;
    p1 = (struct free_block_type *)malloc(sizeof(struct free_block_type));
    p1->next = free_block;
    free_block = p1;
    if (free_block != NULL)
    {
        for (last_flag = NULL; last_flag != free_block; last_flag = p)
        {
            for (p = p1 = free_block; p1->next != NULL && p1->next->next != NULL && p1->next->next != last_flag; p1 = p1->next)
            {
                if (p1->next->size < p1->next->next->size)
                {
                    p2 = p1->next->next;
                    p1->next->next = p2->next;
                    p2->next = p1->next;
                    p1->next = p2;

                    p = p1->next->next;
                }
            }
        }
    }

    p1 = free_block;
    free_block = free_block->next;
    free(p1);
    p1 = NULL;
}

/*创建新的进程，主要是获取内存的申请数量*/
int new_process(){
    struct allocated_block *ab;
    int size;    
    int ret;
    ab = (struct allocated_block *)malloc(sizeof(struct allocated_block));
    if(!ab) exit(-5);
    ab->next = NULL;
    pid++;

    sprintf(ab->process_name, "PROCESS-%02d", pid);
    ab->pid = pid;    
    printf("Memory for %s:", ab->process_name);
    scanf("%d", &size);

    if(size>0) 
        ab->size =size;
    // ################################################。。。。

    ret = allocate_mem(ab);  /* 从空闲区分配内存，ret==1表示分配ok*/

    /*如果此时allocated_block_head尚未赋值，则赋值*/
    if((ret == 1) && (allocated_block_head == NULL)){ 
        allocated_block_head = ab;
        return ret;        
    }
    /*分配成功，将该已分配块的描述插入已分配链表*/
    else if (ret == 1) {
        ab->next=allocated_block_head;
        allocated_block_head=ab;
        return ret;        
    }
    else if(ret == -1){ /*分配不成功*/
        printf("Allocation fail\n");
        free(ab);
        ret = -1;
        return ret;       
    }
    return ret;
}

/*分配内存模块*/
int allocate_mem(struct allocated_block *ab){
    struct free_block_type *fbt, *pre;
    int request_size = ab->size;
    fbt = pre = free_block; //pre指向当前空闲链头 fbt free_block_tmp
    //根据当前算法在空闲分区链表中搜索合适空闲分区进行分配，分配时注意以下情况：
    // 1. 找到可满足空闲分区且分配后剩余空间足够大，则分割
    // 2. 找到可满足空闲分区且但分配后剩余空间比较小，则一起分配
    // 3. 找不可满足需要的空闲分区但空闲分区之和能满足需要，则采用内存紧缩技术，进行空闲分区的合并，然后再分配
    // 4. 在成功分配内存后，应保持空闲分区按照相应算法有序
    // 5. 分配成功则返回1，否则返回-1
    //请自行补充。。。。。

    rearrange(ma_algorithm); // 按照相应算法整理空闲空间

    while((pre!=NULL)&&(request_size > pre->size))//遍历查找匹配空白区
	{
		//memory_count+=pre->size;
		fbt = pre;    // fbt是pre的前一个空闲块
		pre = pre->next;   
	}
    if(!pre)  //没找到：pre=pre->next结尾
	{
		if(mem_size >= request_size)/*memory_count*/ // 判断是否需要紧缩技术 mem_size表示空余的内存空间总大小
		{
			if(mem_size >= request_size + MIN_SLICE)
				free_memory_rearrage(mem_size - request_size, request_size); //采用紧缩技术
			else	
				free_memory_rearrage(0, mem_size); //采用紧缩技术,空闲空间全部分配
            //rearrange(ma_algorithm);//分配成功，按照相应算法排序
			return 0;//全部重定位,不返回
		}
		else
			return -1;//分配失败!
	}

    else  //找到了：内存能满足 request_size<=pre->size
	{
		if((pre->size - request_size) > MIN_SLICE)//找到可满足空闲分区且分配后剩余空间足够大，则分割
		{
		    // pre->size = pre->size - request_size;
		    // ab->start_addr = pre->start_addr + pre->size;
            ab->start_addr= pre->start_addr;
            pre->start_addr += ab->size;
            pre->size -= ab->size;
            printf("hello");
		}
		else//找到可满足空闲分区且但分配后剩余空间比较小，则一起分配，删除该节点
		{
			if(pre == fbt)  //如果给进程分配到了第一个空闲块
			{	
				fbt = pre->next;
				free_block = fbt; //将空闲块指定为下一个空闲块，当前空闲块因为剩余空间过小丢弃
			}
			else    //如果不是第一个空闲块
				fbt->next = pre->next;  // 更新空闲链表的指针结构
                //将ab获得的空间更新
                ab->start_addr = pre->start_addr;
			    ab->size = pre->size;
			    free(pre);//释放节点
		}
	}
	mem_size -= ab->size;//...
	//rearrange(ma_algorithm);//分配成功，按照相应算法排序
    return 1;
}


// 紧缩技术
void free_memory_rearrage(int memory_leftsize, int allocated_size) {
// memory_leftsize：给当前进程分配后剩余空闲空间大小
// allocated_size：当前进程需要的空间大小

    struct free_block_type *p1, *p2;
    struct allocated_block *a1, *a2;

    /* 整理出新的一片空闲空间 */
    if (memory_leftsize != 0) //分配完还有小块空间
    {
        p1 = free_block;
        p2 = p1->next;

        p1->start_addr = 0; 
        p1->size = memory_leftsize;
        p1->next = NULL;

        mem_size = memory_leftsize; // 更新总空闲空间mem_size为剩余的空闲小块空间
    }
    else
    {
        p2 = free_block;
        free_block = NULL;
        mem_size = 0;
    }
    
    /* 释放被紧缩处理的节点 */
    while (p2 != NULL)
    {
        p1 = p2;
        p2 = p2->next;
        free(p1);
    }
    // allocated_block 重新修改链接
    // a1为将要分配内存的进程
    a1 = (struct allocated_block *)malloc(sizeof(struct allocated_block));
    a1->pid = pid; // pid 为当前进程的pid（在new_process函数里每次++）
    a1->size = allocated_size;
    a1->start_addr = memory_leftsize; //已申请的开始地址，从memory_leftsize开始
    sprintf(a1->process_name, "PROCESS-%02d", pid);

    //整理已分配的进程链表
    a1->next = allocated_block_head;
    a2 = allocated_block_head;
    allocated_block_head = a1;

    while (a2 != NULL)
    {
        a2->start_addr = a1->start_addr + a1->size;
        a1 = a2;
        a2 = a2->next;
    }
}

struct allocated_block *find_process(int pid)
{
	struct allocated_block *p;
	p = allocated_block_head;
	while(p)
	{
	    if(p->pid==pid)
		    return p;
	    p = p->next;
	}
	return p;
}

/*删除进程，归还分配的存储空间，并删除描述该进程内存分配的节点*/
void kill_process(){
    struct allocated_block *ab;
    int pid;
    printf("Kill Process, pid=");
    scanf("%d", &pid);
    ab = find_process(pid);
    if(ab != NULL){
        free_mem(ab); /*释放ab所表示的分配区*/
        dispose(ab);  /*释放ab数据结构节点*/
    }
}


// /*将ab所表示的已分配区归还，并进行可能的合并*/
// int free_mem(struct allocated_block *ab){
//     int algorithm = ma_algorithm;
//     struct free_block_type *fbt, *pre, *work;
//     fbt = (struct free_block_type*) malloc(sizeof(struct free_block_type));
//     if(!fbt) return -1;
//     // 进行可能的合并，基本策略如下
//     // 1. 将新释放的结点插入到空闲分区队列末尾
//     // 2. 对空闲链表按照地址有序排列
//     // 3. 检查并合并相邻的空闲分区
//     // 4. 将空闲链表重新按照当前算法排序
//     //请自行补充……
//     mem_size += ab->size;
//     fbt->size = ab->size;
// 	fbt->start_addr=ab->start_addr;
// 	fbt->next=NULL;
// 	rearrange(ma_algorithm);//按地址有序排列


//     return 1;
// }
/*释放ab所表示的分配区*/
int free_mem(struct allocated_block *ab)
{
    int algorithm = ma_algorithm;
    struct free_block_type *fbt, *pre, *work;
    mem_size += ab->size;
    fbt = (struct free_block_type *)malloc(sizeof(struct free_block_type));
    if (!fbt)
        return -1;
    fbt->size = ab->size;
    fbt->start_addr = ab->start_addr;
    fbt->next = NULL;
    rearrange(MA_FF); //按地址大小从小到大排列
    // 进行可能的合并，基本策略如下
    // 1. 将新释放的结点插入到空闲分区队列末尾
    // 2. 对空闲链表按照地址有序排列
    // 3. 检查并合并相邻的空闲分区
    // 4. 将空闲链表重新按照当前算法排序
    pre = NULL;
    work = free_block;
    //查找插入位置
    while ((work != NULL) && (fbt->start_addr > work->start_addr)) {
        pre = work;
        work = work->next;
    }
    //插入当前节点
    //回收内存四种情况
    // 1）回收区与前一个空闲分区相邻接，与前一分区合并，修改前一分区的大小
    // 2）回收区与插入点的后一空闲分区相邻接，将两个分区合并，形成新的分区。（用回收区的首地址作为新分区的首地址，大小为其之和）
    // 3）回收区同时与前后两个空闲分区相邻接，合并三个分区，首地址为第一个分区的首址，大小为三个之和
    // 4）回收区与之均不邻接，建立新表项
    if (!pre) //插入空闲链表头
    {

        if (!work)
            free_block = fbt;   //空闲区只有当前被释放的这一个
        else
        {
            fbt->next = work;
            free_block = fbt;
            if (fbt->start_addr + fbt->size == work->start_addr) // 2)回收区与插入点的后一空闲分区相邻接
            {
                fbt->next = work->next;
                fbt->size = fbt->size + work->size;
                free(work);
            }
        }
    }
    else {
        if (!work) {    //需要插入到最后一个空闲区的后面
            pre->next = fbt;
            if (fbt->start_addr == pre->start_addr + pre->size) // 1)回收区与前一个空闲分区相邻接
            {
                pre->next = work;
                pre->size = fbt->size + pre->size;
                free(fbt);
            }
        }
        else {
            fbt->next = work;
            pre->next = fbt;
            // 检查并合并相邻的空闲分区
            if ((fbt->start_addr == pre->start_addr + pre->size) && (fbt->start_addr + fbt->size == work->start_addr)) // 3)
            {
                pre->next = work->next;
                pre->size = pre->size + fbt->size + work->size;
                free(fbt);
                free(work);
            }
            else if (fbt->start_addr == pre->start_addr + pre->size) // 1)
            {
                pre->next = work;
                pre->size = pre->size + fbt->size;
                free(fbt);
            }
            else if (work->start_addr == fbt->start_addr + fbt->size) // 2
            {
                fbt->next = work->next;
                fbt->size = work->size + fbt->size;
                free(work);
            }
        }
    }
    // 将空闲链表重新按照当前算法排序
    rearrange(ma_algorithm);
    return 1;
}

/*释放ab数据结构节点*/
int dispose(struct allocated_block *free_ab){
    struct allocated_block *pre, *ab;
   if(free_ab == allocated_block_head) { /*如果要释放第一个节点*/
     allocated_block_head = allocated_block_head->next;
        free(free_ab);
        return 1;
    }
    pre = allocated_block_head;  
    ab = allocated_block_head->next;
    while(ab!=free_ab){ pre = ab;  ab = ab->next; }
    pre->next = ab->next;
    free(ab);
    return 2;
}




/* 显示当前内存的使用情况，包括空闲区的情况和已经分配的情况 */
int display_mem_usage(){
    struct free_block_type *fbt=free_block;
    struct allocated_block *ab=allocated_block_head;
    if(fbt==NULL) return(-1);
    printf("----------------------------------------------------------\n");

    /* 显示空闲区 */
    printf("Free Memory:\n");
    printf("%20s %20s\n", "      start_addr", "       size");
    while(fbt!=NULL){
        printf("%20d %20d\n", fbt->start_addr, fbt->size);
        fbt=fbt->next;
    } 
    /* 显示已分配区 */
    printf("\nUsed Memory:\n");
    printf("%10s %20s %10s %10s\n", "PID", "ProcessName", "start_addr", " size");
    while(ab!=NULL){
        printf("%10d %20s %10d %10d\n", ab->pid, ab->process_name, ab->start_addr, ab->size);
        ab=ab->next;
    }
    printf("----------------------------------------------------------\n");
    return 0;
}

//退出程序并释放空间
void do_exit()
{
	struct free_block_type *free_pre,*free_ab;
	struct allocated_block *allocated_pre,*allocated_ab;
    free_pre = free_block;
    // 释放空闲区域
    if (free_pre != NULL)
    {
        free_ab = free_pre->next;
        for (; free_ab != NULL; free_pre = free_ab, free_ab = free_ab->next)
        {
            free(free_pre);
        }
        free(free_pre);
    }

    //释放已分配的区域
    allocated_pre = allocated_block_head;
    if (allocated_pre != NULL)
    {
        allocated_ab = allocated_pre->next;
        for (; allocated_ab != NULL; allocated_pre = allocated_ab, allocated_ab = allocated_ab->next)
        {
            free(allocated_pre);
        }
        free(allocated_pre);
    }
}



int main(){
    char choice;      
    pid=0;
    free_block = init_free_block(mem_size); //初始化空闲区
    while(1) {
        display_menu();	//显示菜单
        fflush(stdin);
        choice=getchar();	//获取用户输入
        // printf("Hello shit~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        
        switch(choice){
            case '1': set_mem_size(); break; 	//设置内存大小
            case '2': set_algorithm();flag=1; break;//设置算法
            case '3': new_process(); flag=1; break;//创建新进程
            case '4': kill_process(); flag=1;   break;//删除进程
            case '5': display_mem_usage();    flag=1; break;	//显示内存使用
            case '0': do_exit(); exit(0);	//释放链表并退出
            default: break;     
        }
        getchar();    
    } 
}
