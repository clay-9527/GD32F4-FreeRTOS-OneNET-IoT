
#include "bsp_usart.h"
#include "stdio.h"
#include <string.h>

#define USART_PRINTF_BUF_SIZE   256


/************************************************
函数名称 ： usart_base_config
功    能 ： 串口底层通用配置函数
参    数 ： usart_periph : 串口外设
           baud_rate    : 波特率
返 回 值 ： 无
*************************************************/
static void usart_base_config(uint32_t usart_periph, uint32_t baud_rate)
{
    usart_deinit(usart_periph);                              // 复位串口
    usart_baudrate_set(usart_periph, baud_rate);             // 设置波特率
    usart_parity_config(usart_periph, USART_PM_NONE);        // 无校验位
    usart_word_length_set(usart_periph, USART_WL_8BIT);      // 8位数据位
    usart_stop_bit_set(usart_periph, USART_STB_1BIT);        // 1位停止位

    usart_transmit_config(usart_periph, USART_TRANSMIT_ENABLE); // 使能发送
    usart_receive_config(usart_periph, USART_RECEIVE_ENABLE);   // 使能接收

    usart_enable(usart_periph);                              // 使能串口
}


/************************************************
函数名称 ： debug_usart_config
功    能 ： Debug串口 USART0 初始化
引    脚 ： TX PA9，RX PA10
*************************************************/
void debug_usart_config(uint32_t baud_rate)
{
    rcu_periph_clock_enable(DEBUG_USART_TX_RCU);
    rcu_periph_clock_enable(DEBUG_USART_RX_RCU);
    rcu_periph_clock_enable(DEBUG_USART_RCU);

    gpio_af_set(DEBUG_USART_TX_PORT, DEBUG_USART_AF, DEBUG_USART_TX_PIN);
    gpio_af_set(DEBUG_USART_RX_PORT, DEBUG_USART_AF, DEBUG_USART_RX_PIN);

    gpio_mode_set(DEBUG_USART_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, DEBUG_USART_TX_PIN);
    gpio_mode_set(DEBUG_USART_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, DEBUG_USART_RX_PIN);

    gpio_output_options_set(DEBUG_USART_TX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, DEBUG_USART_TX_PIN);
    gpio_output_options_set(DEBUG_USART_RX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, DEBUG_USART_RX_PIN);

    usart_base_config(DEBUG_USART, baud_rate);
}


/************************************************
函数名称 ： esp_usart_config
功    能 ： ESP8266串口 USART5 初始化
引    脚 ： TX PC6，RX PC7
*************************************************/
void esp_usart_config(uint32_t baud_rate)
{
    rcu_periph_clock_enable(ESP_USART_TX_RCU);
    rcu_periph_clock_enable(ESP_USART_RX_RCU);
    rcu_periph_clock_enable(ESP_USART_RCU);

    gpio_af_set(ESP_USART_TX_PORT, ESP_USART_AF, ESP_USART_TX_PIN);
    gpio_af_set(ESP_USART_RX_PORT, ESP_USART_AF, ESP_USART_RX_PIN);

    gpio_mode_set(ESP_USART_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ESP_USART_TX_PIN);
    gpio_mode_set(ESP_USART_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ESP_USART_RX_PIN);

    gpio_output_options_set(ESP_USART_TX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, ESP_USART_TX_PIN);
    gpio_output_options_set(ESP_USART_RX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, ESP_USART_RX_PIN);

    usart_base_config(ESP_USART, baud_rate);

    usart_interrupt_enable(ESP_USART, USART_INT_RBNE);
    NVIC_SetPriority(USART5_IRQn, 5);
		NVIC_EnableIRQ(USART5_IRQn);

}

/************************************************
函数名称 ： screen_usart_config
功    能 ： 串口屏 USART2 初始化
引    脚 ： TX PC10，RX PC11
*************************************************/
void screen_usart_config(uint32_t baud_rate)
{
    rcu_periph_clock_enable(SCREEN_USART_TX_RCU);
    rcu_periph_clock_enable(SCREEN_USART_RX_RCU);
    rcu_periph_clock_enable(SCREEN_USART_RCU);

    gpio_af_set(SCREEN_USART_TX_PORT, SCREEN_USART_AF, SCREEN_USART_TX_PIN);
    gpio_af_set(SCREEN_USART_RX_PORT, SCREEN_USART_AF, SCREEN_USART_RX_PIN);

    gpio_mode_set(SCREEN_USART_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, SCREEN_USART_TX_PIN);
    gpio_mode_set(SCREEN_USART_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, SCREEN_USART_RX_PIN);

    gpio_output_options_set(SCREEN_USART_TX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SCREEN_USART_TX_PIN);
    gpio_output_options_set(SCREEN_USART_RX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SCREEN_USART_RX_PIN);

    usart_base_config(SCREEN_USART, baud_rate);
}


/************************************************
函数名称 ： usart_all_config
功    能 ： 三个串口统一初始化
*************************************************/
void usart_all_config(uint32_t debug_baud,
                      uint32_t esp_baud,
                      uint32_t screen_baud)
{
    debug_usart_config(debug_baud);
    esp_usart_config(esp_baud);
    screen_usart_config(screen_baud);
}


/************************************************
函数名称 ： debug_usart_send_data
功    能 ： Debug串口发送一个字节
*************************************************/
void debug_usart_send_data(uint8_t data)
{
    usart_data_transmit(DEBUG_USART, data);
    while(RESET == usart_flag_get(DEBUG_USART, USART_FLAG_TBE));
}


/************************************************
函数名称 ： esp_usart_send_data
功    能 ： ESP8266串口发送一个字节
*************************************************/
void esp_usart_send_data(uint8_t data)
{
    usart_data_transmit(ESP_USART, data);
    while(RESET == usart_flag_get(ESP_USART, USART_FLAG_TBE));
}


/************************************************
函数名称 ： screen_usart_send_data
功    能 ： 串口屏发送一个字节
*************************************************/
void screen_usart_send_data(uint8_t data)
{
    usart_data_transmit(SCREEN_USART, data);
    while(RESET == usart_flag_get(SCREEN_USART, USART_FLAG_TBE));
}


/************************************************
函数名称 ： debug_usart_send_string
功    能 ： Debug串口发送字符串
*************************************************/
void debug_usart_send_string(const char *str)
{
    while(str && *str)
    {
        debug_usart_send_data((uint8_t)(*str++));
    }
}


/************************************************
函数名称 ： esp_usart_send_string
功    能 ： ESP8266串口发送字符串
*************************************************/
void esp_usart_send_string(const char *str)
{
    while(str && *str)
    {
        esp_usart_send_data((uint8_t)(*str++));
    }
}


/************************************************
函数名称 ： screen_usart_send_string
功    能 ： 串口屏发送字符串
*************************************************/
void screen_usart_send_string(const char *str)
{
    while(str && *str)
    {
        screen_usart_send_data((uint8_t)(*str++));
    }
}


/************************************************
函数名称 ： debug_printf
功    能 ： Debug串口格式化打印
*************************************************/
void debug_printf(const char *fmt, ...)
{
    char buffer[USART_PRINTF_BUF_SIZE];
    va_list args;

    va_start(args, fmt);
    vsnprintf(buffer, USART_PRINTF_BUF_SIZE, fmt, args);
    va_end(args);

    debug_usart_send_string(buffer);
}


/************************************************
函数名称 ： esp_printf
功    能 ： ESP8266串口格式化打印
*************************************************/
void esp_printf(const char *fmt, ...)
{
    char buffer[USART_PRINTF_BUF_SIZE];
    va_list args;

    va_start(args, fmt);
    vsnprintf(buffer, USART_PRINTF_BUF_SIZE, fmt, args);
    va_end(args);

    esp_usart_send_string(buffer);
}


/************************************************
函数名称 ： screen_printf
功    能 ： 串口屏格式化打印
*************************************************/
void screen_printf(const char *fmt, ...)
{
    char buffer[USART_PRINTF_BUF_SIZE];
    va_list args;

    va_start(args, fmt);
    vsnprintf(buffer, USART_PRINTF_BUF_SIZE, fmt, args);
    va_end(args);

    screen_usart_send_string(buffer);
}


/************************************************
函数名称 ： fputc
功    能 ： printf重定向到Debug串口 USART0
*************************************************/
int fputc(int ch, FILE *f)
{
    debug_usart_send_data((uint8_t)ch);
    return ch;
}


