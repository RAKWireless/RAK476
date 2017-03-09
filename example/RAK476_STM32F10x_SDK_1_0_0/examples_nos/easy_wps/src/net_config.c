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

/**
 *  brief.			work flow:
								set_ascii--->get_mac--->easy_config--->net ok
																						|
																						|fail
																						|--->start_web--->wait "Welcome to RAK473\r\n"--->get_storeconfig--->auto_connect--->net ok
 * @param[in]		none
 * @param[out]	none
 * @return
 * <pre>
 CMD_ERR_NET_OK -- connect to ap & dhcp success
 CMD_ERR_PEND -- have not recieved respones
 CMD_ERR_TIMEOUT -- have not recieved respones within a timeout
 * </pre>
 */
int net_config(void)
{
    int ret = -1;
    static uint8_t try_times = 10;

    if(!IS_CMD_BLOCK()) {
        switch(driver_ctx.cmd_type)
        {
        case CMD_SET_ASCII:
            rak_set_ascii(0);
            break;
        case CMD_GET_MAC:
            rak_query_mac();
            break;
        case CMD_EASYCONFIG:
            rak_easyconfig();
            break;
				case CMD_WPS:
						rak_wps();
						break;
        case CMD_STARTWEB:
            rak_start_webconfig();
            break;
        case CMD_GETSTORECONFIG:
            if(driver_ctx.res_data == RES_WELCOME) {
                rak_getstoreconfig();
            }
            break;
        case CMD_AUTOCONNECT:
            rak_start_autoconnect();
            try_times--;
            if(try_times == 0) {
                DPRINTF(LOG_ERROR, "auto connect fail!!!");
                while(1);
            }
            break;
        default:
            DPRINTF(LOG_INFO, "unknow cmd");
            break;
        }
        return CMD_ERR_PEND;
    } else {
        if(driver_ctx.res_data != RES_NONE) {
            CLR_CTX_CMD_BLOCK();
            if(driver_ctx.res_data == RES_OK) {
                switch(driver_ctx.cmd_type) {
                case CMD_SET_ASCII:
                    SET_CTX_CMD_TYPE(CMD_GET_MAC);
                    ret = CMD_ERR_OK;
                    break;
                case CMD_GET_MAC:
										SET_CTX_CMD_TYPE(CMD_EASYCONFIG);			//SET_CTX_CMD_TYPE(CMD_WPS);			
										ret = CMD_ERR_OK;
                    break;
                case CMD_EASYCONFIG:
                    ret = CMD_ERR_NET_OK;
                    break;
                case CMD_STARTWEB:
                    SET_CTX_CMD_TYPE(CMD_GETSTORECONFIG);
                    ret = CMD_ERR_OK;
                    break;
                case CMD_GETSTORECONFIG:
                    SET_CTX_CMD_TYPE(CMD_AUTOCONNECT);
                    ret = CMD_ERR_OK;
                    break;
                case CMD_AUTOCONNECT:
                    ret = CMD_ERR_NET_OK;
                    break;
                default:
                    DPRINTF(LOG_INFO, "unknow cmd");
                    break;
                }
                return (ret==CMD_ERR_NET_OK)?CMD_ERR_NET_OK:CMD_ERR_PEND;
            }
            else if(driver_ctx.res_data == RES_ERR) {
                if(driver_ctx.cmd_type == CMD_EASYCONFIG || driver_ctx.cmd_type == CMD_WPS) {
                    SET_CTX_CMD_TYPE(CMD_STARTWEB);
                } else if(driver_ctx.cmd_type == CMD_AUTOCONNECT) {
                    SET_CTX_CMD_TYPE(CMD_AUTOCONNECT);
                } else {
                    DPRINTF(LOG_INFO, "cmd=%d err=%d", driver_ctx.cmd_type, uCmdRspFrame.errorResponse.errorframe.errorCode);
                }
                return CMD_ERR_PEND;
            }
            else if(driver_ctx.res_data == RES_UPGRADE) {
                SET_CTX_CMD_TYPE(CMD_GETSTORECONFIG);
            }
            else if(driver_ctx.res_data == RES_TIMEOUT) {
                return CMD_ERR_TIMEOUT;
            }
            driver_ctx.res_data = RES_NONE;
        }

        return CMD_ERR_PEND;
    }
}
