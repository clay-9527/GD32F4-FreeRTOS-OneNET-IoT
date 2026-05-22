#include "FreeRTOS-TASK.h"
#include "gd32f4xx.h"
#include <stdio.h>
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_key.h"
#include "esp8266.h"
#include "onenet.h"
#include "bsp_usart.h"
#include "FreeRTOS.h"
#include "task.h"

/* 如果 key_scan() 在别的 .c 文件里定义了，这里做外部声明 */
extern void key_scan(void);


#define ESP8266_ONENET_INFO  "AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"

/******************** 任务句柄定义 ********************/
TaskHandle_t StartTask_Handler = NULL;
TaskHandle_t KeyTask_Handler   = NULL;
TaskHandle_t NetTask_Handler = NULL;
/************************************************
函数名称 ： freertos_task_create
功    能 ： 创建启动任务
参    数 ： 无
返 回 值 ： 无
*************************************************/
void freertos_task_create(void)
{
    xTaskCreate((TaskFunction_t )start_task,           /* 任务函数 */
                (const char*    )"start_task",        /* 任务名称 */
                (uint16_t       )START_TASK_STK_SIZE, /* 任务堆栈大小 */
                (void*          )NULL,                /* 传递给任务函数的参数 */
                (UBaseType_t    )START_TASK_PRIO,     /* 任务优先级 */
                (TaskHandle_t*  )&StartTask_Handler); /* 任务句柄 */
}

/************************************************
函数名称 ： start_task
功    能 ： 启动任务，创建其他任务后删除自身
参    数 ： pvParameters: 传入参数
返 回 值 ： 无
*************************************************/
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();   /* 进入临界区，防止创建任务被打断 */

    /* 创建按键任务 */
    xTaskCreate((TaskFunction_t )key_task,
                (const char*    )"key_task",
                (uint16_t       )KEY_TASK_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )KEY_TASK_PRIO,
                (TaskHandle_t*  )&KeyTask_Handler);
				
		xTaskCreate((TaskFunction_t )net_task,
            (const char*    )"net_task",
            (uint16_t       )NET_STK_SIZE,
            (void*          )NULL,
            (UBaseType_t    )NET_TASK_PRIO,
            (TaskHandle_t*  )&NetTask_Handler);
								

    vTaskDelete(StartTask_Handler);   /* 删除启动任务自身 */

    taskEXIT_CRITICAL();              /* 理论上这里不会执行到 */
}

/************************************************
函数名称 ： key_task
功    能 ： 按键扫描任务，调用已有 key_scan() 控制灯翻转
参    数 ： pvParameters: 传入参数
返 回 值 ： 无
说    明 ： 这里不重复写按键逻辑，直接调用你已有的 key_scan()
*************************************************/
void key_task(void *pvParameters)
{
    uint8_t key_last = 0;
    uint8_t key_now  = 0;

    while(1)
    {
        key_now = gpio_input_bit_get(BSP_KEY_PORT, BSP_KEY_PIN);

        if((key_now == SET) && (key_last == RESET))
        {
            vTaskDelay(pdMS_TO_TICKS(20));   // 消抖

            if(gpio_input_bit_get(BSP_KEY_PORT, BSP_KEY_PIN) == SET)
            {
                gpio_bit_toggle(PORT_LED2, PIN_LED2);

                while(gpio_input_bit_get(BSP_KEY_PORT, BSP_KEY_PIN) == SET)
                {
                    vTaskDelay(pdMS_TO_TICKS(10));   // 不空转死等
                }
            }
        }

        key_last = key_now;
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

static void OneNET_NetworkInit(void)
{
    debug_printf("Connect MQTT Server...\r\n");

    while(ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
    {
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    debug_printf("Connect MQTT Successfully\r\n");

    while(OneNet_DevLink())
    {
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    OneNET_Subscribe();

    debug_printf("OneNET Ready\r\n");
}

void net_task(void *pvParameters)
{
    debug_printf("Net Task Start\r\n");

    ESP8266_Init();

    debug_printf("ESP8266 Init Finished\r\n");

    OneNET_NetworkInit();

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}