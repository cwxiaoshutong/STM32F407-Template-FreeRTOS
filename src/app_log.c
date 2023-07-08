/**
 * @file app_log.c
 * @headerfile app_log.h
 * @author cwxiaos
 * @brief 从ESP32移植的LOG库, 使用了ANSI转义符以控制日志颜色, 部分终端不支持, 会直接显示转义字符
 *        另外可以通过define LOG_LOCAL_LEVEL来控制日志输出等级, 详见app_log.h
 * @version 0.1
 * @date 2023-04-06
 * 
 * @copyright Copyright (c) 2023
 * 
 * @example const char* TAG = "example";
 *          LOGI(TAG, "This is a log info example");
 *          LOGE(TAG, "This is a log error example");
 *          LOGW(TAG, "This is a log warning example");
 *          LOGD(TAG, "This is a log debug example");
 *          LOGV(TAG, "This is a log verbose example");
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#include "app_log.h"

#include "FreeRTOS.h"
#include "task.h"

void log_write(log_level_t level,
                   const char *tag,
                   const char *format, ...){
    // 进入临界区, 防止多线程同时访问导致输出交错
    portENTER_CRITICAL();
    va_list arg;
    va_start(arg, format);
    vprintf(format, arg);
    va_end(arg);
    portEXIT_CRITICAL();
}

#include "FreeRTOS.h"
#include "task.h"
uint32_t log_timestamp(void){
    /* 获取系统Tick, 与系统有关*/
    // FreeRTOS 使用 xTaskGetTickCount()
    // Zephyr 使用 k_uptime_get_32()
    return xTaskGetTickCount();
}