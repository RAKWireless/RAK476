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
            SET_CTX_CMD_TYPE(CMD_CONNECT_AP);
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

    rw_reset_target();			  //RESET Module

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
		uint32_t start_time = 0; 
    host_platformInit();
    DPRINTF(LOG_INFO, "Host platform init...success\r\n");
    rak_module_init();
    init_param();

    while(1) {
        read_data_from_module();

        ret = promisc_func();
        if(ret == CMD_ERR_PROMISC_OK) {
						start_time = get_stamp();
        }
				else if(ret == CMD_ERR_STOP_PROMISC_OK) {
						DPRINTF(LOG_INFO, "test over");
						while(1);
				}
				else if(ret == CMD_ERR_TIMEOUT) {
            DPRINTF(LOG_INFO, "cmd timeout");
        }
				
				if(start_time > 0) {
						if((get_stamp() - start_time) >= 10000)	 {	//10s
								SET_CTX_CMD_TYPE(CMD_STOP_PROMISC);
						}
				}
    }

}
