/**
* @file 		filename
* @brief		This is a brief description.
* @details	This is the detail description.
* @author		author
* @date		date
* @version	A001
* @par Copyright (c):
* 		rakwireless
* @par History:
*	version: author, date, desc\n
*/
#include "rak473_driver.h"


/**
 *  brief.
 * @param[in]
 * @param[out]
 * @return
 */
int promisc_func(void)
{
    int ret = -1;
		
		if(driver_ctx.cmd_type == CMD_PROMISC_DATA) return CMD_ERR_OK;
		
    if(!IS_CMD_BLOCK()) {
        switch(driver_ctx.cmd_type)
        {
        case CMD_SET_ASCII:
            rak_set_ascii(0);
            break;
        case CMD_GET_MAC:
            rak_query_mac();
            break;
        case CMD_SET_FILTER:
            rak_set_filter(&rak_strApi.uFilterFrame);
            break;
        case CMD_START_PROMISC:
            rak_start_promisc(&rak_strApi.uFilterFrame);
            break;
        case CMD_STOP_PROMISC:
            rak_stop_promisc();
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
                    SET_CTX_CMD_TYPE(CMD_SET_FILTER);
                    ret = CMD_ERR_OK;
                    break;
                case CMD_SET_FILTER:
                    SET_CTX_CMD_TYPE(CMD_START_PROMISC);
                    ret = CMD_ERR_OK;
                    break;
                case CMD_START_PROMISC:
										SET_CTX_CMD_TYPE(CMD_PROMISC_DATA);
                    ret = CMD_ERR_PROMISC_OK;
                    break;
                case CMD_STOP_PROMISC:
                    ret = CMD_ERR_STOP_PROMISC_OK;
                    break;
                default:
                    DPRINTF(LOG_INFO, "unknow cmd");
                    break;
                }
                return ret;
            }
            else if(driver_ctx.res_data == RES_ERR) {
                DPRINTF(LOG_INFO, "cmd=%d err=%d", driver_ctx.cmd_type, uCmdRspFrame.errorResponse.errorframe.errorCode);
                return driver_ctx.res_data;
            }
            else if(driver_ctx.res_data == RES_TIMEOUT) {
                return CMD_ERR_TIMEOUT;
            }
            driver_ctx.res_data = RES_NONE;
        }

        return CMD_ERR_PEND;
    }
}
