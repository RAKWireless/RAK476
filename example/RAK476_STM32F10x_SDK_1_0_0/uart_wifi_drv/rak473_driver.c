
/**
* @file 		rak473_driver.c
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
#include "rak473_driver.h"

#define htons(n) 				(((n & 0xff) << 8) | ((n & 0xff00) >> 8))
#define htonl(n) 				(((n & 0xff) << 24) | ((n & 0xff00) << 8) | ((n & 0xff0000UL) >> 8) | ((n & 0xff000000UL) >> 24))
#define ntohs(n) 				htons(n)
#define ntohl(n) 				htonl(n)

#define add_stamp(stamp, msec)    do { *(stamp) += msec; } while (0)
#define set_future_stamp(stamp, msec) do { *(stamp) = get_stamp(); add_stamp(stamp, msec); } while (0)
/** set a stamp for cmd timeout*/
#define SET_CTX_STAMP(intv) do { set_future_stamp(&driver_ctx.timestamp, (intv)); } while (0)
/** store current cmd type */
#define SET_CTX_CMD_TYPE(type)	do { driver_ctx.cmd_type = type; } while (0)
/** set cmd block status */
#define SET_CTX_CMD_BLOCK()		do { driver_ctx.block = 1; } while(0)
/** clr cmd block status */
#define CLR_CTX_CMD_BLOCK()		do { driver_ctx.block = 0; } while(0)
/** judge if cmd is block */
#define IS_CMD_BLOCK()				(driver_ctx.block == 1)

/** set a socket send block status */
#define SET_CTX_SEND_BLOCK_SOCK(sockfd) 	do { driver_ctx.send_block_socks |= (1<<sockfd); } while (0)
/** clr a socket send block status */
#define CLR_CTX_SEND_BLOCK_SOCK(sockfd) 	do { driver_ctx.send_block_socks &= ~(1<<sockfd); } while (0)
/** judge if a socket is block in send*/
#define IS_SEND_BLOCK_SOCK(sockfd) 			(driver_ctx.send_block_socks & (1<<sockfd))

/** set a socket read block status */
#define SET_CTX_READ_BLOCK_SOCK(sockfd) 	do { driver_ctx.read_block_socks |= (1<<sockfd); } while (0)
/** clr a socket read block status */
#define CLR_CTX_READ_BLOCK_SOCK(sockfd) 	do { driver_ctx.read_block_socks &= ~(1<<sockfd); } while (0)
/** judge if a socket is block in read */
#define IS_READ_BLOCK_SOCK(sockfd) 			(driver_ctx.read_block_socks & (1<<sockfd))

/** set a socket have data status */
#define SET_CTX_DATA_VALID_SOCK(sockfd) 	do { driver_ctx.data_valid_socks |= (1<<sockfd); } while (0)
/** clr a socket have data status */
#define CLR_CTX_DATA_VALID_SOCK(sockfd) 	do { driver_ctx.data_valid_socks &= ~(1<<sockfd); } while (0)
/** judge if a socket have data */
#define IS_DATA_VALID_SOCK(sockfd) 			(driver_ctx.data_valid_socks & (1<<sockfd))

/** set a socket open block status */
#define SET_CTX_OPEN_BLOCK_SOCK(sockid) 	do { driver_ctx.open_block_socks |= (1<<sockid); } while (0)
/** clr a socket open block status */
#define CLR_CTX_OPEN_BLOCK_SOCK(sockid) 	do { driver_ctx.open_block_socks &= ~(1<<sockid); } while (0)
/** judge if a socket is block in open */
#define IS_OPEN_BLOCK_SOCK(sockid) 			(driver_ctx.open_block_socks & (1<<sockid))

rak_uartDrvCtx 		driver_ctx;		///< driver_ctx.
rak_api      	rak_strApi;			///< store wifi's config param.
rak_uCmdRsp	 	uCmdRspFrame;		///< store wifi's uart response data.
static int 		read_index;			///< the count which have been parsed by driver.
static char print_buf[300];

/**
 *  brief.			set all ATcommand's return as ascii or hex to display
 * @param[in]		uint8_t mode -- 0 hex, 1 ascii
 * @param[out]	none
 * @return			none
 */
void rak_set_ascii(uint8_t mode)
{
    char cmd[50];
    sprintf(cmd, "at+ascii=%d\r\n", mode);
    rw_uart_send(cmd,strlen(cmd));
    DPRINTF(LOG_INFO, "%s", cmd);
    SET_CTX_CMD_TYPE(CMD_SET_ASCII);
    SET_CTX_STAMP(RAK_SETASCII_TIMEOUT);
    SET_CTX_CMD_BLOCK();

}
/**
 *  brief.			query module's mac address
 * @param[in]		none
 * @param[out]	none
 * @return			none
 */
void rak_query_mac(void)
{
    char cmd[50];
    sprintf(cmd, "at+mac\r\n");
    rw_uart_send(cmd, strlen(cmd));
    DPRINTF(LOG_INFO, "%s", cmd);
    SET_CTX_CMD_TYPE(CMD_GET_MAC);
    SET_CTX_STAMP(RAK_GETMAC_TIMEOUT);
    SET_CTX_CMD_BLOCK();
}
/**
 *  brief.			set psk
 * @param[in]		rak_uJoin *uJoinFrame -- use rak_uJoin's member psk
 * @param[out]	none
 * @return			none
 */
void rak_set_psk(rak_uJoin *uJoinFrame)
{
    char cmd[100];
    sprintf(cmd, "at+psk=%s\r\n", (char *)uJoinFrame->joinFrameSnd.psk);
    rw_uart_send(cmd,strlen(cmd));
    DPRINTF(LOG_INFO, "%s", cmd);
    SET_CTX_CMD_TYPE(CMD_SET_PSK);
    SET_CTX_STAMP(RAK_SETPSK_TIMEOUT);
    SET_CTX_CMD_BLOCK();
}
/**
 *  brief.			connect to ap
 * @param[in]		rak_uJoin *uJoinFrame -- use rak_uJoin's member ssid
 * @param[out]	none
 * @return			none
 */
void rak_connect_ap(rak_uJoin *uJoinFrame)
{
    char cmd[100];
    sprintf(cmd, "at+connect=%s\r\n", (char *)uJoinFrame->joinFrameSnd.ssid);
    rw_uart_send(cmd,strlen(cmd));
    DPRINTF(LOG_INFO, "%s", cmd);
    SET_CTX_CMD_TYPE(CMD_CONNECT_AP);
    SET_CTX_STAMP(RAK_CONNECT_TIMEOUT);
    SET_CTX_CMD_BLOCK();
}
/**
 *  brief.			set dhcp
 * @param[in]		rak_uIpparam *uIpparamFrame -- use rak_uIpparam's member dhcpMode
 *							0--dhcp client 1--dhcp server
 * @param[out]	none
 * @return			none
 */
void rak_set_ipdhcp(rak_uIpparam *uIpparamFrame)
{
    char cmd[50];
    sprintf(cmd, "at+ipdhcp=%d\r\n", uIpparamFrame->ipparamFrameSnd.dhcpMode);
    rw_uart_send(cmd,strlen(cmd));
    DPRINTF(LOG_INFO, "%s", cmd);
    SET_CTX_CMD_TYPE(CMD_IPDHCP);
    SET_CTX_STAMP(RAK_DHCP_TIMEOUT);
    SET_CTX_CMD_BLOCK();
}
/**
 *  brief.			open socket
 * @param[in]		uint8 socket_type -- 0:TCP 1:LTCP 2:UDP 3:LUDP
 * @param[in]		uint32 dest_ip
 * @param[in]		uint16 dest_port
 * @param[in]		uint16 local_port
 * @param[in]		uint8_t ssl_enable -- 0:ssl disable 1:ssl enable
 * @param[out]	none
 * @return			none
 */
void rak_open_socket(uint8 socket_type, uint32 dest_ip, uint16 dest_port, uint16 local_port, uint8_t ssl_enable)
{
    char cmd[50]="";
    uint8_t ip_buf[16];

    dest_ip = htonl(dest_ip);
    switch (socket_type)
    {
    case 0x00:
        rak_bytes4ToAsciiDotAddr((uint8_t *)&dest_ip, ip_buf);
        if(!ssl_enable) {
            sprintf(cmd, "at+tcp=%s,%d,%d\r\n", ip_buf, dest_port, local_port);
        } else {
            sprintf(cmd, "at+tcp=%s,%d,%d,1\r\n", ip_buf, dest_port, local_port);
        }
        break;

    case 0x01:
        sprintf(cmd, "at+ltcp=%d\r\n", local_port);
        break;

    case 0x02:
        rak_bytes4ToAsciiDotAddr((uint8_t *)&dest_ip, ip_buf);
        sprintf(cmd, "at+udp=%s,%d,%d\r\n", ip_buf, dest_port, local_port);
        break;

    case 0x03:
        sprintf(cmd, "at+ludp=%d\r\n", local_port);
        break;

    case 0x04:
        rak_bytes4ToAsciiDotAddr((uint8_t *)&dest_ip, ip_buf);
        sprintf(cmd, "at+multicast=%s,%d,%d\r\n", ip_buf, dest_port, local_port);
        break;
    }
    rw_uart_send((char *)cmd,strlen(cmd));
    DPRINTF(LOG_INFO, "%s", cmd);
    SET_CTX_CMD_TYPE(CMD_OPEN_SOCKET);
    SET_CTX_STAMP(RAK_OPENSOCKET_TIMEOUT);
    SET_CTX_CMD_BLOCK();
}

/**
 *  brief.			send data via socket
 * @param[in]		sock_fd
 * @param[in]		dest_port
 * @param[in]		dest_ip
 * @param[in]		send_len
 * @param[in]		buf	-- store data to send
 * @param[out]	none
 * @return			none
 */
void rak_send_data(uint8 sock_fd, uint16 dest_port, uint32 dest_ip, uint16 send_len, uint8 *buf)
{
    char cmd[100]="";
    uint8_t ip_buf[16];

    if(dest_ip != 0) {
        dest_ip = htonl(dest_ip);
        rak_bytes4ToAsciiDotAddr((uint8_t *)&dest_ip, ip_buf);
        sprintf(cmd,"at+send_data=%d,%d,%s,%d,", sock_fd, dest_port, ip_buf, send_len);
    } else {
        sprintf(cmd,"at+send_data=%d,%d,%d,%d,", sock_fd, dest_port, dest_ip, send_len);
    }
    rw_uart_send(cmd, strlen(cmd));
    rw_uart_send((char *)buf, send_len);
    rw_uart_send("\r\n",2);
    DPRINTF(LOG_INFO, "%s", cmd);
    SET_CTX_CMD_TYPE(CMD_SEND_DATA);
    SET_CTX_STAMP(RAK_SENDATA_TIMEOUT);
    SET_CTX_CMD_BLOCK();
}
/**
 *  brief.			query socket's status
 * @param[in]		sock_fd
 * @param[out]	none
 * @return			none
 */
void rak_query_socket_status(uint8 sock_fd)
{
    char cmd[100]="";

    sprintf(cmd, "at+socket_status=%d\r\n", sock_fd);
    rw_uart_send(cmd, strlen(cmd));
    DPRINTF(LOG_INFO, "%s", cmd);
    SET_CTX_CMD_TYPE(CMD_READ_SOCK_STATUS);
    SET_CTX_STAMP(RAK_SOCKSTATUS_TIMEOUT);
    SET_CTX_CMD_BLOCK();
}

/**
 *  brief.			query mode read data from socket
 * @param[in]		sock_fd
 * @param[in]		read_len
 * @param[out]	none
 * @return			none
 */
void rak_query_socket(uint8 sock_fd, uint16 read_len)
{
    char cmd[100]="";

    sprintf(cmd, "at+read=%d,%d\r\n", sock_fd, read_len);
    rw_uart_send(cmd, strlen(cmd));
    DPRINTF(LOG_INFO, "%s", cmd);
    SET_CTX_CMD_TYPE(CMD_READ_SOCK);
    SET_CTX_STAMP(RAK_SOCKDATA_TIMEOUT);
    SET_CTX_CMD_BLOCK();
}
/**
 *  brief.			set special funciton bit
 * @param[in]		func
 * @param[out]	none
 * @return			none
 */
void rak_set_funcBitmap(uint32 func)
{
    char cmd[100]="";

    sprintf(cmd, "at+set_funcbitmap=%d\r\n", func);
    rw_uart_send(cmd, strlen(cmd));
    DPRINTF(LOG_INFO, "%s", cmd);
    SET_CTX_CMD_TYPE(CMD_SET_FUNCMAP);
    SET_CTX_STAMP(RAK_SETFUNCBITMAP_TIMEOUT);
    SET_CTX_CMD_BLOCK();
}
/**
 *  brief.			query net status
 * @param[in]		none
 * @param[out]	none
 * @return			none
 */
void rak_query_con_status(void)
{
    char cmd[100]="";

    sprintf(cmd, "at+con_status\r\n");
    rw_uart_send(cmd, strlen(cmd));
    DPRINTF(LOG_INFO, "%s", cmd);
    SET_CTX_CMD_TYPE(CMD_GET_NET_STATUS);
    SET_CTX_STAMP(RAK_GETNET_TIMEOUT);
    SET_CTX_CMD_BLOCK();
}

/**
 *  brief.			create ap
 * @param[in]		uApFrame -- specify ap's ssid
 * @param[out]	none
 * @return			none
 */
void rak_create_ap(rak_uAp *uApFrame)
{
    char cmd[100]="";
    sprintf(cmd, "at+ap=%s\r\n", uApFrame->apFrameSnd.ssid);
    rw_uart_send(cmd,strlen(cmd));
    DPRINTF(LOG_INFO, "%s", cmd);
    SET_CTX_CMD_TYPE(CMD_CREATE_AP);
    SET_CTX_STAMP(RAK_CONNECT_TIMEOUT);
    SET_CTX_CMD_BLOCK();
}
/**
 *  brief.			set module's static ip
 * @param[in]		uIpparamFrame -- specify module's ip info
 * @param[out]	none
 * @return			none
 */
void rak_set_ipstatic(rak_uIpparam *uIpparamFrame)
{
    char cmd[100]="";
    uint8_t ip_buf[16];

    sprintf(cmd, "at+ipstatic=");
    rak_bytes4ToAsciiDotAddr(uIpparamFrame->ipparamFrameSnd.ipaddr,ip_buf);
    strcat(cmd, (char *)ip_buf);
    strcat(cmd, ",");
    rak_bytes4ToAsciiDotAddr(uIpparamFrame->ipparamFrameSnd.netmask,ip_buf);
    strcat(cmd, (char *)ip_buf);
    strcat(cmd, ",");
    rak_bytes4ToAsciiDotAddr(uIpparamFrame->ipparamFrameSnd.gateway,ip_buf);
    strcat(cmd, (char *)ip_buf);
    strcat(cmd, ",");
    rak_bytes4ToAsciiDotAddr(uIpparamFrame->ipparamFrameSnd.dnssvr1,ip_buf);
    strcat(cmd, (char *)ip_buf);
    strcat(cmd, ",");
    rak_bytes4ToAsciiDotAddr(uIpparamFrame->ipparamFrameSnd.dnssvr2,ip_buf);
    strcat(cmd, (char *)ip_buf);
    strcat(cmd, "\r\n");
    rw_uart_send((char *)cmd,strlen(cmd));
    DPRINTF(LOG_INFO, "%s", cmd);
    SET_CTX_CMD_TYPE(CMD_IPSTATIC);
    SET_CTX_STAMP(RAK_IPSTATIC_TIMEOUT);
    SET_CTX_CMD_BLOCK();
}
/**
 *  brief.			easy config
 * @param[in]		none
 * @param[out]	none
 * @return			none
 */
void rak_easyconfig(void)
{
    char cmd[20]="";
    sprintf(cmd, "at+easy_config\r\n");
    rw_uart_send(cmd,strlen(cmd));
    DPRINTF(LOG_INFO, "%s", cmd);
    SET_CTX_CMD_TYPE(CMD_EASYCONFIG);
    SET_CTX_STAMP(RAK_EASYCONFIG_TIMEOUT);
    SET_CTX_CMD_BLOCK();

}

/**
 *  brief.			wps
 * @param[in]		none
 * @param[out]	none
 * @return			none
 */
void rak_wps(void)
{
    char cmd[20]="";
    sprintf(cmd, "at+wps\r\n");
    rw_uart_send(cmd,strlen(cmd));
    DPRINTF(LOG_INFO, "%s", cmd);
    SET_CTX_CMD_TYPE(CMD_WPS);
    SET_CTX_STAMP(RAK_WPS_TIMEOUT);
    SET_CTX_CMD_BLOCK();

}

/**
 *  brief.			webconfig
 * @param[in]		none
 * @param[out]	none
 * @return			none
 */
void rak_start_webconfig(void)
{
    char cmd[20]="";
    sprintf(cmd, "at+start_web\r\n");
    rw_uart_send(cmd,strlen(cmd));
    DPRINTF(LOG_INFO, "%s", cmd);
    SET_CTX_CMD_TYPE(CMD_STARTWEB);
    SET_CTX_STAMP(RAK_STARTWEB_TIMEOUT);
    SET_CTX_CMD_BLOCK();
}

/**
 *  brief.			get_storecofnig
 * @param[in]		none
 * @param[out]	none
 * @return			none
 */
void rak_getstoreconfig(void)
{
    char cmd[30]="";
    sprintf(cmd, "at+get_storeconfig\r\n");
    rw_uart_send(cmd,strlen(cmd));
    DPRINTF(LOG_INFO, "%s", cmd);
    SET_CTX_CMD_TYPE(CMD_GETSTORECONFIG);
    SET_CTX_STAMP(RAK_GETSTORECONFIG_TIMEOUT);
    SET_CTX_CMD_BLOCK();
}

/**
 *  brief.			auto connect
 * @param[in]		none
 * @param[out]	none
 * @return			none
 */
void rak_start_autoconnect(void)
{
    char cmd[20]="";
    sprintf(cmd, "at+auto_connect\r\n");
    rw_uart_send(cmd,strlen(cmd));
    DPRINTF(LOG_INFO, "%s", cmd);
    SET_CTX_CMD_TYPE(CMD_AUTOCONNECT);
    SET_CTX_STAMP(RAK_AUTOCONNECT_TIMEOUT);
    SET_CTX_CMD_BLOCK();
}

/**
 *  brief.			set filter -- promisc api
 * @param[in]		none
 * @param[out]	none
 * @return			none
 */
void rak_set_filter(rak_uFilter *uFilterFrame)
{
    char cmd[100]="";

    if(uFilterFrame->filterFrameSnd.src_mac[0] == '\0') {
        uFilterFrame->filterFrameSnd.src_mac[0] = '0';
        uFilterFrame->filterFrameSnd.src_mac[1] = '\0';
    }

    if(uFilterFrame->filterFrameSnd.dst_mac[0] == '\0') {
        uFilterFrame->filterFrameSnd.dst_mac[0] = '0';
        uFilterFrame->filterFrameSnd.dst_mac[1] = '\0';
    }

    sprintf(cmd, "at+set_filter=%s,%s,%d\r\n",uFilterFrame->filterFrameSnd.src_mac,
            uFilterFrame->filterFrameSnd.dst_mac, uFilterFrame->filterFrameSnd.rssi);
    rw_uart_send(cmd,strlen(cmd));
    DPRINTF(LOG_INFO, "%s", cmd);
    SET_CTX_CMD_TYPE(CMD_SET_FILTER);
    SET_CTX_STAMP(RAK_SETFILTER_TIMEOUT);
    SET_CTX_CMD_BLOCK();
}

/**
 *  brief.			set filter -- promisc api
 * @param[in]		none
 * @param[out]	none
 * @return			none
 */
void rak_start_promisc(rak_uFilter *uFilterFrame)
{
    char cmd[20]="";
    sprintf(cmd, "at+start_promisc=%d\r\n", uFilterFrame->filterFrameSnd.channel);
    rw_uart_send(cmd,strlen(cmd));
    DPRINTF(LOG_INFO, "%s", cmd);
    SET_CTX_CMD_TYPE(CMD_START_PROMISC);
    SET_CTX_STAMP(RAK_STARTPROMISC_TIMEOUT);
    SET_CTX_CMD_BLOCK();
}

/**
 *  brief.			stop promisc -- promisc api
 * @param[in]		none
 * @param[out]	none
 * @return			none
 */
void rak_stop_promisc(void)
{
    char cmd[20]="";
    sprintf(cmd, "at+stop_promisc\r\n");
    rw_uart_send(cmd,strlen(cmd));
    DPRINTF(LOG_INFO, "%s", cmd);
    SET_CTX_CMD_TYPE(CMD_STOP_PROMISC);
    SET_CTX_STAMP(RAK_STOPPROMISC_TIMEOUT);
    SET_CTX_CMD_BLOCK();
}

/**
 *  brief.			rak_nvm_write-- usr flash api
 * @param[in]		none
 * @param[out]	none
 * @return			none
 */
void rak_nvm_write(uint32_t addr, uint16_t len, char *data)
{
    char cmd[20]="";
    sprintf(cmd, "at+nvm_write=%d,%d,", addr, len);
    rw_uart_send(cmd,strlen(cmd));
    rw_uart_send(data, len);
    rw_uart_send("\r\n",2);
    DPRINTF(LOG_INFO, "%s", cmd);
    SET_CTX_CMD_TYPE(CMD_NVM_WRITE);
    SET_CTX_STAMP(RAK_NVMWRITE_TIMEOUT);
    SET_CTX_CMD_BLOCK();
}


/**
 *  brief.			rak_nvm_read -- usr flash api
 * @param[in]		none
 * @param[out]	none
 * @return			none
 */
void rak_nvm_read(uint32_t addr, uint16_t len)
{
    char cmd[20]="";
    sprintf(cmd, "at+nvm_read=%d,%d\r\n", addr, len);
    rw_uart_send(cmd,strlen(cmd));
    DPRINTF(LOG_INFO, "%s", cmd);
    SET_CTX_CMD_TYPE(CMD_NVM_READ);
    SET_CTX_STAMP(RAK_NVMREAD_TIMEOUT);
    SET_CTX_CMD_BLOCK();
}

/**
 *  brief.			read and parse data from wifi module one by one
 * @param[in]		none
 * @param[out]	none
 * @return			none
 */
void read_data_from_module(void)
{
    int c = -1;
    uint16_t       data_len = 0;
    int 					 recv_fd = 0;
    uint8_t        rspcode = 0;
    rak_recvdataFrame	*recv_data_frame;
    rak_nvmReadFrame *nvm_read_frame;

    if(driver_ctx.data_valid_socks != 0)  {//In isr mode,a socket(fdx) has received data but did not call read_socket(fdx...) to read data out
        recv_data_frame = (rak_recvdataFrame*)uCmdRspFrame.uCmdRspBuf;
        recv_fd = recv_data_frame->recvdataframe.socketDescriptor;
        data_len = (recv_data_frame->recvdataframe.recDataLen[1]*256 + recv_data_frame->recvdataframe.recDataLen[0]);
        DPRINTF(LOG_INFO, "----------valid socks=0x%x fd=%d len=%d", driver_ctx.data_valid_socks, recv_fd, data_len);
        return;
    }

    c = rw_uart_rx_byte();
    if(c >= 0) {
        uCmdRspFrame.uCmdRspBuf[read_index++] = c;
    }

    driver_ctx.res_data = RES_NONE;

    //parse pkt
    if(driver_ctx.cmd_type > CMD_NONE && driver_ctx.cmd_type <= CMD_TYPE_1 && read_index > 2) {
        if(uCmdRspFrame.uCmdRspBuf[read_index-2] == '\r' && uCmdRspFrame.uCmdRspBuf[read_index-1] == '\n') {
            rak_hexArrayToStr((char *)uCmdRspFrame.uCmdRspBuf, print_buf, read_index);
            DPRINTF(LOG_INFO, "%s", print_buf);
            if(0 == strncmp((char *)uCmdRspFrame.uCmdRspBuf, "ERROR",5)) {
                if(uCmdRspFrame.errorResponse.errorframe.errorCode == -14) {
                    DPRINTF(LOG_INFO, "wifi module is upgrading...");
                    driver_ctx.res_data = RES_UPGRADE;		//module is in upgrade status
                } else {
                    driver_ctx.res_data = RES_ERR;
                }
                read_index = 0;
            }
            else if(0 == strncmp((char *)uCmdRspFrame.uCmdRspBuf, "OK",2)) {
                driver_ctx.res_data = RES_OK;
                read_index = 0;
            }
        }
    }

#if defined MODE_INTERRUPT

    if( strncmp((char *)uCmdRspFrame.uCmdRspBuf, RAK_RECVDATA, strlen(RAK_RECVDATA)) == 0
            && read_index >= (strlen(RAK_RECVDATA) + 1)) {
        recv_data_frame = (rak_recvdataFrame*)uCmdRspFrame.uCmdRspBuf;
        rspcode =recv_data_frame->recvstatuframe.rspCode;
        if(rspcode & 0x80) {
            while(1) {
                c = rw_uart_rx_byte();
                if(c >= 0) {
                    uCmdRspFrame.uCmdRspBuf[read_index++] = c;
                    if(c == '\n') {
                        read_index = 0;
                        break;
                    }
                }
            }
            recv_fd = recv_data_frame->recvstatuframe.socketDescriptor;
            driver_ctx.conn_event(rspcode, driver_ctx.block, recv_fd);

        } else {
            if(read_index >= (strlen(RAK_RECVDATA)+1+2+4+2)) {
                recv_fd = recv_data_frame->recvdataframe.socketDescriptor;
                data_len = (recv_data_frame->recvdataframe.recDataLen[1]*256 + recv_data_frame->recvdataframe.recDataLen[0]);
                uint32_t start_time = get_stamp();
                while( read_index <(strlen(RAK_RECVDATA)+1+2+4+2+data_len+2)) {
                    c = rw_uart_rx_byte();
                    if(c >= 0) {
                        uCmdRspFrame.uCmdRspBuf[read_index++] = c;
                    }
                    if(get_stamp()-start_time > 1000) {
                        break;
                    }
                }
                read_index = 0;
                SET_CTX_DATA_VALID_SOCK(recv_fd);
            }
        }
    }
						
#elif defined MODE_QUERY
    rak_qrySocketStatusFrame * socket_status_frame;
    rak_readSocketFrame * read_socket_frame;
    int valid_num = 0;
    int sock_num = 0;

    if(driver_ctx.cmd_type == CMD_READ_SOCK_STATUS) {
        if(0 == strncmp((char *)uCmdRspFrame.uCmdRspBuf, "OK", 2) && read_index == 3) {
            socket_status_frame = (rak_qrySocketStatusFrame*)uCmdRspFrame.uCmdRspBuf;
            valid_num = socket_status_frame->qrySocketStatusFrame.valid_num;
            sock_num = (valid_num==0)?1:valid_num;
            while( read_index <(3+sock_num*(1+2+4)+2)) {
                c = rw_uart_rx_byte();
                if(c >= 0) {
                    uCmdRspFrame.uCmdRspBuf[read_index++] = c;
                }
            }
            read_index = 0;
            driver_ctx.res_data = RES_OK;
        }
    }

    if(driver_ctx.cmd_type == CMD_READ_SOCK) {
        if(0 == strncmp((char *)uCmdRspFrame.uCmdRspBuf, "OK", 2) && read_index == 4) {	//OK+len
            read_socket_frame = (rak_readSocketFrame*)uCmdRspFrame.uCmdRspBuf;
            data_len = read_socket_frame->readSocketFrame.recDataLen[1]*256+read_socket_frame->readSocketFrame.recDataLen[0];
            while( read_index <(data_len+6)) {
                c = rw_uart_rx_byte();
                if(c >= 0) {
                    uCmdRspFrame.uCmdRspBuf[read_index++] = c;
                }
            }
            read_index = 0;
            driver_ctx.res_data = RES_OK;
        }
        else if(0 == strncmp((char *)uCmdRspFrame.uCmdRspBuf, "ERROR",5) && read_index == 8) {
            read_index = 0;
            driver_ctx.res_data = RES_ERR;
        }
    }
#endif
		
		if(driver_ctx.cmd_type == CMD_PROMISC_DATA && read_index == 18 && uCmdRspFrame.uCmdRspBuf[read_index-2] == '\r' 
																																	 && uCmdRspFrame.uCmdRspBuf[read_index-1] == '\n') {
        rak_hexArrayToStr((char *)uCmdRspFrame.uCmdRspBuf, print_buf, read_index);
        DPRINTF(LOG_INFO, "%s", print_buf);
        read_index = 0;
    }
																																	 
    if(driver_ctx.cmd_type == CMD_NVM_READ) {
        if(0 == strncmp((char *)uCmdRspFrame.uCmdRspBuf, "OK", 2) && read_index == 4) {	//OK+len
            nvm_read_frame = (rak_nvmReadFrame*)uCmdRspFrame.uCmdRspBuf;
            data_len = nvm_read_frame->nvmReadFrame.len;
            while( read_index <(data_len+6)) {
                c = rw_uart_rx_byte();
                if(c >= 0) {
                    uCmdRspFrame.uCmdRspBuf[read_index++] = c;
                }
            }
            read_index = 0;
            driver_ctx.res_data = RES_OK;
        }
        else if(0 == strncmp((char *)uCmdRspFrame.uCmdRspBuf, "ERROR",5) && read_index == 8) {
            read_index = 0;
            driver_ctx.res_data = RES_ERR;
        }

    }
    
		if(strncmp((char *)uCmdRspFrame.uCmdRspBuf, "Welcome to RAK473\r\n", 19) == 0 && read_index == 19) {
        driver_ctx.res_data = RES_WELCOME;
        read_index = 0;
    }
		
    if(IS_CMD_BLOCK() && is_stamp_passed(&driver_ctx.timestamp)) {
        read_index = 0;
        driver_ctx.res_data = RES_TIMEOUT;
    }



}

/**
 *  brief.			Send cmd to open socket and parse respone
 * @param[in]		cmd_id -- use this param to Distinguish which socket is block in create socket ATcommand
 * @param[in]		socket_type -- 0:TCP 1:LTCP 2:UDP 3:LUDP
 * @param[in]		dest_ip -- destination ip, ex:0xc0a80103 presents 192.168.1.3
 * @param[in]		dest_port -- destination port
 * @param[in]		local_port -- if =0, the wifi module will alloc a random port
 * @param[in]		ssl_enable -- 0:disable ssl 	1:enable ssl
 * @param[out]	none
 * @return
 * <pre>
		>=0 -- socket id
		CMD_ERR_PEND -- have not recieved respones
		CMD_ERR_TIMEOUT -- have not recieved respones within a timeout
		ltcp: tcp server
		-2 -- create tcp servers's num > 4
		-3 -- bind local port fail ex:repeatly use a same port to create a ltcp
		-5 -- listen fail
		tcp: tcp client
		-2 -- create sockets's num > 8
		-3 -- bind local port fail
		-4 -- connect tcp server fail
		-7 -- ssl init fail
		-8 -- ssl handshake fail
		-9 -- ssl verify fail
		udp: udp client
		-2 -- create sockets's num > 8
		-3 -- bind local port fail
		-4 -- connect dest port fail
		ludp: udp server
		-2 -- create sockets's num > 8
		-3 -- bind local port fail
		multicast: udp multicast
		-2 -- create sockets's num > 8
		-3 -- bind local port fail
 * </pre>
 */
int open_socket(uint8_t cmd_id, uint8_t socket_type, uint32_t dest_ip, uint16_t dest_port, uint16_t local_port, uint8_t ssl_enable)//0:TCP 1:LTCP 2:UDP 3:LUDP 4 Mutilcast
{
    if(!IS_CMD_BLOCK()) {
        rak_open_socket(socket_type, dest_ip, dest_port, local_port, ssl_enable);
        SET_CTX_OPEN_BLOCK_SOCK(cmd_id);
    } else if(driver_ctx.cmd_type == CMD_OPEN_SOCKET && IS_OPEN_BLOCK_SOCK(cmd_id)) {
        if(driver_ctx.res_data != RES_NONE) {
            CLR_CTX_CMD_BLOCK();
            CLR_CTX_OPEN_BLOCK_SOCK(cmd_id);
            if(driver_ctx.res_data == RES_OK) {
                return uCmdRspFrame.socketFrameRcv.socketframe.socketDescriptordata;
            }
            else if(driver_ctx.res_data == RES_ERR) {
                return uCmdRspFrame.errorResponse.errorframe.errorCode;
            }
            else if(driver_ctx.res_data == RES_TIMEOUT) {
                return CMD_ERR_TIMEOUT;
            }
            driver_ctx.res_data = RES_NONE;
        }
        return CMD_ERR_PEND;
    }
    return CMD_ERR_PEND;

}
/**
 *  brief.			Send cmd to send data via socket and parse response from wifi module
 * @param[in]		fd
 * @param[in]		dest_port
 * @param[in]		dest_ip
 * @param[in]		send_len
 * @param[in]		buf
 * @param[out]	none
 * @return
 * <pre>
	CMD_ERR_OK -- send data success
	CMD_ERR_PEND -- have not recieved respones
	CMD_ERR_TIMEOUT	-- have not recieved respones within a timeout
	-2 -- socket invalid
	-3 -- send fail
 * </pre>
 */
int write_socket(int fd, uint16_t dest_port, uint32_t dest_ip, uint16_t send_len, uint8_t *buf)
{
    if(fd == FD_INVALID) return CMD_ERR_SOCK_INVALID;

    if(!IS_CMD_BLOCK()) {
        rak_send_data(fd, dest_port, dest_ip, send_len, buf);
        SET_CTX_SEND_BLOCK_SOCK(fd);
    }
    else if(driver_ctx.cmd_type == CMD_SEND_DATA && IS_SEND_BLOCK_SOCK(fd)) {
        if(driver_ctx.res_data != RES_NONE) {
            CLR_CTX_CMD_BLOCK();
            CLR_CTX_SEND_BLOCK_SOCK(fd);

            if(driver_ctx.res_data == RES_OK) {
                return CMD_ERR_OK;
            }
            else if(driver_ctx.res_data == RES_ERR) {
                return uCmdRspFrame.errorResponse.errorframe.errorCode;
            }
            else if(driver_ctx.res_data == RES_TIMEOUT) {
                return CMD_ERR_TIMEOUT;
            }
            driver_ctx.res_data = RES_NONE;
        }
        return CMD_ERR_PEND;
    }
    return CMD_ERR_PEND;
}


/**
 *  brief.			Read data from socket, support two types(query and interrupt)
 * @param[in]		fd
 * @param[in]		maxlen
 * @param[in]		remoteip
 * @param[in]		remoteport
 * @param[out]	buf
 * @return
 * <pre>
	>0 -- receive data
	=0 -- have not recieved data
	-2 -- socket invalid
	CMD_ERR_PEND	-- have not recieved respones
	CMD_ERR_SOCK_INVALID -- socket invalid
	CMD_ERR_TIMEOUT	-- have not recieved respones within a timeout
 * </pre>
 */
int read_socket(int fd, uint8_t* buf, uint16_t maxlen, uint32_t *remoteip, uint16_t *remoteport)
{
    uint16_t       data_len = 0;

    if(fd == FD_INVALID) return CMD_ERR_SOCK_INVALID;

#if defined MODE_INTERRUPT
    rak_recvdataFrame	*recv_data_frame;

    if(IS_DATA_VALID_SOCK(fd)) {
        CLR_CTX_DATA_VALID_SOCK(fd);
        recv_data_frame = (rak_recvdataFrame*)uCmdRspFrame.uCmdRspBuf;
        data_len = (recv_data_frame->recvdataframe.recDataLen[1]*256 + recv_data_frame->recvdataframe.recDataLen[0]);
        *remoteip = recv_data_frame->recvdataframe.destIp[3]<<24 |
                    recv_data_frame->recvdataframe.destIp[2]<<16 |
                    recv_data_frame->recvdataframe.destIp[1]<<8  |
                    recv_data_frame->recvdataframe.destIp[0];
        *remoteport = recv_data_frame->recvdataframe.destPort[0] + recv_data_frame->recvdataframe.destPort[1]*256;

        if(fd == recv_data_frame->recvdataframe.socketDescriptor) {
            memcpy(buf ,(uint8_t*)&recv_data_frame->recvdataframe.recvdataBuf,data_len);
            return data_len;
        }
        return 0;
    }

#elif defined MODE_QUERY

    rak_readSocketFrame * read_socket_frame;

    if(!IS_CMD_BLOCK()) {
        rak_query_socket(fd, maxlen);
        while(1) {
            read_data_from_module();
            if(driver_ctx.res_data != RES_NONE) {
                CLR_CTX_CMD_BLOCK();
                if(driver_ctx.res_data == RES_OK) {
                    read_socket_frame = (rak_readSocketFrame*)uCmdRspFrame.uCmdRspBuf;
                    data_len = read_socket_frame->readSocketFrame.recDataLen[1]*256+read_socket_frame->readSocketFrame.recDataLen[0];
                    if(data_len > 0) {
                        memcpy(buf, read_socket_frame->readSocketFrame.recvdataBuf, data_len);
                    }
                    return data_len;
                }
                else if(driver_ctx.res_data == RES_ERR) {
                    return uCmdRspFrame.errorResponse.errorframe.errorCode;
                }
                else if(driver_ctx.res_data == RES_TIMEOUT) {
                    return CMD_ERR_TIMEOUT;
                }
            }
        }
    }
#endif
    return CMD_ERR_PEND;
}

/**
 *  brief.			query socket status
 * @param[in]		sock_fd
 * @param[out]	none
 * @return
 * <pre>
	CMD_ERR_OK	-- cmd success, query the variable - uCmdRspFrame for detail info
	CMD_ERR_PEND	-- have not recieved respones
	CMD_ERR_TIMEOUT	-- have not recieved respones within a timeout
 * </pre>
 */
int query_socket_status(int sock_fd)
{
    if(!IS_CMD_BLOCK()) {
        rak_query_socket_status(sock_fd);
        while(1) {
            read_data_from_module();
            if(driver_ctx.res_data != RES_NONE) {
                CLR_CTX_CMD_BLOCK();
                if(driver_ctx.res_data == RES_OK) {
                    return CMD_ERR_OK;
                }
                else if(driver_ctx.res_data == RES_TIMEOUT) {
                    return CMD_ERR_TIMEOUT;
                }
            }
        }
    }
    return CMD_ERR_PEND;
}

/**
 *  brief.			query module's connection with router
 * @param[in]		none
 * @param[out]	none
 * @return
 * <pre>
	CMD_ERR_OK	-- cmd succuess, query the variable - uCmdRspFrame for detail info
	CMD_ERR_PEND	-- have not recieved respones
	<0 -- cmd error
	CMD_ERR_TIMEOUT	-- have not recieved respones within a timeout
 * </pre>
 */
int query_con_status(void)
{
    if(!IS_CMD_BLOCK()) {
        rak_query_con_status();
        while(1) {
            read_data_from_module();
            if(driver_ctx.res_data != RES_NONE) {
                CLR_CTX_CMD_BLOCK();
                if(driver_ctx.res_data == RES_OK) {
                    return CMD_ERR_OK;
                }
                else if(driver_ctx.res_data == RES_ERR) {
                    return uCmdRspFrame.errorResponse.errorframe.errorCode;
                }
                else if(driver_ctx.res_data == RES_TIMEOUT) {
                    return CMD_ERR_TIMEOUT;
                }
            }
        }
    }
    return CMD_ERR_PEND;
}


/**
 *  brief.			write data to usr flash
 * @param[in]		addr -- write addr
 * @param[in]		len -- write len
 * @param[in]		data -- data to write
 * @param[out]	none
 * @return
 * <pre>
	CMD_ERR_OK	-- cmd ok
	<0 -- cmd error
	CMD_ERR_PEND	-- cmd is block
	CMD_ERR_TIMEOUT	-- have not recieved respones within a timeout
 * </pre>
 */
int nvm_write(uint32_t addr, uint16_t len, char *data)
{
    if(!IS_CMD_BLOCK()) {
        rak_nvm_write(addr, len, data);
        while(1) {
            read_data_from_module();
            if(driver_ctx.res_data != RES_NONE) {
                CLR_CTX_CMD_BLOCK();
                if(driver_ctx.res_data == RES_OK) {
                    return CMD_ERR_OK;
                }
                else if(driver_ctx.res_data == RES_ERR) {
                    return uCmdRspFrame.errorResponse.errorframe.errorCode;
                }
                else if(driver_ctx.res_data == RES_TIMEOUT) {
                    return CMD_ERR_TIMEOUT;
                }
            }
        }
    }
    return CMD_ERR_PEND;
}

/**
 *  brief.			read data from usr flash
 * @param[in]		addr -- read addr
 * @param[in]		len  -- read len
 * @param[out]	buf  -- store read data
 * @return
 * <pre>
	>0 -- read data ok
	-2 -- cmd error
	CMD_ERR_PEND	-- have not recieved respones
	CMD_ERR_TIMEOUT	-- have not recieved respones within a timeout
 * </pre>
 */
int nvm_read(uint32_t addr, uint16_t len, char *buf)
{
    rak_nvmReadFrame * nvm_read_frame;
    uint16_t data_len = 0;

    if(!IS_CMD_BLOCK()) {
        rak_nvm_read(addr, len);
        while(1) {
            read_data_from_module();
            if(driver_ctx.res_data != RES_NONE) {
                CLR_CTX_CMD_BLOCK();
                if(driver_ctx.res_data == RES_OK) {
                    nvm_read_frame = (rak_nvmReadFrame*)uCmdRspFrame.uCmdRspBuf;
                    data_len = nvm_read_frame->nvmReadFrame.len;
                    if(data_len > 0) {
                        memcpy(buf, nvm_read_frame->nvmReadFrame.data, data_len);
                    }
                    return data_len;
                }
                else if(driver_ctx.res_data == RES_ERR) {
                    return uCmdRspFrame.errorResponse.errorframe.errorCode;
                }
                else if(driver_ctx.res_data == RES_TIMEOUT) {
                    return CMD_ERR_TIMEOUT;
                }
            }
        }
    }
    return CMD_ERR_PEND;
}

