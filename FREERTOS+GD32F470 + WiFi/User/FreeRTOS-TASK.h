#ifndef __FREERTOS_TASK_H
#define __FREERTOS_TASK_H

#include "FreeRTOS.h"
#include "task.h"

/******************** 任务优先级 ********************/
#define START_TASK_PRIO     3
#define KEY_TASK_PRIO       2
#define NET_TASK_PRIO       2


/******************** 任务堆栈大小 ********************/
#define START_TASK_STK_SIZE 128
#define KEY_TASK_STK_SIZE   128
#define NET_STK_SIZE        1024


/******************** 任务句柄 ********************/
extern TaskHandle_t StartTask_Handler;
extern TaskHandle_t KeyTask_Handler;
extern TaskHandle_t NetTask_Handler;
/******************** 任务函数声明 ********************/
void start_task(void *pvParameters);
void key_task(void *pvParameters);
void net_task(void *pvParameters);


/******************** 对外接口 ********************/
void freertos_task_create(void);

#endif






