/**
 * @file macros.h
 * @author cwxiaos
 * @brief 一些宏函数
 * @version 0.1
 * @date 2023-05-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */


/**
 * @brief 和GCC内置函数__builtin_expect配合使用，用于分支预测优化，提高程序运行效率
 * @example if(likely(a > b)) { ... } else { ... }
 * 
 */
#define likely(x)   __builtin_expect(!!(x), 1)   //执行if后面语句的可能性大些
#define unlikely(x) __builtin_expect(!!(x), 0)   //执行else后面语句的可能性大些

/**
 * @brief GCC内置函数，用于获取函数调用栈的返回地址
 * 
 */
#define stack_return(LEVEL) __builtin_return_address (LEVEL)   //LEVEL调用栈的级数

#define __purefunc  __attribute__((pure))   //表明函数除返回值外，不会通过其它（如全局变量、指针）对函数外部产生任何影响

#define is_const(x) __builtin_constant_p(x)   //判断x是否为常量表达式

#include <string.h>
#define FILE(x) strrchr(x,'/')?strrchr(x,'/')+1:x

#define TRACE_INFO "Traceback: File: %s, line %d, in %s\n", FILE(__FILE__), __LINE__, __func__   //函数位置信息
#define trace() printf(TRACE_INFO)   //打印函数位置