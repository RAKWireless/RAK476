#include "rw_app.h"

#define PRI_IP "%u.%u.%u.%u"
#define PRI_IP_FORMAT(addr) (uint8_t)(addr>>24), (uint8_t)(addr>>16), (uint8_t)(addr>>8), (uint8_t)(addr)
#define DEST_IP			0xc0a80174

#define SOCK_INDEX0			0
#define SOCK_INDEX1			1
#define SOCK_INDEX2			2
#define SOCK_INDEX3			3

uint8_t temp_buf[RAK_MAX_PAYLOAD_SEND_SIZE];
static uint32_t   data_total[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

////////////////////////////////////////////

void RAK_TcpClient_EventHandle()
{
    int temp_fd = -1;
    int ret;
    static int send_count = RAK_MAX_PAYLOAD_SEND_SIZE*100;
    uint32_t ip;
    uint16_t port;

    if(app_ctx.tcpc == FD_INVALID && !IS_SEND_BLOCK_SOCK(app_ctx.tcpc) && !IS_DATA_VALID_SOCK(app_ctx.tcpc)) {
//        temp_fd = open_socket(0, 0, DEST_IP, 443, 0, 1);
			  temp_fd = open_socket(SOCK_INDEX0, 0, DEST_IP, 8000, 0, 0);
        if(temp_fd >= 0) {
            DPRINTF(LOG_INFO, "fd %d ok", temp_fd);
            app_ctx.tcpc = temp_fd;
            data_total[temp_fd] = 0;
        } else if(temp_fd < 0) {
            if(temp_fd == CMD_ERR_PEND) {
//								DPRINTF(LOG_INFO, "cmd pend");
            } else {
                DPRINTF(LOG_INFO, "open sock error %d", temp_fd);
            }
        }
    } else {
        ret = read_socket(app_ctx.tcpc, temp_buf, RAK_MAX_PAYLOAD_SIZE, &ip, &port);
        if(ret > 0) {
            data_total[app_ctx.tcpc] += ret;
            DPRINTF(LOG_INFO, "fd%u %u %u " PRI_IP ":%u", app_ctx.tcpc, ret, data_total[app_ctx.tcpc], PRI_IP_FORMAT(ip), port);
        } else if(ret == 0) {
            DPRINTF(LOG_INFO, "fd %d read 0", app_ctx.tcpc);
        }
        else if(ret < 0) {
            if(ret == CMD_ERR_PEND) {
//								DPRINTF(LOG_INFO, "cmd pend");
            } else if(ret == CMD_ERR_SOCK_INVALID) {
                DPRINTF(LOG_INFO, "fd %d invalid", app_ctx.tcpc);
                app_ctx.tcpc = FD_INVALID;
            }	else if(ret == CMD_ERR_TIMEOUT) {
                DPRINTF(LOG_ERROR, "cmd timeout");
            } else {
                DPRINTF(LOG_INFO, "read sock error %d", ret);
            }
        }
				
        if(send_count > 0) {
            ret = write_socket(app_ctx.tcpc, 0, 0, RAK_MAX_PAYLOAD_SEND_SIZE, temp_buf);
            if(ret == CMD_ERR_OK) {
                DPRINTF(LOG_INFO, "send ok");
                send_count -= RAK_MAX_PAYLOAD_SEND_SIZE;
            } else if(ret < 0) {
                if(ret == CMD_ERR_PEND) {
//								DPRINTF(LOG_INFO, "cmd pend");
                } else if(ret == CMD_ERR_SOCK_INVALID) {
                    DPRINTF(LOG_INFO, "fd %d invalid", app_ctx.tcpc);
                    app_ctx.tcpc = FD_INVALID;
                }	else if(ret == CMD_ERR_TIMEOUT) {
                    DPRINTF(LOG_ERROR, "cmd timeout");
                } else {
                    DPRINTF(LOG_INFO, "send error %d", ret);
                }
            }
        }
    }
}

void RAK_TcpServer_EventHandle(void)
{
    int temp_fd = -1;
    int ret;
    int i=0;
    uint32_t ip;
    uint16_t port;

    static int send_count[3] = {RAK_MAX_PAYLOAD_SEND_SIZE*100, RAK_MAX_PAYLOAD_SEND_SIZE*100, RAK_MAX_PAYLOAD_SEND_SIZE*100};

    if(app_ctx.ltcps == FD_INVALID) {
        temp_fd = open_socket(SOCK_INDEX1, 1, 0, 0, 25000, 0);
        if(temp_fd >= 0) {
            DPRINTF(LOG_INFO, "fd %d ok", temp_fd);
            app_ctx.ltcps = temp_fd;
        } else if(temp_fd < 0) {
            if(temp_fd == CMD_ERR_PEND) {
//								DPRINTF(LOG_INFO, "cmd pend");
            } else {
                DPRINTF(LOG_INFO, "open sock error %d", temp_fd);
            }
        }
    } else {

#if defined MODE_QUERY
        //query socket status
        ret = query_socket_status(app_ctx.ltcps);
        if(ret == CMD_ERR_OK) {
            int valid_num = 0;
            int i = 0;
            int cli_fd = -1;
            rak_qrySocketStatusFrame* socket_status_frame;
            socket_status_frame = (rak_qrySocketStatusFrame*)uCmdRspFrame.uCmdRspBuf;
            valid_num = socket_status_frame->qrySocketStatusFrame.valid_num;

//            DPRINTF(LOG_INFO, "valid_num=%d", valid_num);

            for(i=0; i<RAK_MAX_TCP_CLIENTS; i++) {
                app_ctx.ltcps_cli[i] = FD_INVALID;
                app_ctx.tcpc_num = 0;
            }
            if(valid_num > 0) {
                for(i=0; i<valid_num; i++) {
                    cli_fd = socket_status_frame->qrySocketStatusFrame.socket_info[i].socketDescriptor;
                    app_ctx.ltcps_cli[i] = cli_fd;
                    app_ctx.tcpc_num++;
//                    DPRINTF(LOG_INFO, "recv fd %d", app_ctx.ltcps_cli[i]);
                }
            }
        }
#endif
        if(app_ctx.tcpc_num > 0) {
            for(i=0; i<RAK_MAX_TCP_CLIENTS; i++) {
                if(app_ctx.ltcps_cli[i] != FD_INVALID || IS_SEND_BLOCK_SOCK(app_ctx.ltcps_cli[i]) || IS_DATA_VALID_SOCK(app_ctx.ltcps_cli[i])) {
                    ret = read_socket(app_ctx.ltcps_cli[i], temp_buf, RAK_MAX_PAYLOAD_SIZE, &ip, &port);
                    if(ret > 0) {
                        data_total[app_ctx.ltcps_cli[i]] += ret;
                        DPRINTF(LOG_INFO, "fd%u %u %u " PRI_IP ":%u", app_ctx.ltcps_cli[i], ret,
                                data_total[app_ctx.ltcps_cli[i]], PRI_IP_FORMAT(ip), port);
                    } else if(ret == 0) {
                        DPRINTF(LOG_INFO, "fd %d read 0", app_ctx.ltcps_cli[i]);
                    }
                    else if(ret < 0) {
                        if(ret == CMD_ERR_PEND) {
                            //								DPRINTF(LOG_INFO, "cmd pend");
                        } else if(ret == CMD_ERR_SOCK_INVALID) {
                            DPRINTF(LOG_INFO, "fd %d invalid", app_ctx.ltcps_cli[i]);
                            app_ctx.ltcps_cli[i] = FD_INVALID;
                        }	else if(ret == CMD_ERR_TIMEOUT) {
                            DPRINTF(LOG_ERROR, "cmd timeout");
                        } else {
                            DPRINTF(LOG_INFO, "read sock error %d", ret);
                        }
                    }
										
                    if(send_count[i] > 0) {
                        ret = write_socket(app_ctx.ltcps_cli[i], 0, 0, RAK_MAX_PAYLOAD_SEND_SIZE, temp_buf);
                        if(ret == CMD_ERR_OK) {
                            DPRINTF(LOG_INFO, "send ok");
                            send_count[i] -= RAK_MAX_PAYLOAD_SEND_SIZE;
                        } else if(ret < 0) {
                            if(ret == CMD_ERR_PEND) {
                                //								DPRINTF(LOG_INFO, "cmd pend");
                            } else if(ret == CMD_ERR_SOCK_INVALID) {
                                DPRINTF(LOG_ERROR, "fd %d invalid", app_ctx.ltcps_cli[i]);
                                app_ctx.ltcps_cli[i] = FD_INVALID;
                            }	else if(ret == CMD_ERR_TIMEOUT) {
                                DPRINTF(LOG_ERROR, "cmd timeout");
                            } else {
                                DPRINTF(LOG_INFO, "send error %d", ret);
                            }
                        }
                    }

                } else if(app_ctx.ltcps_cli[i] == FD_INVALID) {
                    data_total[app_ctx.ltcps_cli[i]] = 0;
                }
            }
        }
    }
}


void RAK_UdpClient_EventHandle(void)
{
    int temp_fd = -1;
    int ret;
    static int send_count = RAK_MAX_PAYLOAD_SEND_SIZE*100;
    uint32_t ip;
    uint16_t port;

    if(app_ctx.udpc == FD_INVALID) {
        temp_fd = open_socket(SOCK_INDEX2, 2, DEST_IP, 9000, 3000, 0);
        if(temp_fd >= 0) {
            DPRINTF(LOG_INFO, "fd %d ok", temp_fd);
            app_ctx.udpc = temp_fd;
            data_total[temp_fd] = 0;
        } else if(temp_fd < 0) {
            if(temp_fd == CMD_ERR_PEND) {
//								DPRINTF(LOG_INFO, "cmd pend");
            } else {
                DPRINTF(LOG_INFO, "open sock error %d", temp_fd);
            }
        }
    }
    else {
        ret = read_socket(app_ctx.udpc, temp_buf, RAK_MAX_PAYLOAD_SIZE, &ip, &port);
        if(ret > 0) {
            data_total[app_ctx.udpc] += ret;
            DPRINTF(LOG_INFO, "fd%u %u %u " PRI_IP ":%u", app_ctx.udpc, ret, data_total[app_ctx.udpc], PRI_IP_FORMAT(ip), port);
        } else if(ret == 0) {
            DPRINTF(LOG_INFO, "fd %d read 0", app_ctx.udpc);
        }
        else if(ret < 0) {
            if(ret == CMD_ERR_PEND) {
                //								DPRINTF(LOG_INFO, "cmd pend");
            } else if(ret == CMD_ERR_TIMEOUT) {
                DPRINTF(LOG_ERROR, "cmd timeout");
            } else {
                DPRINTF(LOG_INFO, "read sock error %d", ret);
            }
        }

        if(app_ctx.udpc != FD_INVALID && send_count > 0) {
            ret = write_socket(app_ctx.udpc, 0, 0, RAK_MAX_PAYLOAD_SEND_SIZE, temp_buf);
            if(ret == CMD_ERR_OK) {
                DPRINTF(LOG_INFO, "send ok");
                send_count -= RAK_MAX_PAYLOAD_SEND_SIZE;
            } else if(ret < 0) {
                if(ret == CMD_ERR_PEND) {
                    //								DPRINTF(LOG_INFO, "cmd pend");
                } else if(ret == CMD_ERR_SOCK_INVALID) {
                    DPRINTF(LOG_ERROR, "fd %d invalid", app_ctx.udpc);
                    app_ctx.udpc = 0;
                }	else if(ret == CMD_ERR_TIMEOUT) {
                    DPRINTF(LOG_ERROR, "cmd timeout");
                } else {
                    DPRINTF(LOG_INFO, "send error %d", ret);
                }
            }
        }
    }
}

void RAK_UdpServer_EventHandle(void)
{
    int temp_fd = -1;
    int ret;
    static uint32_t ip = DEST_IP;
    static uint16_t port = 3000;
    static int send_count = RAK_MAX_PAYLOAD_SEND_SIZE*100;

    if(app_ctx.ludps == FD_INVALID) {
        temp_fd = open_socket(SOCK_INDEX3, 3, 0, 0, 25001, 0);
        if(temp_fd >= 0) {
            DPRINTF(LOG_INFO, "fd %d ok", temp_fd);
            app_ctx.ludps = temp_fd;
            data_total[temp_fd] = 0;
        } else if(temp_fd < 0) {
            if(temp_fd == CMD_ERR_PEND) {
//								DPRINTF(LOG_INFO, "cmd pend");
            } else {
                DPRINTF(LOG_INFO, "open sock error %d", temp_fd);
            }
        }
    }
    else {
        ret = read_socket(app_ctx.ludps, temp_buf, RAK_MAX_PAYLOAD_SIZE, &ip, &port);
        if(ret > 0) {
            data_total[app_ctx.ludps] += ret;
            DPRINTF(LOG_INFO, "fd%u %u %u " PRI_IP ":%u", app_ctx.ludps, ret, data_total[app_ctx.ludps], PRI_IP_FORMAT(ip), port);
        } else if(ret == 0) {
            DPRINTF(LOG_INFO, "fd %d read 0", app_ctx.ludps);
        }
        else if(ret < 0) {
            if(ret == CMD_ERR_PEND) {
                //								DPRINTF(LOG_INFO, "cmd pend");
            } else if(ret == CMD_ERR_TIMEOUT) {
                DPRINTF(LOG_ERROR, "cmd timeout");
            } else {
                DPRINTF(LOG_INFO, "read sock error %d", ret);
            }
        }

        if(app_ctx.ludps != FD_INVALID && ip != 0 && send_count > 0) {
            ret = write_socket(app_ctx.ludps, port, ip, RAK_MAX_PAYLOAD_SEND_SIZE, temp_buf);
            if(ret == CMD_ERR_OK) {
                DPRINTF(LOG_INFO, "send ok");
                send_count -= RAK_MAX_PAYLOAD_SEND_SIZE;
            } else if(ret < 0) {
                if(ret == CMD_ERR_PEND) {
                    //								DPRINTF(LOG_INFO, "cmd pend");
                } else if(ret == CMD_ERR_SOCK_INVALID) {
                    DPRINTF(LOG_ERROR, "fd %d invalid", app_ctx.ludps);
                    app_ctx.ludps = FD_INVALID;
                }	else if(ret == CMD_ERR_TIMEOUT) {
                    DPRINTF(LOG_ERROR, "cmd timeout");
                } else {
                    DPRINTF(LOG_INFO, "send error %d", ret);
                }
            }
        }
    }
}
