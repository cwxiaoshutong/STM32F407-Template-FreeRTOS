/**
 * @file app_uart.c
 * @author cwxiaos
 * @brief uart初始化
 * @version 0.1
 * @date 2023-04-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "misc.h"

#include "app_uart.h"

void USART_Config(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟

	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1

	//USART1端口配置
	GPIO_InitTypeDef GPIO_InitStructure = {
		.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10,
		.GPIO_Mode = GPIO_Mode_AF,
		.GPIO_Speed = GPIO_Speed_50MHz,
		.GPIO_OType = GPIO_OType_PP,
		.GPIO_PuPd = GPIO_PuPd_UP,
	};

	GPIO_Init(GPIOA,&GPIO_InitStructure);
  
    /* Enable the USARTx Interrupt */
	NVIC_InitTypeDef NVIC_InitStructure = {
    	.NVIC_IRQChannel = USART1_IRQn,
    	.NVIC_IRQChannelPreemptionPriority = 0,
    	.NVIC_IRQChannelSubPriority = 0,
    	.NVIC_IRQChannelCmd = ENABLE,
	};

	NVIC_Init(&NVIC_InitStructure);
	
	USART_InitTypeDef USART_InitStructure = {
		.USART_BaudRate = UART_BAUDRATE,
		.USART_WordLength = USART_WordLength_8b,
		.USART_StopBits = USART_StopBits_1,
		.USART_Parity = USART_Parity_No,
		.USART_HardwareFlowControl = USART_HardwareFlowControl_None,
		.USART_Mode = USART_Mode_Rx | USART_Mode_Tx,
	};

	USART_Init(USART1,&USART_InitStructure);

	USART_Cmd(USART1, ENABLE);
	
	// 不加这个while，将会丢失一个字符
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);

	USART_ClearITPendingBit(USART1, USART_IT_TC); 
	USART_ITConfig(USART1, USART_IT_RXNE , ENABLE);
}

/**
 * @brief 串口中断处理函数, 注释掉USART_SendData(USART1, ch)可以关掉回显
 * 
 */
void USART1_IRQHandler(void){
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
        char ch = USART_ReceiveData(USART1);
        USART_SendData(USART1, ch);
    }
}