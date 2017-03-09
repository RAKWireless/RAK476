/**
* @file 		rak_config_init.c
* @brief		this is a config file for uart wifi module.
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
 *  brief.			Initialize the global parameter structure
 * @param[in]		rsi_api *ptrStrApi, pointer to the global parameter structure
 * @param[out]	none
 * @return			status
 *	        		0  = SUCCESS
 * @description	This function is used to initialize the global parameter structure with parameters
 * 		used to configure the Wi-Fi module.
 */
uint16 rak_init_struct(rak_api *ptrStrApi)
{
    /* Scan Parameters */
    ptrStrApi->uScanFrame.scanFrameSnd.channel = RAK_SCAN_CHANNEL;
    strcpy((char *)&ptrStrApi->uScanFrame.scanFrameSnd.ssid,RAK_SCAN_SSID);

    /* Join Parameters */
    strcpy((char *)&ptrStrApi->uJoinFrame.joinFrameSnd.psk,  RAK_PSK);
    strcpy((char *)&ptrStrApi->uJoinFrame.joinFrameSnd.ssid, RAK_CONNECT_SSID);

    /* Ap Parameters */
    ptrStrApi->uApFrame.apFrameSnd.ibssApChannel = RAK_CREAT_CHANNEL;
    strcpy((char *)&ptrStrApi->uApFrame.apFrameSnd.ssid, RAK_CREAT_AP_SSID);
    ptrStrApi->uApFrame.apFrameSnd.apMode = RAK_AP_BRODCAST_ENABLE;
	
    /* IP Parameters */
    ptrStrApi->uIpparamFrame.ipparamFrameSnd.dhcpMode = RAK_IPDHCP_MODE_ENABLE;
    rak_asciiDotAddressTo4Bytes((uint8 *)&ptrStrApi->uIpparamFrame.ipparamFrameSnd.ipaddr, (int8 *)	RAK_IPSTATIC_IP_ADDRESS,sizeof(RAK_IPSTATIC_IP_ADDRESS));
    rak_asciiDotAddressTo4Bytes((uint8 *)&ptrStrApi->uIpparamFrame.ipparamFrameSnd.netmask, (int8 *)RAK_IPSTATIC_NETMASK,sizeof(RAK_IPSTATIC_NETMASK));
    rak_asciiDotAddressTo4Bytes((uint8 *)&ptrStrApi->uIpparamFrame.ipparamFrameSnd.gateway, (int8 *)RAK_IPSTATIC_GATEWAY,sizeof(RAK_IPSTATIC_GATEWAY));
    rak_asciiDotAddressTo4Bytes((uint8 *)&ptrStrApi->uIpparamFrame.ipparamFrameSnd.dnssvr1, (int8 *)RAK_IPSTATIC_DNS1,sizeof(RAK_IPSTATIC_DNS1));
    rak_asciiDotAddressTo4Bytes((uint8 *)&ptrStrApi->uIpparamFrame.ipparamFrameSnd.dnssvr2, (int8 *)RAK_IPSTATIC_DNS2,sizeof(RAK_IPSTATIC_DNS2));
		
		/* promisc filter */
		strcpy((char*)ptrStrApi->uFilterFrame.filterFrameSnd.src_mac, "8c210ad9eb7b");
		strcpy((char*)ptrStrApi->uFilterFrame.filterFrameSnd.dst_mac, "ffffffffffff");
		ptrStrApi->uFilterFrame.filterFrameSnd.rssi = -90;
		ptrStrApi->uFilterFrame.filterFrameSnd.channel = 0;
    return 0;
}
