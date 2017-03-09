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
#if (RAK_AP_STATION_MODE==0)
int net_config(void)
{
    int ret = -1;

    if(!IS_CMD_BLOCK()) {
        switch(driver_ctx.cmd_type)
        {
        case CMD_SET_ASCII:
            rak_set_ascii(0);
            break;
//        case CMD_SET_FUNCMAP:
//#if defined MODE_INTERRUPT
//            rak_set_funcBitmap(0);
//#elif defined MODE_QUERY
//            rak_set_funcBitmap(1);
//#endif
//            break;
        case CMD_GET_MAC:
            rak_query_mac();
            break;
        case CMD_SET_PSK:
            rak_set_psk(&rak_strApi.uJoinFrame);
            break;
        case CMD_CONNECT_AP:
            rak_connect_ap(&rak_strApi.uJoinFrame);
            break;
        case CMD_IPDHCP:
            rak_set_ipdhcp(&rak_strApi.uIpparamFrame);
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
//                case CMD_SET_FUNCMAP:
//                    SET_CTX_CMD_TYPE(CMD_GET_MAC);
//                    ret = CMD_ERR_OK;
//                    break;
                case CMD_GET_MAC:
                    SET_CTX_CMD_TYPE(CMD_SET_PSK);
                    ret = CMD_ERR_OK;
                    break;
                case CMD_SET_PSK:
                    SET_CTX_CMD_TYPE(CMD_CONNECT_AP);
                    ret = CMD_ERR_OK;
                    break;
                case CMD_CONNECT_AP:
                    SET_CTX_CMD_TYPE(CMD_IPDHCP);
                    ret = CMD_ERR_OK;
                    break;
                case CMD_IPDHCP:
                    ret = CMD_ERR_NET_OK;
                    break;
                default:
                    DPRINTF(LOG_INFO, "unknow cmd");
                    break;
                }
                return (ret==CMD_ERR_NET_OK)?CMD_ERR_NET_OK:CMD_ERR_PEND;
            }
            else if(driver_ctx.res_data == RES_ERR) {
                if(driver_ctx.cmd_type == CMD_CONNECT_AP) {
                    SET_CTX_CMD_TYPE(CMD_CONNECT_AP);
                } else if(driver_ctx.cmd_type ==  CMD_IPDHCP) {
										SET_CTX_CMD_TYPE(CMD_IPDHCP);
								} else {
                    DPRINTF(LOG_INFO, "cmd=%d err=%d", driver_ctx.cmd_type, uCmdRspFrame.errorResponse.errorframe.errorCode);
                }
            }
            else if(driver_ctx.res_data == RES_TIMEOUT) {
                return CMD_ERR_TIMEOUT;
            }
            driver_ctx.res_data = RES_NONE;
        }

        return CMD_ERR_PEND;
    }
}
#endif
/**
 *  brief.
 * @param[in]
 * @param[out]
 * @return
 */
#if (RAK_AP_STATION_MODE==1)
int net_config(void)
{
    int ret = -1;

    if(!IS_CMD_BLOCK()) {
        switch(driver_ctx.cmd_type)
        {
        case CMD_SET_ASCII:
            rak_set_ascii(0);
            break;
        case CMD_GET_MAC:
            rak_query_mac();
            break;
        case CMD_SET_PSK:
            rak_set_psk(&rak_strApi.uJoinFrame);
            break;
        case CMD_CREATE_AP:
            rak_create_ap(&rak_strApi.uApFrame);
            break;
        case CMD_IPSTATIC:
            rak_set_ipstatic(&rak_strApi.uIpparamFrame);
            break;
        case CMD_IPDHCP:
            rak_set_ipdhcp(&rak_strApi.uIpparamFrame);
            break;
        default:
            DPRINTF(LOG_INFO, "unknow cmd");
            break;
        }
        return (ret==CMD_ERR_NET_OK)?CMD_ERR_NET_OK:CMD_ERR_PEND;
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
                    SET_CTX_CMD_TYPE(CMD_SET_PSK);
                    ret = CMD_ERR_OK;
                    break;
                case CMD_SET_PSK:
                    SET_CTX_CMD_TYPE(CMD_CREATE_AP);
                    ret = CMD_ERR_OK;
                    break;
                case CMD_CREATE_AP:
                    SET_CTX_CMD_TYPE(CMD_IPSTATIC);
                    ret = CMD_ERR_OK;
                    break;
                case CMD_IPSTATIC:
                    SET_CTX_CMD_TYPE(CMD_IPDHCP);
                    ret = CMD_ERR_OK;
                    break;
                case CMD_IPDHCP:
                    ret = CMD_ERR_NET_OK;
                    break;
                default:
                    DPRINTF(LOG_INFO, "unknow cmd");
                    break;
                }
                return ret;
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
}
#endif
