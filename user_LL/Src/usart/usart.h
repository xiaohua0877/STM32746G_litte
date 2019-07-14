#ifndef _USART_H
#define _USART_H
//#include "sys.h"
#include "stdio.h"  
#include "stm32f7xx_hal.h"
//////////////////////////////////////////////////////////////////////////////////   
//本程序只供学习使用，未经作者许可，不得用于其它任何用�?
//ALIENTEK STM32F7开发板
//串口1初始�?          
//正点原子@ALIENTEK
//技术论�?www.openedv.csom
//修改日期:2016/6/23
//版本：V1.0
//版权所有，盗版必究�?
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//V1.0修改说明 
//////////////////////////////////////////////////////////////////////////////////  
#define USART_REC_LEN           200     //定义最大接收字节数 200
#define EN_USART1_RX            1       //使能�?�?禁止�?）串�?接收
        
extern uint8_t  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字�?末字节为换行�?
extern uint16_t USART_RX_STA;               //接收状态标�?   
extern UART_HandleTypeDef UART1_Handler; //UART句柄

#define RXBUFFERSIZE   1 //缓存大小
uint8_t aRxBuffer[128];//HAL库USART接收Buffer

//如果想串口中断接收，请不要注释以下宏定义
void uart_init(uint32_t bound);
#endif
