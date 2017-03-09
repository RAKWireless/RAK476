/**
* @file 		rak_config.h
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
#ifndef _RAKCONFIG_H_
#define _RAKCONFIG_H_

#define RAK_AP_STATION_MODE         0				       			///< 0 station  1 AP

#define RAK_SCAN_SSID			    			"RAK_2.4GHz" 				///< null string ("") scans all ssids
#define RAK_CONNECT_SSID						"RAK_2.4GHz"				///< SSID to join to in 2.4GHz 
#define RAK_SCAN_CHANNEL		    		0				            ///< 0 scans all channels	1-11

#define RAK_GET_SCAN_NUM		    		3			   						///< get scan result number

#define RAK_PSK				       	 			"rakwireless205"  	///< If we are using WPA2, this is the key
#define RAK_CREAT_CHANNEL		    		9										///< ap channel

#define RAK_CREAT_AP_SSID       		"RAK473_UART_TEST"	///< ap name
#define RAK_AP_BRODCAST_ENABLE      0                   ///< 0: no hidden	 1:hidden

#define RAK_IPDHCP_MODE_ENABLE			0	   			  				///< station 0--ipdhcp 1--ipstatic; AP 0--disable dhcp 1--enable dhcp
#define RAK_IPSTATIC_IP_ADDRESS			"192.168.7.10"
#define RAK_IPSTATIC_GATEWAY				"192.168.7.1"
#define RAK_IPSTATIC_NETMASK				"255.255.255.0"
#define RAK_IPSTATIC_DNS1						"0"
#define RAK_IPSTATIC_DNS2						"0"

#define RAK_DOMAIN_NAME            	"www.lthonway.com" 	///< set the domain name for which dns is requested

#endif
