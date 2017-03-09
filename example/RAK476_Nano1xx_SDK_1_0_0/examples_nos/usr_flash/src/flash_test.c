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

uint16_t nvm_write_buf[512];
uint16_t nvm_read_buf[512];

int main()
{
    int ret = -1;
		uint32_t start_time = 0; 
    host_platformInit();
    DPRINTF(LOG_INFO, "Host platform init...success\r\n");
    rak_module_init();
    init_param();
		
		for(int i=0;i<512;i++) {
				nvm_write_buf[i] = i;
		}
		
		ret = nvm_write(0, 1024, (char *)nvm_write_buf);
		DPRINTF(LOG_INFO, "nvm_write ret %d", ret);
		
		ret = nvm_read(0, 1024, (char *)nvm_read_buf);
		DPRINTF(LOG_INFO, "nvm_read ret %d", ret);
		
		if(memcmp(nvm_write_buf, nvm_read_buf, 1024) == 0) {
				DPRINTF(LOG_INFO, "flash test ok");
		} else {
				DPRINTF(LOG_INFO, "flash test err");
		}
		while(1);

}
