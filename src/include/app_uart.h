/**
 * @file app_uart.h
 * @author cwxiaos
 * @brief UART初始化, 需要在tick=0前初始化, 在main()执行开始就初始化以便日志输出
 * @version 0.1
 * @date 2023-04-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef _APP_UART_H_
#define _APP_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "stm32f4xx.h"

void USART_Config(void);

#ifndef UART_BAUDRATE
#warning "UART_BAUDRATE not defined, use default@115200"
#define UART_BAUDRATE 115200
#endif

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // _APP_UART_H_