/**
* @file 		rak_global.h
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
#ifndef _RAKGLOBAL_H_
#define _RAKGLOBAL_H_

#include "stdio.h"

typedef unsigned char	uint8;
typedef unsigned short	uint16;
typedef unsigned long	uint32;
typedef signed char	    int8;
typedef signed short	int16;
typedef signed long	     int32;


/*********************  Interface Definition  ***********************************/

/********************** The AT command definition **********************************/

/**************************·ûºÅ*******************************************/
#define    RAK_RECVDATA					  "at+recv_data="

/**
 * 0:TCP 1:LTCP 2:UDP 3:LUDP  4:multicast
 */
#define RAK_TCP_SOCKET                     0
#define RAK_LTCP_SOCKET                    1
#define RAK_UDP_SOCKET                     2
#define RAK_LUDP_SOCKET                    3
#define RAK_MULTI_SOCKET                   4

/**
 * limit
 */
#define RAK_AP_SCANNED_MAX		        10	       ///< maximum number of scanned acces points. 
#define RAK_SSID_LEN			        		33	     ///< maximum length of SSID.
#define RAK_BSSID_LEN			        		6	     ///< maximum length of SSID.
#define RAK_PSK_LEN										65
#define RAK_MAX_TCP_CLIENTS						(3)
#define RAK_MAX_PAYLOAD_SIZE					1024     ///< maximum recieve data payload size.
#define RAK_MAX_PAYLOAD_SEND_SIZE			1400

/*
 * Main struct which defines all the paramaters of the API library
 * This structure is defined as a variable, the  pointer to, which is passed
 *  toall the functions in the API
 * The struct is initialized by a set of #defines for default values
 *  or for simple use cases
 */

// The scan command argument union/variables

typedef union {
    struct {
        uint8			channel;			    					///< RF channel to scan, 0=All, 1-13 for 2.5GHz.
        uint8			ssid[RAK_SSID_LEN];					///< uint8[32], ssid to scan for.
    } scanFrameSnd;														///< scanFrameSnd.
    uint8			uScanBuf[RAK_SSID_LEN + 1];			///< byte format to send to the spi interface, 36 bytes.
} rak_uScan;

typedef union {
    struct {
        uint8			securityType ;							///< 0:no sec  1:sec.
        uint8			psk[RAK_PSK_LEN];						///< pre-shared key, 32-byte string.
        uint8			ssid[RAK_SSID_LEN];	    		///< ssid of access point to join to, 33-byte string.
    } joinFrameSnd;														///< joinFrameSnd.
    uint8			uJoinBuf[RAK_SSID_LEN + RAK_PSK_LEN + 1];	///< joinFrameSnd container
} rak_uJoin;

typedef union {
    struct {
        uint8			nwType;								///< 0 sta 1 AP.
        uint8			securityType ;				///< 0:no sec  1:sec.
        uint8			ssid[RAK_SSID_LEN];	  ///< ssid of access point to join to, 33-byte string.
        uint8			apMode;								///< ap mode if hide.
        uint8			ibssApChannel;				///< rf channel number for AP mode.
    } apFrameSnd;												///< apFrameSnd.
    uint8			uApBuf[RAK_SSID_LEN + 4];	///< apFrameSnd container.
} rak_uAp;

typedef union {
    struct {
        uint8			dhcpMode;		        	///< 0=dhcp client, 1=ip static
        uint8			ipaddr[4];						///< IP address of this module if in manual mode
        uint8			netmask[4];		      	///< Netmask used if in manual mode
        uint8			gateway[4];						///< IP address of default gateway if in manual mode
        uint8     dnssvr1[4];						///< dns1
        uint8     dnssvr2[4];						///< dns2
        uint8			padding;							///< padding
    } ipparamFrameSnd;									///< ipparamFrameSnd
    uint8			uIpparamBuf[22];					///< ipparamFrameSnd container
} rak_uIpparam;

typedef union {
		struct {
				uint8 		src_mac[13];					///< src mac filter
				uint8 		dst_mac[13];					///< dst mac filter
				int8			rssi;									///< rssi filter
				uint16 		channel;							///< channel filter bit0-12 present channel 1-13
		} filterFrameSnd;
		uint8			uFilterBuf[30];						///< filterFrameSnd container
} rak_uFilter;

typedef struct {
    rak_uScan			uScanFrame;						///< scan param struct.
    rak_uJoin			uJoinFrame;						///< join param struct.
    rak_uAp				uApFrame;							///< ap param struct.
    rak_uIpparam	uIpparamFrame;				///< ip param struct.
		rak_uFilter		uFilterFrame;					///< promisc filter struct
} rak_api;

typedef union {
    struct {
        uint8			rspCode[5];						///< error response header.
        int8      errorCode;						///< error response code.
        uint8	 		end[2];								///< error response end.
    } errorframe;												///< errorframe
    uint8			errorFrameRcv[8];					///< errorframe container.
} rak_errorResponse;

typedef struct {
    uint8			ssid[RAK_SSID_LEN];				///< 33-byte ssid of scanned access point.
    uint8			bssid[RAK_BSSID_LEN];			///< 6-byte bssid of scanned access point.
    uint8			rfChannel;								///< rf channel to us, 0=scan for all.
    uint8			rssiVal;									///< absolute value of RSSI.
    uint8			securityMode;							///< security mode.
} rak_getscanInfo;

typedef  union {
    struct {
        uint8			rspCode[2];  			  	///< 0= success	   !0= Failure.
        uint8			scanCount;						///< uint8, number of access points found.
        uint8	 		end[2];								///< end
    } scanframe;												///< scanframe
    uint8			scanFrameRcv[6]  ;				///< scanframe container
} rak_scanResponse;

typedef  union {
    struct {
        uint8			          rspCode[2];						///< response head
        rak_getscanInfo			strScanInfo[RAK_AP_SCANNED_MAX];	///< 32 maximum responses from scan command.
        uint8	 		          end[2];								///< response end
    } getscanframe;											///< getscanframe
    uint8			getscanFrameRcv[44]  ;		///< getscanframe container
} rak_getscanResponse;

typedef union {
    struct {
        uint8			rspCode[2];						///< response head
        uint8			status;								///< con status
        uint8	 		end[2];								///< response end
    } qryconFrame;											///< qryconFrame
    uint8			qryconFrameRcv[6];				///< qryconFrame container
} rak_qryconResponse;

typedef  union {
    struct {
        uint8			rspCode[2];						///< response head
        uint8			macAddr[6];						///< MAC address of this module.
        uint8			ipAddr[4];						///< Configured IP address.
        uint8			netMask[4];						///< Configured netmask.
        uint8			gateWay[4];						///< Configured default gateway.
        uint8			dns1[4];							///< dns1.
        uint8			dns2[4];							///< dns2.
        uint8	 		end[2];								///< response end
    } ipconfigframe;										///< ipconfigframe
    uint8			ipconfigFrameRcv[30];			///< ipconfigframe container
} rak_ipconfigResponse;

typedef union {
    struct {
        uint8    	rspCode[2];           ///< response head
        uint8     socketDescriptordata;	///< socket fd
        uint8	 		end[2];								///< response end
    } socketframe;											///< socketframe
    uint8			socketFrameRcv[6]  ;			///< socketframe container
} rak_socketFrameRcv;

typedef  union {

    struct {
        uint8   	CMDCode[13];									///< "at+recv_data="
        uint8			socketDescriptor;							///< socket fd
        uint8			destPort[2];									///< dest port
        uint8			destIp[4];										///< dest ip
        uint8			recDataLen[2];								///< data len
        uint8     recvdataBuf[RAK_MAX_PAYLOAD_SIZE];	///< recv buf
        uint8	 		end[2];												///< response end
    } recvdataframe;														///< recvdataframe
    struct {
        uint8			CMDCode[13]; 		   						///< "at+recv_data="
        uint8			rspCode;											///< 0x01 TCP/UCP 0xFF recieve error 0x80 TCP con	0x81 TCP dis 0x82 0x83..
        uint8			socketDescriptor;							///< socket fd
        uint8			destPort[2];									///< dest port
        uint8			destIp[4];										///< dest ip
        uint8	 		end[2];												///< reponse end
    } recvstatuframe;														///< recvstatuframe
    uint8			socketFrameRcv[RAK_MAX_PAYLOAD_SIZE+24]  ;			///< recvdataframe or recvstatuframe container
} rak_recvdataFrame;

typedef union {
    struct {
        uint8			rspCode[2];										///< response header
        uint8			mac[6];												///< mac address
        uint8	 		end[2];												///< response end 
    } qryMacframe;															///< qryMacframe
    uint8			qryMacFrameRcv[18];								///< qryMacframe container
} rak_qryMacFrameRcv;

typedef union {
    struct {
        uint8    	rspCode[2];										///< response header
        uint8			hostFwversion[8];							///< firmware version text string, x.y.z as 1.3.0
        uint8			wlanFwversion[6];							///< wlan version
        uint8	 		end[2];												///< response end
    } qryFwversionframe;												///< qryFwversionframe
    uint8			qryFwversionFrameRcv[18];					///< qryFwversionframe container
} rak_qryFwversionFrameRcv;

typedef union {
    struct {
        uint8    	rspCode[2];										///< response header
        uint8			recDataLen[2];								///< read data len
        uint8			recvdataBuf[RAK_MAX_PAYLOAD_SIZE];	///< read data buf
        uint8	 		end[2];												///< response end
    } readSocketFrame;													///< readSocketFrame
    uint8			readSocketFrameRcv[RAK_MAX_PAYLOAD_SIZE +6];			///< readSocketFrame container
} rak_readSocketFrame;

typedef struct {
    uint8			socketDescriptor;						///< socket fd
    uint8			destPort[2];								///< dest port 
    uint8			destIp[4];									///< dest ip
} rak_socketInfo;

typedef  union {
    struct {
        uint8			rspCode[2];							///< response header
        uint8			valid_num;							///< recv clients's num
        rak_socketInfo			socket_info[RAK_MAX_TCP_CLIENTS];	///< store clients's info
        uint8	 		end[2];									///< response end
    } qrySocketStatusFrame;								///< qrySocketStatusFrame
    uint8			qrySocketStatusFrameRcv[44]  ;			///< qrySocketStatusFrame container
} rak_qrySocketStatusFrame;

typedef  union {
    struct {
        uint8			type;										///< packet type
        int8			rssi;										///< packet rssi
        uint16		len;										///< packet len
        uint8	 		src_mac[6];							///< packet src mac
				uint8			dst_mac[6];							///< packet dst mac
				uint8 		end[2];
    } promiscFrame;												///< promiscFrame
    uint8			promiscFrameRcv[18]  ;			///< promiscFrame container
} rak_promiscFrame;

typedef  union {
    struct {
				uint8			rspCode[2];							///< response header
        uint16		len;										///< write len max -- 1024
				int8 			data[1000];
				uint8 		end[2];									///< response end
    } nvmReadFrame;												///< nvmReadFrame
    uint8			nvmReadFrameRcv[1000+6]  ;			///< nvmReadFrame container
} rak_nvmReadFrame;

typedef union {
    uint8                     		rspCode;                 	///< response header
    uint8 												bootInfo[19];						 	///< rak start info
    rak_errorResponse							errorResponse;						///< error res
    rak_qryconResponse            qryconResponse;						///< query con_status res
    rak_ipconfigResponse          ipconfigResponse;					///< query ip info res
    rak_qryMacFrameRcv  		    	qryMacFrameRcv;						///< query mac res	
    rak_socketFrameRcv        		socketFrameRcv;						///< socket frame 
    rak_recvdataFrame          		recvdataFrame;						///< recv data frame	
    rak_readSocketFrame						readSocketFrame;					///< read data frame 
    rak_qrySocketStatusFrame			qrySocketStatusFrame;			///< query socket status res
		rak_promiscFrame							promiscFrame;
	
    uint8					        uCmdRspBuf[2048];
} rak_uCmdRsp;

void rak_asciiDotAddressTo4Bytes(uint8 *hexAddr, int8 *asciiDotAddress, uint8 length);
int8 * rak_bytes4ToAsciiDotAddr(uint8 *hexAddr,uint8 *strBuf);
void rak_hexArrayToStr(char *src, char *dst, uint8 len);

/** ATcommand timeout */

#define RAK_TICKS_PER_SECOND   			1000
#define RAK_EASYCONFIG_TIMEOUT       	90 * RAK_TICKS_PER_SECOND
#define RAK_WPS_TIMEOUT       			90 * RAK_TICKS_PER_SECOND
#define RAK_AUTOCONNECT_TIMEOUT       	60 * RAK_TICKS_PER_SECOND
#define RAK_STARTWEB_TIMEOUT       		100 * RAK_TICKS_PER_SECOND
#define RAK_SETCHANNEL_TIMEOUT       	10 * RAK_TICKS_PER_SECOND
#define RAK_SETPSK_TIMEOUT       		10 * RAK_TICKS_PER_SECOND
#define RAK_APCONFIG_TIMEOUT       		10 * RAK_TICKS_PER_SECOND
#define RAK_CONNECT_TIMEOUT       		60 * RAK_TICKS_PER_SECOND
#define RAK_GETVERSION_TIMEOUT       	10 * RAK_TICKS_PER_SECOND
#define RAK_IPCONFIG_QUERY_TIMEOUT		10 * RAK_TICKS_PER_SECOND
#define RAK_IPSTATIC_TIMEOUT			20 * RAK_TICKS_PER_SECOND
#define RAK_DISCONNECT_TIMEOUT			10 * RAK_TICKS_PER_SECOND
#define RAK_PING_TIMEOUT       			10 * RAK_TICKS_PER_SECOND
#define RAK_DHCP_TIMEOUT       			100 * RAK_TICKS_PER_SECOND
#define RAK_DNS_IMEOUT       			10 * RAK_TICKS_PER_SECOND
#define RAK_SCAN_IMEOUT       			30 * RAK_TICKS_PER_SECOND
#define RAK_GETSCAN_IMEOUT       		10 * RAK_TICKS_PER_SECOND
#define RAK_GETNET_TIMEOUT       		10 * RAK_TICKS_PER_SECOND
#define RAK_GETRSSI_TIMEOUT       		10 * RAK_TICKS_PER_SECOND
#define RAK_STORECONFIG_TIMEOUT       	10 * RAK_TICKS_PER_SECOND
#define RAK_STOREWEB_TIMEOUT       		10 * RAK_TICKS_PER_SECOND
#define RAK_OPENSOCKET_TIMEOUT       	50 * RAK_TICKS_PER_SECOND
#define RAK_SETLISTEN_TIMEOUT       	10 * RAK_TICKS_PER_SECOND
#define RAK_CLOSESOCKET_TIMEOUT       	10 * RAK_TICKS_PER_SECOND
#define RAK_STOREDATA_TIMEOUT       	10 * RAK_TICKS_PER_SECOND

#define RAK_SETASCII_TIMEOUT       		10 * RAK_TICKS_PER_SECOND
#define RAK_GETMAC_TIMEOUT       		10 * RAK_TICKS_PER_SECOND
#define RAK_SENDATA_TIMEOUT					10 * RAK_TICKS_PER_SECOND
#define RAK_SOCKSTATUS_TIMEOUT					10 * RAK_TICKS_PER_SECOND
#define RAK_SOCKDATA_TIMEOUT					10 * RAK_TICKS_PER_SECOND
#define RAK_SETFUNCBITMAP_TIMEOUT       		10 * RAK_TICKS_PER_SECOND
#define RAK_GETSTORECONFIG_TIMEOUT       	10 * RAK_TICKS_PER_SECOND

#define RAK_SETFILTER_TIMEOUT       	10 * RAK_TICKS_PER_SECOND
#define RAK_STARTPROMISC_TIMEOUT       	10 * RAK_TICKS_PER_SECOND
#define RAK_STOPPROMISC_TIMEOUT       	10 * RAK_TICKS_PER_SECOND
#define RAK_NVMWRITE_TIMEOUT       	10 * RAK_TICKS_PER_SECOND
#define RAK_NVMREAD_TIMEOUT       	10 * RAK_TICKS_PER_SECOND
#endif

