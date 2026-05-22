#include "gd32f4xx.h"
#include <stdio.h>
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_key.h"
#include "onenet.h"
#include "esp8266.h"
#include "FreeRTOS-TASK.h"
#include "FreeRTOS.h"
#include "task.h"

int main(void)
{
	  led_gpio_config();   			 // led初始化
    key_gpio_config(); 				 // key初始化
		usart_all_config(115200, 115200, 9600);
	
		freertos_task_create();

    /* 启动调度器 */
    vTaskStartScheduler();
	
    while(1) 
		{
	    
    }
}
