#include "rw_app.h"

APP_CTX app_ctx;

//
void print_conn_event(uint8_t rsp_code, uint8_t block, uint8_t socket_fd)
{
    switch(rsp_code) {
    case 0x80:
        break;
    case 0x81:
        break;
    case 0x82:
        DPRINTF(LOG_INFO, "net conn");
        break;
    case 0x83:
        DPRINTF(LOG_INFO, "net dis");
        if(RAK_AP_STATION_MODE == 0) {
            app_ctx.net_status = 0;
            SET_CTX_CMD_TYPE(CMD_AUTOCONNECT);
        }
        break;
    default:
        break;
    }
}

void init_param(void)
{
    driver_ctx.conn_event = print_conn_event;
}

/**
 *  brief.			module init
 * @param[in]		none
 * @param[out]	none
 * @return			0
 */
int rak_module_init(void)
{
    rak_init_struct(&rak_strApi);

    rw_reset_target();			  //RESET wifi Module

    while(1) {
        read_data_from_module();
        if(driver_ctx.res_data == RES_WELCOME)
            break;
    }

    SET_CTX_CMD_TYPE(CMD_SET_ASCII);
    return 0;

}

int main()
{
    int ret = -1;
    host_platformInit();
    DPRINTF(LOG_INFO, "Host platform init...success\r\n");
    rak_module_init();
    init_param();

    while(1) {
        read_data_from_module();

        if(app_ctx.net_status != 1) {
            ret = net_config();
            if(ret == CMD_ERR_NET_OK) {
                app_ctx.net_status = 1;
                DPRINTF(LOG_INFO, "net config done");
            } else if(ret == CMD_ERR_TIMEOUT) {
                DPRINTF(LOG_INFO, "cmd timeout");
            }
        }
        if(app_ctx.net_status == 1) {
        }
    }

}
