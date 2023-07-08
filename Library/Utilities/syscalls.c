/**
 * @file syscalls.c
 * @author cwxiaos
 * @brief 重写系统调用, 其中_write()函数用于重定向printf()函数， _read()函数用于重定向scanf()函数
 * @version 0.1
 * @date 2023-04-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "stdio.h"
#include "stm32f4xx.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/***************************************************************************/

// int _open(const char *name, int flags, int mode){
//   return -1;
// }

//__attribute__((weak))
int _read(int file, char *ptr, int len) {
    for (int i = 0; i < len; i++) {
        while (!(USART1->SR & USART_SR_RXNE));
        ptr[i] = (USART1->DR & 0xFF);
    }
    return len;
}

/***************************************************************************/

// int _lseek(int file, int ptr, int dir) {
//   file = file;
//   ptr = ptr;
//   dir = dir;
//   return 0;
// }

/***************************************************************************/
// QueueHandle_t logQueue;
// #define LOG_QUEUE_LENGTH 32
// void __write(){

// }

int _write(int file, char * ptr, int len) {
  // if (logQueue == NULL) {
  //       logQueue = xQueueCreate(LOG_QUEUE_LENGTH, sizeof(char));
  // }
  if (!ptr) {
    return 0;
  }
  for (int index = 0; index < len; index++) {
    while (!(USART1->SR & 0x00000040));
    USART_SendData(USART1, ptr[index]);
  }
  return len;
}

int __io_putchar(int ch) {
    while (!(USART1->SR & USART_SR_TXE));
    // 发送数据
    USART1->DR = ch & 0xFF;
    return ch;
}

/***************************************************************************/

// int _close(int file) {
//   return 0;
// }

/***************************************************************************/

// /* Register name faking - works in collusion with the linker.  */
// register char * stack_ptr asm ("sp");

// caddr_t _sbrk(int incr) {
//   extern char   end asm ("end"); // Defined by the linker.
//   static char * heap_end;
//   char *        prev_heap_end;
//   if (heap_end == NULL)
//     heap_end = & end;
//   prev_heap_end = heap_end;
//   if (heap_end + incr > stack_ptr) {
//     // Some of the libstdc++-v3 tests rely upon detecting
//     // out of memory errors, so do not abort here.
// #if 0
//     extern void abort (void);
//     _write (1, "_sbrk: Heap and stack collision\n", 32);
//     abort ();
// #else
//     errno = ENOMEM;
//     return (caddr_t) -1;
// #endif
//   }
//   heap_end += incr;
//   return (caddr_t) prev_heap_end;
// }
extern int __bss_end__; // 在链接脚本中定义的符号

static char *heap_end = NULL;
char *stack_ptr = (char *)(0x8000000); // Cortex-M的起始栈指针

void* _sbrk(int incr) {
    char *prev_heap_end;

    if (heap_end == NULL) {
        heap_end = (char *)&__bss_end__;
    }

    prev_heap_end = heap_end;

    if (heap_end + incr > stack_ptr) {
        // Heap和Stack碰撞，返回错误
        return (void *)-1;
    }

    heap_end += incr;
    return (void *)prev_heap_end;
}

/***************************************************************************/

int _fstat(int file, struct stat * st) {
  file = file;
  memset (st, 0, sizeof (* st));
  st->st_mode = S_IFCHR;
  return 0;
}

/***************************************************************************/

// int _isatty(int fd) {
//   fd = fd;
//   return 1;
// }

int _kill(int pid, int sig) {
  errno=EINVAL;
  return(-1);
}

int _getpid(void) {
  return 1;
}

int _exit(int status) {
  _write(1, "exit", 4);
  while (1);
}

/*** EOF ***/
