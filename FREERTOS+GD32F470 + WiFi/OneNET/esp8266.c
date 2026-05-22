#include "gd32f4xx.h"
#include "esp8266.h"

#include "FreeRTOS.h"
#include "task.h"

#include "bsp_usart.h"

#include <string.h>
#include <stdio.h>


#define ESP8266_WIFI_INFO   "AT+CWJAP=\"WiFi名称\",\"WiFi密码\"\r\n"      ///填写自己WiFi名称和密码


unsigned char esp8266_buf[512];
unsigned short esp8266_cnt = 0;
unsigned short esp8266_cntPre = 0;


/************************************************
函数名称 ： ESP8266_Clear
功    能 ： 清空ESP8266接收缓存
*************************************************/
void ESP8266_Clear(void)
{
    memset(esp8266_buf, 0, sizeof(esp8266_buf));
    esp8266_cnt = 0;
    esp8266_cntPre = 0;
}


/************************************************
函数名称 ： ESP8266_WaitRecive
功    能 ： 等待ESP8266接收完成
返 回 值 ： REV_OK-接收完成
           REV_WAIT-接收未完成
*************************************************/
_Bool ESP8266_WaitRecive(void)
{
    if(esp8266_cnt == 0)
    {
        return REV_WAIT;
    }

    if(esp8266_cnt == esp8266_cntPre)
    {
        esp8266_cnt = 0;
        return REV_OK;
    }

    esp8266_cntPre = esp8266_cnt;

    return REV_WAIT;
}


/************************************************
函数名称 ： ESP8266_SendCmd
功    能 ： 向ESP8266发送AT指令
参    数 ： cmd：AT指令
           res：期望返回关键字
返 回 值 ： 0-成功
           1-失败
*************************************************/
_Bool ESP8266_SendCmd(char *cmd, char *res)
{
    unsigned char timeOut = 200;

    ESP8266_Clear();

    esp_usart_send_string(cmd);

    while(timeOut--)
    {
        if(ESP8266_WaitRecive() == REV_OK)
        {
            if(strstr((const char *)esp8266_buf, res) != NULL)
            {
                ESP8266_Clear();
                return 0;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }

    return 1;
}


/************************************************
函数名称 ： ESP8266_SendData
功    能 ： 向ESP8266发送数据
参    数 ： data：数据
           len ：数据长度
*************************************************/
void ESP8266_SendData(unsigned char *data, unsigned short len)
{
    char cmdBuf[32];

    ESP8266_Clear();

    sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);

    if(!ESP8266_SendCmd(cmdBuf, ">"))
    {
        for(unsigned short i = 0; i < len; i++)
        {
            esp_usart_send_data(data[i]);
        }
    }
}


/************************************************
函数名称 ： ESP8266_GetIPD
功    能 ： 获取ESP8266接收到的平台数据
参    数 ： timeOut：等待时间，单位约为10ms
返 回 值 ： 平台返回的数据指针
*************************************************/
unsigned char *ESP8266_GetIPD(unsigned short timeOut)
{
    char *ptrIPD = NULL;

    do
    {
        if(ESP8266_WaitRecive() == REV_OK)
        {
            ptrIPD = strstr((char *)esp8266_buf, "IPD,");

            if(ptrIPD != NULL)
            {
                ptrIPD = strchr(ptrIPD, ':');

                if(ptrIPD != NULL)
                {
                    ptrIPD++;
                    return (unsigned char *)(ptrIPD);
                }
                else
                {
                    return NULL;
                }
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10));

    } while(timeOut--);

    return NULL;
}


/************************************************
函数名称 ： ESP8266_Init
功    能 ： 初始化ESP8266
*************************************************/
void ESP8266_Init(void)
{
    ESP8266_Clear();

    vTaskDelay(pdMS_TO_TICKS(500));

    debug_printf("1. AT\r\n");
    while(ESP8266_SendCmd("AT\r\n", "OK"))
    {
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    debug_printf("2. CWMODE\r\n");
    while(ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK"))
    {
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    debug_printf("3. AT+CWDHCP\r\n");
    while(ESP8266_SendCmd("AT+CWDHCP=1,1\r\n", "OK"))
    {
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    debug_printf("4. CWJAP\r\n");
    while(ESP8266_SendCmd(ESP8266_WIFI_INFO, "GOT IP"))
    {
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    debug_printf("5. ESP8266 Init OK\r\n");
}


/************************************************
函数名称 ： USART5_IRQHandler
功    能 ： ESP8266串口USART5接收中断
说    明 ： ESP8266使用USART5，TX:PC6，RX:PC7
*************************************************/
void USART5_IRQHandler(void)
{
    if(RESET != usart_interrupt_flag_get(ESP_USART, USART_INT_FLAG_RBNE))
    {
        if(esp8266_cnt >= sizeof(esp8266_buf))
        {
            esp8266_cnt = 0;
        }

        esp8266_buf[esp8266_cnt++] = usart_data_receive(ESP_USART);

        usart_interrupt_flag_clear(ESP_USART, USART_INT_FLAG_RBNE);
    }
}


