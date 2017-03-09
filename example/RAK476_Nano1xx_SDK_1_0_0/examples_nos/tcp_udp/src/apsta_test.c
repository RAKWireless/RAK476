#include "rw_app.h"
APP_CTX app_ctx;

void init_param(void);
void print_conn_event(uint8_t rsp_code, uint8_t block, uint8_t socket_fd);

//
void print_conn_event(uint8_t rsp_code, uint8_t block, uint8_t socket_fd)
{
    int i;
    switch(rsp_code) {
    case 0x80:
        DPRINTF(LOG_INFO, "sock %d conn", socket_fd);
        for(i=0; i<RAK_MAX_TCP_CLIENTS; i++) {
            if(app_ctx.ltcps_cli[i] == FD_INVALID) {
                app_ctx.ltcps_cli[i] = socket_fd;
                app_ctx.tcpc_num++;
                break;
            }
        }
        if(i == RAK_MAX_TCP_CLIENTS) {
            DPRINTF(LOG_INFO, "too much client")
        }
        break;
    case 0x81:
        DPRINTF(LOG_INFO, "sock %d dis", socket_fd);
        if(!block) {
            if(app_ctx.tcpc == socket_fd) {		//tcp client recv close event
                app_ctx.tcpc = FD_INVALID;
            }
            for(i=0; i<RAK_MAX_TCP_CLIENTS; i++) {			//tcp server recv client dis event
                if(app_ctx.ltcps_cli[i] == socket_fd) {
                    app_ctx.ltcps_cli[i] = FD_INVALID;
                    app_ctx.tcpc_num--;
                    break;
                }
            }
        }
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
    int i;

    driver_ctx.conn_event = print_conn_event;
		
		app_ctx.net_status = 0;
    app_ctx.ltcps = FD_INVALID;
    for(i=0; i<RAK_MAX_TCP_CLIENTS; i++) {
        app_ctx.ltcps_cli[i] = FD_INVALID;
    }
    app_ctx.tcpc = FD_INVALID;
    app_ctx.ludps = FD_INVALID;
    app_ctx.udpc = FD_INVALID;
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
            } else if(ret == CMD_ERR_TIMEOUT) {
                DPRINTF(LOG_INFO, "cmd timeout");
            } 
        }
        if(app_ctx.net_status == 1) {
//            RAK_TcpClient_EventHandle();
            RAK_TcpServer_EventHandle();
//            RAK_UdpClient_EventHandle();
//            RAK_UdpServer_EventHandle();
        }
    }

}
