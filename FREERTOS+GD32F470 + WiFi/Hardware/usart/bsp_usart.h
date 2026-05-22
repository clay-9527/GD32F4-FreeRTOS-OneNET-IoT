#ifndef _BSP_USART_H
#define _BSP_USART_H

#include "gd32f4xx.h"
#include <stdio.h>
#include <stdarg.h>

/************************************************
 * Debug 串口 USART0
 * TX: PA9
 * RX: PA10
*************************************************/
#define DEBUG_USART_TX_RCU      RCU_GPIOA
#define DEBUG_USART_RX_RCU      RCU_GPIOA
#define DEBUG_USART_RCU         RCU_USART0

#define DEBUG_USART_TX_PORT     GPIOA
#define DEBUG_USART_RX_PORT     GPIOA
#define DEBUG_USART_TX_PIN      GPIO_PIN_9
#define DEBUG_USART_RX_PIN      GPIO_PIN_10
#define DEBUG_USART_AF          GPIO_AF_7

#define DEBUG_USART             USART0


/************************************************
 * ESP8266 串口 USART5
 * TX: PC6
 * RX: PC7
*************************************************/
#define ESP_USART_TX_RCU        RCU_GPIOC
#define ESP_USART_RX_RCU        RCU_GPIOC
#define ESP_USART_RCU           RCU_USART5

#define ESP_USART_TX_PORT       GPIOC
#define ESP_USART_RX_PORT       GPIOC
#define ESP_USART_TX_PIN        GPIO_PIN_6
#define ESP_USART_RX_PIN        GPIO_PIN_7
#define ESP_USART_AF            GPIO_AF_8

#define ESP_USART               USART5


/************************************************
 * 串口屏 USART2
 * TX: PC10
 * RX: PC11
*************************************************/
#define SCREEN_USART_TX_RCU     RCU_GPIOC
#define SCREEN_USART_RX_RCU     RCU_GPIOC
#define SCREEN_USART_RCU        RCU_USART2

#define SCREEN_USART_TX_PORT    GPIOC
#define SCREEN_USART_RX_PORT    GPIOC
#define SCREEN_USART_TX_PIN     GPIO_PIN_10
#define SCREEN_USART_RX_PIN     GPIO_PIN_11
#define SCREEN_USART_AF         GPIO_AF_7

#define SCREEN_USART            USART2


/************************************************
 * 函数声明
*************************************************/

/* 三个串口统一初始化 */
void usart_all_config(uint32_t debug_baud,
                      uint32_t esp_baud,
                      uint32_t screen_baud);

/* 单独初始化 */
void debug_usart_config(uint32_t baud_rate);
void esp_usart_config(uint32_t baud_rate);
void screen_usart_config(uint32_t baud_rate);

/* 发送单字节 */
void debug_usart_send_data(uint8_t data);
void esp_usart_send_data(uint8_t data);
void screen_usart_send_data(uint8_t data);

/* 发送字符串 */
void debug_usart_send_string(const char *str);
void esp_usart_send_string(const char *str);
void screen_usart_send_string(const char *str);

/* 独立 printf 打印函数 */
void debug_printf(const char *fmt, ...);
void esp_printf(const char *fmt, ...);
void screen_printf(const char *fmt, ...);

#endif


