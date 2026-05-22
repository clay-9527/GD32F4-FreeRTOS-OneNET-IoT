#ifndef _BSP_KEY_H
#define _BSP_KEY_H

#include "gd32f4xx.h"

#define BSP_KEY_RCU   RCU_GPIOA   // 按键端口时钟
#define BSP_KEY_PORT  GPIOA       // 按键端口
#define BSP_KEY_PIN   GPIO_PIN_0  // 按键引脚
 
void key_gpio_config(void);       // key gpio引脚配置
void key_scan(void);              // 按键扫描




#endif  /* BSP_KEY_H */




