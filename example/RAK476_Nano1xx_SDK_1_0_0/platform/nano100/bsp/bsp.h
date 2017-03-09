#ifndef  _BSP_H_
#define  _BSP_H_

#include "nano1xx.h"
#include "nano1xx_sys.h"
#include "nano1xx_uart.h"
#include "nano1xx_spi.h"
#include "nano1xx_gpio.h"
#include "nano1xx_pdma.h"
#include "nano1xx_fmc.h"
#include "nano1xx_timer.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define   DEBUG   

extern uint32_t __IO u32Timer0Cnt;

#define DEFAULT_VALUE         {\
                                  115200,\
                                  DRVUART_DATABITS_8,\
                                  DRVUART_STOPBITS_1,\
                                  DRVUART_PARITY_NONE,\
                                  DRVUART_FIFO_1BYTES,\
                                  DISABLE\
                              }

#define WIFI_PORT_PWD                           GPIOE
#define WIFI_PIN_PWD                            5


#define PRINT_UART                              UART0
#define WIFI_UART																UART1


static __INLINE void sys_soft_reset(void)
{
    NVIC_SystemReset();
}

static __INLINE void sys_hard_reset(void)
{
    GCR->IPRST_CTL1 |= GCR_IPRSTCTL1_CHIP;
    while(1);
}


void host_platformInit(void);
void WIFI_GPIO_Init(void);

void SYS_Config(void);
void UART_TxByte(UART_TypeDef  *tUART, uint8_t data);
int  Print_UART_TxBuf(uint8_t *buffer, int nbytes);
void Print_UART_Init(void);
void TIMER_Tick_Config(void);
int  UART_RxByte(void);

void delay_ms(int count);																				
																				
#endif
