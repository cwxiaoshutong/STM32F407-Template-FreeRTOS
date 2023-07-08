/**
 * @file app_log.h
 * @author cwxiaos
 * @brief 这个库来自于ESP-IDF， 做了一些修改, 移除了ESP字样, 在-Wall下存在警告, 但是不影响使用
 * @version 0.1
 * @date 2023-04-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include <stdint.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif

#define strlcpy(a, b, c)
#define strlcat(a, b, c)

#define heap_caps_malloc(a, b)  NULL
#define MALLOC_CAP_INTERNAL     0
#define MALLOC_CAP_8BIT         0

//这里的LOG_LOCAL_LEVEL是一个宏，用来定义日志的级别，可以在编译时通过-DLOG_LOCAL_LEVEL=LOG_VERBOSE来定义
/* 只有高于这里定义的日志等级的日志才会输出*/
#ifndef LOG_LOCAL_LEVEL
#warning "LOG_LOCAL_LEVEL not defined, defaulting to LOG_WARN"
#define LOG_LOCAL_LEVEL LOG_VERBOSE
#endif

typedef enum {
    LOG_NONE,       /*!< No log output */
    LOG_ERROR,      /*!< Critical errors, software module can not recover on its own */
    LOG_WARN,       /*!< Error conditions from which recovery measures have been taken */
    LOG_INFO,       /*!< Information messages which describe normal flow of events */
    LOG_DEBUG,      /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
    LOG_VERBOSE     /*!< Bigger chunks of debugging information, or frequent messages which can potentially flood the output. */
} log_level_t;

// "\033[30m 黑色 \033[0m"
// "\033[31m 红色 \033[0m"
// "\033[32m 绿色 \033[0m"
// "\033[33m 黄色 \033[0m"
// "\033[34m 蓝色 \033[0m"
// "\033[35m 紫色 \033[0m"
// "\033[36m 青色 \033[0m"
// "\033[37m 白色 \033[0m"

// "\033[40;37m 黑底白字 \033[0m"
// "\033[41;30m 红底黑字 \033[0m"
// "\033[42;30m 绿底黑字 \033[0m"
// "\033[43;30m 黄底黑字 \033[0m"
// "\033[44;37m 蓝底白字 \033[0m"
// "\033[45;30m 紫底黑字 \033[0m"
// "\033[46;30m 青底黑字 \033[0m"
// "\033[47;30m 白底黑字 \033[0m"

// "\033[5m 闪烁 \033[0m"
// "\033[8m 隐藏 \033[0m"
// "\033[1m 加粗 \033[0m"
// "\033[4m 下划线 \033[0m"
// "\033[7m 反显 \033[0m"
// "\033[30;42m 黑底绿字 \033[0m"

// "\033[0m 关闭所有属性 \033[0m"
// "\033[1;5;31m 加粗闪烁红字 \033[0m"

#define LOG_COLOR_E "\033[31m"
#define LOG_COLOR_W "\033[33m"
#define LOG_COLOR_I "\033[32m"
#define LOG_COLOR_D "\033[44;37m"
#define LOG_COLOR_V "\033[34m"
#define LOG_RESET_COLOR "\033[0m"

#undef _Static_assert
#define _Static_assert(cond, message)

uint32_t log_timestamp(void);
void log_write(log_level_t level, const char* tag, const char* format, ...) __attribute__ ((format (printf, 3, 4)));

#define LOG_FORMAT(letter, format)  LOG_COLOR_ ## letter #letter " (%d) %s: " format LOG_RESET_COLOR "\r\n"

#define LOGE( tag, format, ... )  if (LOG_LOCAL_LEVEL >= LOG_ERROR)   { log_write(LOG_ERROR,   tag, LOG_FORMAT(E, format), log_timestamp(), tag, ##__VA_ARGS__); }

#define LOGW( tag, format, ... )  if (LOG_LOCAL_LEVEL >= LOG_WARN)    { log_write(LOG_WARN,    tag, LOG_FORMAT(W, format), log_timestamp(), tag, ##__VA_ARGS__); }

#define LOGI( tag, format, ... )  if (LOG_LOCAL_LEVEL >= LOG_INFO)    { log_write(LOG_INFO,    tag, LOG_FORMAT(I, format), log_timestamp(), tag, ##__VA_ARGS__); }

#define LOGD( tag, format, ... )  if (LOG_LOCAL_LEVEL >= LOG_DEBUG)   { log_write(LOG_DEBUG,   tag, LOG_FORMAT(D, format), log_timestamp(), tag, ##__VA_ARGS__); }

#define LOGV( tag, format, ... )  if (LOG_LOCAL_LEVEL >= LOG_VERBOSE) { log_write(LOG_VERBOSE, tag, LOG_FORMAT(V, format), log_timestamp(), tag, ##__VA_ARGS__); }

#ifdef __cplusplus
}
#endif