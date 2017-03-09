/**
* @file 		rak473_driver.h
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
#ifndef _RAK473_DRIVER_H_
#define _RAK473_DRIVER_H_

#include "stdint.h"
#include "string.h"
#include "rak_global.h"
#include "rak_config.h"
#include "bsp_driver.h"

#define MODE_INTERRUPT
//#define MODE_QUERY

#define FD_INVALID									-1

#define CMD_ERR_OK									0
#define CMD_ERR_NET_OK							1
#define CMD_ERR_PROMISC_OK					2
#define CMD_ERR_STOP_PROMISC_OK			3

#define CMD_ERR_PEND								-128
#define CMD_ERR_TIMEOUT							-127
#define CMD_ERR_SOCK_INVALID				-2
#define CMD_ERR_PARAM								-1
#define CMD_ERR_SYSBUSY							-10
#define CMD_ERR_MEM									-12							
#define CMD_ERR_UPGRADE							-14

#define IS_CMD_BLOCK()									(driver_ctx.block == 1)
#define IS_SEND_BLOCK_SOCK(sockfd) 			(driver_ctx.send_block_socks & (1<<sockfd))
#define IS_READ_BLOCK_SOCK(sockfd) 			(driver_ctx.read_block_socks & (1<<sockfd))
#define IS_DATA_VALID_SOCK(sockfd) 			(driver_ctx.data_valid_socks & (1<<sockfd))
#define SET_CTX_CMD_TYPE(type)					do { driver_ctx.cmd_type = type; } while (0)
#define CLR_CTX_CMD_BLOCK()							do { driver_ctx.block = 0; } while(0)

/** CMD List */
typedef enum {
    CMD_NONE = 0,
    CMD_GET_WELCOME,
    CMD_SET_ASCII,
    CMD_SET_FUNCMAP,
    CMD_GET_MAC,
    CMD_SET_PSK,
    CMD_CONNECT_AP,
    CMD_CREATE_AP,
    CMD_IPDHCP,
    CMD_IPSTATIC,
    CMD_EASYCONFIG,
		CMD_WPS,
    CMD_STARTWEB,
    CMD_GETSTORECONFIG,
    CMD_AUTOCONNECT,
    CMD_GET_NET_STATUS,
    CMD_OPEN_SOCKET,
    CMD_SEND_DATA,
	
		CMD_SET_FILTER,
		CMD_START_PROMISC,
		CMD_STOP_PROMISC,
		
		CMD_NVM_WRITE,
    
		CMD_TYPE_1,
		
    CMD_READ_SOCK_STATUS,
    CMD_READ_SOCK,
		CMD_PROMISC_DATA,
		CMD_NVM_READ,
    CMD_ALL,
} rak_cmd_type_t;

/** CMD result List*/
typedef enum {
    RES_NONE = -1,
    RES_WELCOME = 0,
    RES_OK,
    RES_ERR,
    RES_UPGRADE,
    RES_TIMEOUT,
} rak_response_type_t;

typedef void(*conn_event_callback)(uint8_t rsp_code, uint8_t block, uint8_t socket_fd);

/** uart driver context */
typedef struct {
    rak_cmd_type_t cmd_type;			///< store current ATcommand type.
    uint32_t timestamp;						///< driver timestamp.
    uint8_t block;								///< ATcommand block flag.
    uint32_t send_block_socks;		///< each bit presents which socket is block in send ATcommand bit0-bit7 socket0-7.
    uint32_t read_block_socks;		///< each bit presents which socket is block in read ATcommand bit0-bit7 socket0-7.
    uint32_t data_valid_socks;		///< each bit presents which socket has received data bit0-bit7 socket0-7.
    uint32_t open_block_socks;		///< each bit presents which socket is block in create socket ATcommand bit0-bit7 socket0-7.
    rak_response_type_t res_data; ///< ATcommand response result.

    conn_event_callback conn_event;///< connect callback event.
} rak_uartDrvCtx;

/** basic api */
void rak_set_ascii(uint8_t mode);
void rak_query_mac(void);
void rak_set_psk(rak_uJoin *uJoinFrame);
void rak_connect_ap(rak_uJoin *uJoinFrame);
void rak_set_ipdhcp(rak_uIpparam *uIpparamFrame);
void rak_open_socket(uint8 socket_type, uint32 dest_ip, uint16 dest_port, uint16 local_port, uint8_t ssl_enable);
void rak_send_data(uint8 sock_fd, uint16 dest_port, uint32 dest_ip, uint16 send_len, uint8 *buf);
void rak_query_socket_status(uint8 sock_fd);
void rak_query_socket(uint8 sock_fd, uint16 send_len);
void rak_set_funcBitmap(uint32 func);
void rak_query_con_status(void);
void rak_create_ap(rak_uAp *uApFrame);
void rak_set_ipstatic(rak_uIpparam *uIpparamFrame);
void rak_easyconfig(void);
void rak_wps(void);
void rak_start_webconfig(void);
void rak_getstoreconfig(void);
void rak_start_autoconnect(void);
void rak_set_filter(rak_uFilter *uFilterFrame);
void rak_start_promisc(rak_uFilter *uFilterFrame);
void rak_stop_promisc(void);

/** advance api */
uint16 rak_init_struct(rak_api *ptrStrApi);
void read_data_from_module(void);
int rak_module_init(void);
int net_config(void);
int promisc_func(void);
int open_socket(uint8_t cmd_id, uint8_t socket_type, uint32_t dest_ip, uint16_t dest_port, uint16_t local_port, uint8_t ssl_enable);//0:TCP 1:LTCP 2:UDP 3:LUDP
int write_socket(int fd, uint16_t dest_port, uint32_t dest_ip, uint16_t send_len, uint8_t *buf);
int read_socket(int fd, uint8_t* buf, uint16_t maxlen, uint32_t *remoteip, uint16_t *remoteport);
int query_socket_status(int sock_fd);
int query_con_status(void);
int nvm_write(uint32_t addr, uint16_t len, char *data);
int nvm_read(uint32_t addr, uint16_t len, char *buf);

extern rak_uCmdRsp	 	uCmdRspFrame;
extern rak_uartDrvCtx driver_ctx;
extern rak_api      	rak_strApi;

#define     LOG_ERROR    0x01
#define     LOG_WARN     0x02
#define     LOG_INFO     0x04
#define     LOG_DEBUG    0x08
#define     LOG_TRACE    0x10

#define     RAK_DEBUG     LOG_ERROR | LOG_WARN | LOG_INFO | LOG_DEBUG | LOG_TRACE
#define     LOG_LEVEL_CHECK(level)      RAK_DEBUG & level

#ifdef RAK_DEBUG
static const char* clean_filename(const char* path)
{
    const char* filename = path + strlen(path);
    while(filename > path)
    {
        if(*filename == '/' || *filename == '\\')
        {
            return filename + 1;
        }
        filename--;
    }
    return path;
}
#endif

#ifdef RAK_DEBUG
//#define DPRINTF(fmt, args...) printf(fmt, ##args)
#define DPRINTF(level, fmt, args...)    if(LOG_LEVEL_CHECK(level)) {        \
                                            printf("\r\n%d  ""%s "":%u  ", get_stamp(), clean_filename(__FILE__), __LINE__); \
                                            printf(fmt, ##args);    \
                                        }
#else
#define DPRINTF(fmt, args...)
#endif

#endif
