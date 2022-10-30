// /* 软中断通信实验程序 */ 
// #include <stdio.h> 
// #include <stdlib.h>
// #include <sys/wait.h>
// #include <signal.h>     
// #include <unistd.h>  
// #include <sys/types.h>
// int wait_flag; 
// void stop(int signum);

// int main( ) { 
//     int pid1, pid2;     					// 定义两个进程号变量  
//     signal(14,stop); //signal(2,stop);            				// 或者 signal(14,stop); 
//     alarm(8);
//     while((pid1 = fork( )) == -1);		// 若创建子进程1不成功,则空循环
//     if(pid1 > 0) {             	  	// 子进程创建成功,pid1为进程号 
//         while((pid2 = fork( )) == -1);	// 创建子进程2      
//         if(pid2 > 0) { 
//             wait_flag = 1; 
//             sleep(5);//补充；				// 父进程等待5秒 
//             while(wait_flag == 1);
//             kill(pid1, 14);//补充);      		       // 杀死进程1发中断号16
//             //sleep(1);
//             kill(pid2, 14);       			    // 杀死进程2  
//             wait(0);           			// 等待第1个子进程1结束的信号 
//             wait(0);           			// 等待第2个子进程2结束的信号 
//             printf("\n Parent process is killed !!\n"); //printf("\n Parent process is killed !!\n"); 
//             exit(0);           				           // 父进程结束 
//         }
//         else { 
//             wait_flag = 1; 
//             //printf("wait_flag2=%d ", wait_flag);
//             signal(14,stop);//补充; 		// 等待进程2被杀死的中断号17      
//             while(wait_flag == 1);                                                                              
//             printf("\n Child process 2 is killed by parent !!\n");//printf("\n Child process 2 is killed by parent !!\n"); 
//             exit(0);                                                                                                    
//         }
//     }                                                                                                                    
//     else {                                                                                                               
//         wait_flag = 1;   
//         //printf("wait_flag1=%d ", wait_flag);                                                                                  
//         signal(14,stop); 				// 等待进程1被杀死的中断号16 
//         while(wait_flag == 1);                                                                                        
//         printf("\n Child process 1 is killed by parent !!\n"); //printf("\n Child process 1 is killed by parent !!\n");                                                        
//         exit(0); 
//     }                                                                                                                    
// }  

// void stop(int signum) { 
//     wait_flag = 0;//补充; 
//     // printf("( wait_flag=%d, set by %d )", wait_flag, signum);
//     printf("\n %d stop test, pid: %d, wait_flag=%d\n", signum, getpid(), wait_flag);                                                                            
// }  

/* 软中断通信实验程序 */ 
#include <stdio.h> 
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>     
#include <unistd.h>  
#include <sys/types.h>
int wait_flag; 
void stop(int signum);

int main( ) { 
    int pid1, pid2;     					// 定义两个进程号变量  
    signal(3,stop);            				// 或者 signal(14,stop); 
    while((pid1 = fork( )) == -1);		// 若创建子进程1不成功,则空循环
    if(pid1 > 0) {             	  	// 子进程创建成功,pid1为进程号 
        while((pid2 = fork( )) == -1);	// 创建子进程2      
        if(pid2 > 0) { 
            wait_flag = 1; 
            sleep(5);//补充；				// 父进程等待5秒 
            while(wait_flag == 1);
            kill(pid1, 16);//补充);      		       // 杀死进程1发中断号16
            //sleep(1);
            kill(pid2, 17);       			    // 杀死进程2  
            wait(0);           			// 等待第1个子进程1结束的信号 
            wait(0);           			// 等待第2个子进程2结束的信号 
            printf("\n Parent process is killed !!  pid = %d\n", getpid()); //printf("\n Parent process is killed !!\n"); 
            exit(0);           				           // 父进程结束 
        }
        else { 
            wait_flag = 1;  // 父进程和子进程的wait_flag是两个独立的变量
            //printf("wait_flag2=%d ", wait_flag);
            signal(17,stop);//补充; 		// 等待进程2被杀死的中断号17      
            while(wait_flag == 1);                                                                              
            printf("\n Child process 2 is killed by parent !!  pid = %d\n", getpid());//printf("\n Child process 2 is killed by parent !!\n"); 
            exit(0);                                                                                                    
        }
    }                                                                                                                    
    else {                                                                                                               
        wait_flag = 1;   
        //printf("wait_flag1=%d ", wait_flag);                                                                                  
        signal(16,stop); 				// 等待进程1被杀死的中断号16 
        while(wait_flag == 1);                                                                                        
        printf("\n Child process 1 is killed by parent !!  pid = %d\n", getpid()); //printf("\n Child process 1 is killed by parent !!\n");                                                        
        exit(0); 
    }                                                                                                                    
}  

void stop(int signum) { 
    wait_flag = 0;//补充; 
    // printf("( wait_flag=%d, set by %d )", wait_flag, signum);
    printf("\n %d stop test, pid: %d, wait_flag=%d\n", signum, getpid(), wait_flag);                                                                            
}  


// /* 软中断通信实验程序 */ 
// #include <stdio.h> 
// #include <stdlib.h>
// #include <sys/wait.h>
// #include <signal.h>     
// #include <unistd.h>  
// #include <sys/types.h>
// int wait_flag; 
// void stop(int signum);

// int main( ) { 
//     int pid1, pid2;     					// 定义两个进程号变量  
//     signal(3,stop);            				// 或者 signal(14,stop); 
//     while((pid1 = fork( )) == -1);		// 若创建子进程1不成功,则空循环
//     if(pid1 > 0) {             	  	// 子进程创建成功,pid1为进程号 
//         while((pid2 = fork( )) == -1);	// 创建子进程2      
//         if(pid2 > 0) { 
//             wait_flag = 1; 
//             sleep(5);//补充；				// 父进程等待5秒 
//             kill(pid1, 16);//补充);      		       // 杀死进程1发中断号16
//             kill(pid2, 17);       			    // 杀死进程2  
//             wait(NULL);           			// 等待第1个子进程1结束的信号 
//             wait(0);           			// 等待第2个子进程2结束的信号 
//             printf("\n Parent process is killed !!\n"); //printf("\n Parent process is killed !!\n"); 
//             exit(0);           				           // 父进程结束 
//         }
//         else { 
//             wait_flag = 1; 
//             signal(17,stop);//补充; 		// 等待进程2被杀死的中断号17      
                                                                                  
//             printf("\n Child process 2 is killed by parent !!\n");//printf("\n Child process 2 is killed by parent !!\n"); 
//             exit(0);                                                                                                    
//         }
//     }                                                                                                                    
//     else {                                                                                                               
//         wait_flag = 1;                                                                                     
//         signal(16,stop); 				// 等待进程1被杀死的中断号16 
                                                                                   
//         printf("\n Child process 1 is killed by parent !!\n"); //printf("\n Child process 1 is killed by parent !!\n");                                                        
//         exit(0); 
//     }                                                                                                                    
// }  

// void stop(int signum) { 
    
//     printf("\n %d stop test \n", signum);//printf("\n %d stop test \n", signum);                                                                            
// }  