#include "stdint.h"
#include "rak473_driver.h"
#include "bsp.h"

typedef struct {
    uint8_t net_status;
	
    int ltcps;
    int ltcps_cli[RAK_MAX_TCP_CLIENTS];
    int tcpc_num;

    int tcpc;
    int ludps;
    int udpc;

} APP_CTX;

void init_param(void);
void RAK_TcpClient_EventHandle(void);
void RAK_TcpServer_EventHandle(void);
void RAK_UdpClient_EventHandle(void);
void RAK_UdpServer_EventHandle(void);

extern APP_CTX app_ctx;

