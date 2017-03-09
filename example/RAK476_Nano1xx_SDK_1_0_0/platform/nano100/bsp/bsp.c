#include "bsp_driver.h"
#include "bsp.h"
#include "stdbool.h"



#define RXBUFSIZE       2048

volatile int rxReadIndex  = 0;
volatile int rxWriteIndex = 0;
volatile int rxCount = 0;
volatile uint8_t rxBuffer[RXBUFSIZE];
uint32_t __IO u32Timer0Cnt=0;

static void set_sys_clock(void);



#if   defined ( __CC_ARM )

#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
    UART_TxByte(PRINT_UART, ch);

    return ch;
}
#endif

static void set_sys_clock(void)
{
    S_SYS_CHIP_CLKCFG sChipClkCfg;

    uint32_t clksel = CLK->CLKSEL0 & CLK_CLKSEL0_HCLK_MASK;

    /* Init Chip clock source and IP clocks */
    memset(&sChipClkCfg, 0, sizeof(sChipClkCfg));
    sChipClkCfg.u32ChipClkEn = CLK_PWRCTL_HXT_EN;
    sChipClkCfg.u32PLLClkSrc = CLK_PLLCTL_PLLSRC_HXT;
    sChipClkCfg.u8PLLEnable  = ENABLE;   /* enable PLL */
    sChipClkCfg.ePLLInFreq   = E_SYS_PLLIN_12M;
    sChipClkCfg.ePLLOutFreq  = E_SYS_PLLOUT_120M;
    sChipClkCfg.u32HClkDiv   = HCLK_CLK_DIVIDER(2);    /* HCLK = 120/(2+1) = 40 MHz */
    sChipClkCfg.u32HClkSrc = CLK_CLKSEL0_HCLK_PLL;
    /* IPs clock setting */
    SYS_InitChipClock(&sChipClkCfg);

    /* Update CPU Clock Frequency */
    SystemCoreClockUpdate();

}

void SYS_Config(void)
{
    UNLOCKREG();
    set_sys_clock();
    LOCKREG();
}

void TMR0_IRQHandler(void)
{
    TIMER0->ISR = 3;
    u32Timer0Cnt++;
}

void TIMER_Tick_Config(void)
{
    TIMER_Init(TIMER0, 11, 1000, TIMER_CTL_MODESEL_PERIODIC);
    /* Enable TIMER0 Intettupt */
    TIMER_EnableInt(TIMER0, TIMER_IER_TMRIE);
    /* Start counting */
    u32Timer0Cnt = 0;
    TIMER_Start(TIMER0);

}

void Print_UART_Init(void)
{
    STR_UART_T sParam = DEFAULT_VALUE;
    /* Set UART0 Pin */
    MFP_UART0_TO_PORTB();
    UART_Init(PRINT_UART, &sParam);
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

void UART1_IRQHandler(void)
{
    uint32_t uart_isr = UART1->ISR;

		if(uart_isr & DRVUART_RDAINT)
    {
        while(UART1->FSR & UART_FSR_RX_POINTER_F) {
            /* Store Data */
            rxBuffer[rxWriteIndex] = (UART1->RBR);
            rxWriteIndex++;
            rxCount++;
            if (rxWriteIndex == RXBUFSIZE)
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
}

void Wifi_UART_Init(void)
{
    STR_UART_T sParam = DEFAULT_VALUE;
    SYS_SelectIPClockSource_1(CLK_CLKSEL1_UART_MASK, CLK_CLKSEL1_UART_HXT);
    /* Set UART1 Pin */
    MFP_UART1_TO_PORTA();
		GPIO_EnablePullup(GPIOA, 2);
    UART_Init(WIFI_UART, &sParam);
    UART_EnableInt(WIFI_UART,  DRVUART_RLSINT | DRVUART_RDAINT | DRVUART_BUFERRINT);
}

void Wifi_GPIO_Init(void)
{
    GPIO_SetBit(WIFI_PORT_PWD, WIFI_PIN_PWD);
    GPIO_Open(WIFI_PORT_PWD, GPIO_PMD_PMD5_OUTPUT, GPIO_PMD_PMD5_MASK);
}

void rw_reset_target(void)
{
    GPIO_ClrBit(WIFI_PORT_PWD,WIFI_PIN_PWD);
    delay_ms(50);
    GPIO_SetBit(WIFI_PORT_PWD,WIFI_PIN_PWD);
}

void host_platformInit(void)
{
    SYS_Config();
    Print_UART_Init();
    Wifi_GPIO_Init();
    Wifi_UART_Init();
    TIMER_Tick_Config();
}



int Print_UART_TxBuf(uint8_t *buffer, int nbytes)
{
    for (int i = 0; i < nbytes; i++)
        UART_TxByte(PRINT_UART, *buffer++);

    return nbytes;
}

void UART_TxByte(UART_TypeDef  *tUART, uint8_t data)
{
    while (!(tUART->FSR & UART_FSR_TX_EMPTY_F));
    tUART->THR = data;
}

uint8_t rw_uart_send(char *tx_buf,uint16_t buflen)
{
    uint16_t i;

    for (i=0; i<buflen; i++) {
        UART_TxByte(WIFI_UART, *tx_buf++);
    }
    return 0;
}

void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while(1);

}


void delay_ms(int count)
{
    for (int i = 0; i < count; i++) {
        SYS_Delay(1000);
    }
}

rw_stamp_t get_stamp(void)
{
    return (rw_stamp_t)u32Timer0Cnt;
}

bool is_stamp_passed(rw_stamp_t* stamp) {
//    return ((get_stamp() - (*stamp)) > 0);
    return (get_stamp() > (*stamp));
}


