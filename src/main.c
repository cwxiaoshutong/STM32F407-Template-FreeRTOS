/**
 * @file main.c
 * @author cwxiaos
 * @brief main file， 用于创建所有Application, 逻辑代码在app_main.c的app_main()下
 * @version 0.1
 * @date 2023-04-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "FreeRTOS.h"
#include "task.h"

#include "macros.h"

#include "util_data.h"

#include "app_main.h"
#include "app_uart.h"

#include "app_log.h"

static const char*  TAG = "main";

/**
 * @brief Application creater
 * 
 */
void application_creater(){
    LOGD(TAG, "Called@%d", __builtin_return_address(0));

    TaskHandle_t xHandle = NULL;

    // 这里的临界区是为了保证创建任务的时候不会被中断打断, 保证创建成功
    taskENTER_CRITICAL();

    LOGI(TAG, "Creating Application app_main");
    BaseType_t xReturn = xTaskCreate(app_main, "app_main", 512, NULL, 1, &xHandle);
    if(xReturn == pdPASS){
        SYSTEM_STATUS = 1;
        LOGI(TAG, "Create app_main success");
    }else{
        SYSTEM_STATUS = -1;
        LOGE(TAG, "Create app_main failed");
    }

    taskEXIT_CRITICAL();

    /* 以下删除任务作用是进程保活, 这和FreeRTOS的调度有关， 如果执行到RET了, 所有Task都会停止*/
    // 关于这个问题下面的网址有解答
    // https://www.freertos.org/FreeRTOS_Support_Forum_Archive/December_2013/freertos_Task_creation_in_another_task_9eb49261j.html
    vTaskDelete(NULL);

    LOGE(TAG, "Application Creater Halted");
}

/**
 * @brief 程序启动时执行
 * 这个constructor是GCC特性
 * 
 */
__attribute__((constructor)) void init(){
    // 初始化UART, 用于日志输出
    USART_Config();

    LOGI(TAG, "on System Start");
    // 下面的信息来自task.h , portmacro.h 和 编译器
    LOGI(TAG, "FreeRTOS Version: %s", tskKERNEL_VERSION_NUMBER);
    LOGI(TAG, "FreeRTOS Build Major: %d", tskKERNEL_VERSION_MAJOR);
    LOGI(TAG, "Build Date: %s", __DATE__);
    LOGI(TAG, "Build Time: %s", __TIME__);
    LOGI(TAG, "Compiler: %s", __VERSION__);
    LOGI(TAG, "Tick Rate: %d", configTICK_RATE_HZ);
    LOGI(TAG, "Tick Period: %d", portTICK_PERIOD_MS);
    LOGI(TAG, "Tick Type: %d", portTICK_TYPE_IS_ATOMIC);

    LOGD(TAG, "System on boot");

}

/**
 * @brief 程序退出时执行
 * 这个destructor是GCC特性
 * 
 */
__attribute__((destructor)) void deinit(){
    LOGE(TAG, "System Panic");
}

/**
 * @brief Entry point, Create task and start scheduler
 * 
 * @return int QEMU仿真可以看这个值, 实际上这个值没有意义， 如果返回了，说明FreeRTOS出错了
 */
int main(){
    LOGV(TAG, "Starting Application Creater...");
    
    // 此处创建线程, 具体逻辑代码实现在app_main()下
    // 只要调度没开始, Tick会保持0， 在这之前可以进行一些初始化工作
    /* 逻辑代码在app_main.c的app_main()下 */
    LOGI(TAG, "Creating application_creater")
    BaseType_t xReturn = xTaskCreate(application_creater, "vTask_Creater", 512, NULL, 1, NULL);
    if(xReturn == pdPASS){
        LOGI(TAG, "Create application_creater success");
        LOGI(TAG, "Starting FreeRTOS Scheduler...");
        vTaskStartScheduler();
    }else{
        LOGE(TAG, "Create application_creater failed");
    }
    
    // 如果执行到这里，FreeRTOS调度已经停止
    return SYSTEM_STATUS;
}