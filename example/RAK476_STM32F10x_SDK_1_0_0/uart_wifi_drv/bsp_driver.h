/**
* @file 		bsp_driver.h
* @brief		this is a uart driver for ATcommand wifi.
* @details	This is the detail description.
* @author		harry
* @date			2016/1/23
* @version	1.0.0
* @par Copyright (c):
* 					rakwireless
* @par History:
*	version: author, date, desc\n
*/
#ifndef __BSP_DRIVER_H__
#define __BSP_DRIVER_H__

#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"

typedef uint32_t rw_stamp_t;

void 				rw_reset_target(void);
int 				rw_uart_rx_byte(void);
uint8_t 		rw_uart_send(char *tx_buf,uint16_t buflen);
void 				delay_ms(int count);
rw_stamp_t 	get_stamp(void);
bool 				is_stamp_passed(rw_stamp_t* stamp);
#endif
