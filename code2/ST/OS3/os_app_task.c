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



//�������ȼ���˵����
//ϵͳ������5���������������ռ��5�����ȼ����û�����ʹ����5�����ȼ�
//���ȼ�0���жϷ����������OS_IntQTask();
//���ȼ�1��ʱ�ӽ�������OS_TickTask();
//���ȼ�2����ʱ����OS_TmrTask();
//���ȼ�OS_CFG_PRIO_MAX-2(�����ڶ������ȼ�)��ͳ������OS_StatTask();
//���ȼ�OS_CFG_PRIO_MAX-1(������һ�����ȼ�)����������OS_IdleTask();

#define START_TASK_PRIO         3                   //�������ȼ�;
#define START_STK_SIZE          128                 //�����ջ��С,ʵ�ʴ�С�ǣ�128*4�ֽ�
CPU_STK START_TASK_STK[START_STK_SIZE];             //�����ջ;
OS_TCB StartTaskTCB;                                //������ƿ�;
//����������(һ�㽫һ������д��һ������):
void start_task(void);                       //ע����p_arg��������������ò���,�������д��

#define LED0_TASK_PRIO          4                   //�������ȼ�;
#define LED0_STK_SIZE           128                 //�����ջ��С,ʵ�ʴ�С�ǣ�128*4�ֽ�
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];               //�����ջ;
OS_TCB Led0TaskTCB;                                 //������ƿ�;
//����������(һ�㽫һ������д��һ������):
void Led0_task(void *p_arg);                        //ע����p_arg��������������ò���,�������д��

#define LED1_TASK_PRIO          5                   //�������ȼ�;
#define LED1_STK_SIZE           128                 //�����ջ��С,ʵ�ʴ�С�ǣ�128*4�ֽ�
CPU_STK LED1_TASK_STK[LED1_STK_SIZE];               //�����ջ;
OS_TCB Led1TaskTCB;                                 //������ƿ�;
//����������(һ�㽫һ������д��һ������):
void Led1_task(void *p_arg);                        //ע����p_arg��������������ò���,�������д��

#define Printf_TASK_PRIO         6                  //�������ȼ�;
#define Printf_STK_SIZE          128                //�����ջ��С,ʵ�ʴ�С�ǣ�128*4�ֽ�
CPU_STK Printf_TASK_STK[Printf_STK_SIZE];           //�����ջ;
OS_TCB PrintfTaskTCB;                               //������ƿ�;
//����������(һ�㽫һ������д��һ������):
void Printf_task(void *p_arg);                      //ע����p_arg��������������ò���,�������д��

typedef struct {
    int  Led0_task;
    int  Led1_task;
    int  printf_task;
    
}task_info_t;

task_info_t         taskInfo;


//������;ע:һ�㽫��������Ĵ����ŵ�һ��ר�Ŵ�������ĺ�����;
void start_task(void)
{
    OS_ERR   os_err;
    CPU_ERR  cpu_err;

    //����LED0����:
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
                 
    //����LED1����:
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
             
    //����Printf����:
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

//                                  ����3������

//*******************************************************************************

//"Led0_task"����:
void Led0_task(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;          //����ʹ��һ���������,����������ᾯ��˵�˲���û��ʹ��
    while(1)
    {
        //LED0=~LED0;
        taskInfo.Led0_task++;
        OSTimeDly(200, OS_OPT_TIME_HMSM_STRICT,&err);
        //OSTimeDlyHMSM(0,0,0,300,OS_OPT_TIME_HMSM_STRICT,&err);
    }
}

//"Led1_task"����:
void Led1_task(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;          //����ʹ��һ���������,����������ᾯ��˵�˲���û��ʹ��
    while(1)
    {
        //LED1=~LED1;
        taskInfo.Led1_task++;
        OSTimeDly(400, OS_OPT_TIME_HMSM_STRICT,&err);
        //OSTimeDlyHMSM(0,0,0,300,OS_OPT_TIME_HMSM_STRICT,&err);
    }
}

//"Printf_task"����:
void Printf_task(void *p_arg)
{
    OS_ERR err;
    CPU_SR_ALLOC();
    p_arg = p_arg;          //����ʹ��һ���������,����������ᾯ��˵�˲���û��ʹ��
    while(1)
    {
        //OS_CRITICAL_ENTER();    //�����ٽ���
       // printf("Hello World!\r\n");
       // OS_CRITICAL_EXIT();        //�˳��ٽ���
       taskInfo.printf_task++;
       // delay_ms(300);
        OSTimeDlyHMSM(0,0,0,300,OS_OPT_TIME_HMSM_STRICT,&err);
    }
}

