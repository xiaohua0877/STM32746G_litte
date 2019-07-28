/*
*********************************************************************************************************
*                                               uC/OS-III
*                                          The Real-Time Kernel
*
*                         (c) Copyright 2009-2018; Silicon Laboratories Inc.,
*                                400 W. Cesar Chavez, Austin, TX 78701
*
*                   All rights reserved. Protected by international copyright laws.
*
*                  Your use of this software is subject to your acceptance of the terms
*                  of a Silicon Labs Micrium software license, which can be obtained by
*                  contacting info@micrium.com. If you do not agree to the terms of this
*                  license, you may not use this software.
*
*                  Please help us continue to provide the Embedded community with the finest
*                  software available. Your honesty is greatly appreciated.
*
*                    You can find our product's documentation at: doc.micrium.com
*
*                          For more information visit us at: www.micrium.com
*********************************************************************************************************
*/
#include  <stm32f7xx_hal.h>
#include  <cpu.h>
#include  <lib_math.h>
#include  <lib_mem.h>
#include  <os.h>
#include  <os_app_hooks.h>
#include  <app_cfg.h>
#include  "bsp_int.h"
#include  <bsp_led.h>
#include  <bsp_os.h>
#include  <bsp_clk.h>



//关于优先级的说明：
//系统保留有5个任务，这五个任务占了5个优先级，用户不得使用这5个优先级
//优先级0：中断服务管理任务：OS_IntQTask();
//优先级1：时钟节拍任务：OS_TickTask();
//优先级2：定时任务：OS_TmrTask();
//优先级OS_CFG_PRIO_MAX-2(倒数第二个优先级)：统计任务：OS_StatTask();
//优先级OS_CFG_PRIO_MAX-1(倒数第一个优先级)：空闲任务：OS_IdleTask();

#define START_TASK_PRIO         3                   //任务优先级;
#define START_STK_SIZE          128                 //任务堆栈大小,实际大小是：128*4字节
CPU_STK START_TASK_STK[START_STK_SIZE];             //任务堆栈;
OS_TCB StartTaskTCB;                                //任务控制块;
//声明任务函数(一般将一个任务写成一个函数):
void start_task(void);                       //注：“p_arg”这个参数基本用不上,但必须得写上

#define LED0_TASK_PRIO          4                   //任务优先级;
#define LED0_STK_SIZE           128                 //任务堆栈大小,实际大小是：128*4字节
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];               //任务堆栈;
OS_TCB Led0TaskTCB;                                 //任务控制块;
//声明任务函数(一般将一个任务写成一个函数):
void Led0_task(void *p_arg);                        //注：“p_arg”这个参数基本用不上,但必须得写上

#define LED1_TASK_PRIO          5                   //任务优先级;
#define LED1_STK_SIZE           128                 //任务堆栈大小,实际大小是：128*4字节
CPU_STK LED1_TASK_STK[LED1_STK_SIZE];               //任务堆栈;
OS_TCB Led1TaskTCB;                                 //任务控制块;
//声明任务函数(一般将一个任务写成一个函数):
void Led1_task(void *p_arg);                        //注：“p_arg”这个参数基本用不上,但必须得写上

#define Printf_TASK_PRIO         6                  //任务优先级;
#define Printf_STK_SIZE          128                //任务堆栈大小,实际大小是：128*4字节
CPU_STK Printf_TASK_STK[Printf_STK_SIZE];           //任务堆栈;
OS_TCB PrintfTaskTCB;                               //任务控制块;
//声明任务函数(一般将一个任务写成一个函数):
void Printf_task(void *p_arg);                      //注：“p_arg”这个参数基本用不上,但必须得写上

typedef struct {
    int  Led0_task;
    int  Led1_task;
    int  printf_task;
    
}task_info_t;

task_info_t         taskInfo;


//任务函数;注:一般将其它任务的创建放到一个专门创建任务的函数中;
void start_task(void)
{
    OS_ERR   os_err;
    CPU_ERR  cpu_err;

    //创建LED0任务:
    OSTaskCreate(   (OS_TCB *)      &Led0TaskTCB,
                    (CPU_CHAR *)    "led0 task",
                    (OS_TASK_PTR)   Led0_task,
                    (void *)        0,
                    (OS_PRIO)       LED0_TASK_PRIO,
                    (CPU_STK *)     &LED0_TASK_STK[0],
                    (CPU_STK_SIZE)  LED0_STK_SIZE/10,
                    (CPU_STK_SIZE)  LED0_STK_SIZE,
                    (OS_MSG_QTY)    0,
                    (OS_TICK)       0,
                    (void *)        0,
                    (OS_OPT)        OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                    (OS_ERR *)      &os_err
                );
                 
    //创建LED1任务:
    OSTaskCreate(   (OS_TCB *)      &Led1TaskTCB,
                    (CPU_CHAR *)    "led1 task",
                    (OS_TASK_PTR)   Led1_task,
                    (void *)        0,
                    (OS_PRIO)       LED1_TASK_PRIO,
                    (CPU_STK *)     &LED1_TASK_STK[0],
                    (CPU_STK_SIZE)  LED1_STK_SIZE/10,
                    (CPU_STK_SIZE)  LED1_STK_SIZE,
                    (OS_MSG_QTY)    0,
                    (OS_TICK)       0,
                    (void *)        0,
                    (OS_OPT)        OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                    (OS_ERR *)      &os_err
                );
             
    //创建Printf任务:
    OSTaskCreate(   (OS_TCB *)         &PrintfTaskTCB,
                    (CPU_CHAR *)       "Printf test task",
                    (OS_TASK_PTR)      Printf_task,
                    (void *)           0,
                    (OS_PRIO)          Printf_TASK_PRIO,
                    (CPU_STK *)        &Printf_TASK_STK[0],
                    (CPU_STK_SIZE)     Printf_STK_SIZE/10,
                    (CPU_STK_SIZE)     Printf_STK_SIZE,
                    (OS_MSG_QTY)       0,
                    (OS_TICK)          0,
                    (void *)           0,
                    (OS_OPT)           OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                    (OS_ERR*)          &os_err
                );    
   
}

//*******************************************************************************

//                                  具体3个任务

//*******************************************************************************

//"Led0_task"任务:
void Led0_task(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;          //故意使用一下这个参数,否则编译器会警告说此参数没有使用
    while(1)
    {
        //LED0=~LED0;
        taskInfo.Led0_task++;
        OSTimeDly(200, OS_OPT_TIME_HMSM_STRICT,&err);
        //OSTimeDlyHMSM(0,0,0,300,OS_OPT_TIME_HMSM_STRICT,&err);
    }
}

//"Led1_task"任务:
void Led1_task(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;          //故意使用一下这个参数,否则编译器会警告说此参数没有使用
    while(1)
    {
        //LED1=~LED1;
        taskInfo.Led1_task++;
        OSTimeDly(400, OS_OPT_TIME_HMSM_STRICT,&err);
        //OSTimeDlyHMSM(0,0,0,300,OS_OPT_TIME_HMSM_STRICT,&err);
    }
}

//"Printf_task"任务:
void Printf_task(void *p_arg)
{
    OS_ERR err;
    CPU_SR_ALLOC();
    p_arg = p_arg;          //故意使用一下这个参数,否则编译器会警告说此参数没有使用
    while(1)
    {
        //OS_CRITICAL_ENTER();    //进入临界区
       // printf("Hello World!\r\n");
       // OS_CRITICAL_EXIT();        //退出临界区
       taskInfo.printf_task++;
       // delay_ms(300);
        OSTimeDlyHMSM(0,0,0,300,OS_OPT_TIME_HMSM_STRICT,&err);
    }
}

