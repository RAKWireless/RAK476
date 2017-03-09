#include "bsp_driver.h"
#include "bsp.h"
#include "stdbool.h"

#define   RXBUFSIZE          2048
uint8_t   rxBuffer[RXBUFSIZE] = {0};
volatile uint16_t  rxWriteIndex =0;
volatile uint16_t  rxReadIndex =0;
volatile uint16_t  rxCount = 0;

#if   defined ( __CC_ARM )
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
    USART_SendData(PRINT_USART, (uint16_t) ch);

    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(PRINT_USART, USART_FLAG_TC) == RESET)
    {};

    return ch;
}
#endif

void USART2_IRQHandler(void)
{
    uint8_t  temp=0;
		
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        temp = ( uint8_t )USART_ReceiveData(USART2);
        rxBuffer[rxWriteIndex] =temp;
        rxWriteIndex++ ;
        rxCount++;
        if(rxWriteIndex == RXBUFSIZE)
        {
            rxWriteIndex = 0;
        }
        /* Check for overflow */
        if (rxCount == RXBUFSIZE)
        {
            rxWriteIndex = 0;
            rxCount      = 0;
            rxReadIndex  = 0;
            printf("out of buffer, need increase buffer size!!!");
        }
    }

}

/**
 *  brief.			initialize wifi reset pin
 * @param[in]		none
 * @param[out]	none
 * @return			none
 */
void Wifi_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(WIFI_PWD_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = WIFI_PWD_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(WIFI_PWD_GPIO_PORT, &GPIO_InitStructure);
}

/**
 *  brief.			initialize print uart
 * @param[in]		none
 * @param[out]	none
 * @return			none
 */
void Print_UART_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(PRINT_USART_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(PRINT_USART_TX_GPIO_CLK | PRINT_USART_RX_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = PRINT_USART_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PRINT_USART_TX_GPIO_PORT,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = PRINT_USART_RX_PIN;
    GPIO_Init(PRINT_USART_RX_GPIO_PORT, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(PRINT_USART, &USART_InitStructure);
    USART_Cmd(PRINT_USART, ENABLE);

}

/**
 *  brief.			initialize wifi uart
 * @param[in]		none
 * @param[out]	none
 * @return			none
 */
void  Wifi_UART_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(WIFI_USART_TX_GPIO_CLK | WIFI_USART_RX_GPIO_CLK, ENABLE);
    RCC_APB1PeriphClockCmd(WIFI_USART_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = WIFI_USART_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(WIFI_USART_TX_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = WIFI_USART_RX_PIN;
    GPIO_Init(WIFI_USART_RX_GPIO_PORT, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(WIFI_USART, &USART_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_ITConfig(WIFI_USART, USART_IT_RXNE, ENABLE);
    USART_Cmd(WIFI_USART, ENABLE);
}

int rw_uart_rx_byte(void)
{
    int c = -1;

    __disable_irq();
    if (rxCount > 0)
    {
        c = rxBuffer[rxReadIndex];
        rxReadIndex++;
        if (rxReadIndex == RXBUFSIZE)
        {
            rxReadIndex = 0;
        }
        rxCount--;
    }
    __enable_irq();

    return c;
}

uint8_t rw_uart_send(char *tx_buf,uint16_t buflen)
{
    uint16_t i;

    for (i=0; i<buflen; i++) {
        while((USART2->SR&0X40)==0);
        USART2->DR = (u8) tx_buf[i];
    }
    return 0;
}

void rw_reset_target(void)
{
    GPIO_WriteBit(WIFI_PWD_GPIO_PORT,WIFI_PWD_PIN, Bit_RESET);
    delay_ms(50);
    GPIO_WriteBit(WIFI_PWD_GPIO_PORT,WIFI_PWD_PIN, Bit_SET);
}

void host_platformInit(void)
{
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock/1000);
    Print_UART_Init();
    Wifi_GPIO_Init();
    Wifi_UART_Init();
}


void delay_ms(int count)
{
    int time =HAL_GetTick() + count;
    while(HAL_GetTick()<time);
}

rw_stamp_t get_stamp(void)
{
    return HAL_GetTick();
}

bool is_stamp_passed(rw_stamp_t* stamp) {
    return (get_stamp() > (*stamp));
}

