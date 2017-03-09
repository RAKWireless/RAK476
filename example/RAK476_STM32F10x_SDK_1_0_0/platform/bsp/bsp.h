#ifndef __BSP_H
#define __BSP_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>

#define WIFI_PWD_GPIO_PORT             			 GPIOC
#define WIFI_PWD_PIN                   			 GPIO_Pin_2
#define WIFI_PWD_GPIO_CLK              			 RCC_APB2Periph_GPIOC

#define PRINT_USART                          USART1 
#define PRINT_USART_CLK                      RCC_APB2Periph_USART1
#define PRINT_USART_INIT                     RCC_APB2PeriphClockCmd

#define PRINT_USART_TX_PIN                   GPIO_Pin_9
#define PRINT_USART_TX_GPIO_PORT             GPIOA
#define PRINT_USART_TX_GPIO_CLK              RCC_APB2Periph_GPIOA
#define PRINT_USART_TX_SOURCE                GPIO_PinSource9
#define PRINT_USART_TX_AF                    GPIO_AF_USART1

#define PRINT_USART_RX_PIN                   GPIO_Pin_10
#define PRINT_USART_RX_GPIO_PORT             GPIOA
#define PRINT_USART_RX_GPIO_CLK              RCC_APB2Periph_GPIOA
#define PRINT_USART_RX_SOURCE                GPIO_PinSource10
#define PRINT_USART_RX_AF                    GPIO_AF_USART1


#define WIFI_USART                          USART2 
#define WIFI_USART_CLK                      RCC_APB1Periph_USART2
#define WIFI_USART_INIT                     RCC_APB1PeriphClockCmd

#define WIFI_USART_TX_PIN                   GPIO_Pin_2
#define WIFI_USART_TX_GPIO_PORT             GPIOA
#define WIFI_USART_TX_GPIO_CLK              RCC_APB2Periph_GPIOA
#define WIFI_USART_TX_SOURCE                GPIO_PinSource2
#define WIFI_USART_TX_AF                    GPIO_AF_USART2

#define WIFI_USART_RX_PIN                   GPIO_Pin_3
#define WIFI_USART_RX_GPIO_PORT             GPIOA
#define WIFI_USART_RX_GPIO_CLK              RCC_APB2Periph_GPIOA
#define WIFI_USART_RX_SOURCE                GPIO_PinSource3
#define WIFI_USART_RX_AF                    GPIO_AF_USART2


void host_platformInit(void);
uint32_t HAL_GetTick(void);

#endif //__BSP_H
