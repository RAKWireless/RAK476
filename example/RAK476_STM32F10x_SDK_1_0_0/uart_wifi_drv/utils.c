/**
* @file 		utils.c
* @brief		this is a file for data formation.
* @details	This is the detail description.
* @author		harry
* @date			2016/1/23
* @version	1.0.0
* @par Copyright (c):
* 					rakwireless
* @par History:
*	version: author, date, desc\n
*/
#include "rak_global.h"

/**
 *  brief.			Convert an uint8 4-Byte array to  . notation network address
 * @param[in]		uint8 *hexAddress, Address to convert
 * @param[in]		uint8 *strBuf, String Pointer to hold the Concatenated String
 * @param[out]		none
 * @return			char * destString, pointer to the string with the data
 */
int8 * rak_bytes4ToAsciiDotAddr(uint8 *hexAddr,uint8 *strBuf)
{
    uint8			i;							// loop counter
    uint8			ii;							// loop counter
    int8			cBuf[4];						// character buffer
    int8			*destString;

    strBuf[0] = 0;								// make  strcat think the array is empty
    for (i = 0; i < 3; i++) {							// we are assuming IPV4, so 4 bytes
        // This will take care of the first 3 bytes
        // zero out the character buffer since we don't know how long the string will be
        for(ii = 0; ii < sizeof(cBuf); ii++) {
            cBuf[ii] = 0;
        }
        sprintf ((char *)cBuf, "%d", (unsigned int)(((uint8*)hexAddr)[i]));
        destString =(int8 *) strcat((char *)strBuf,(char *) cBuf);
        destString = (int8 *)strcat((char *)strBuf,(char *) ".");
    }
    // take care of the last entry outside the loop, there is no . after the last octet
    // zero out the character buffer since we don't know how long the string will be
    for(ii = 0; ii < sizeof(cBuf); ii++) {
        cBuf[ii] = 0;
    }
    sprintf ((char *)cBuf, "%d", (unsigned int)(((uint8*)hexAddr)[i]));
    destString = (int8 *) strcat((char *)strBuf,(char *) cBuf);
    return destString;
}
/**
 *  brief.		Convert an ASCII . notation network address to 4-byte hex address
 * @param[in]	int8 *asciiDotFormatAddress, source address to convert, must be a null terminated string
 * @param[out]	uint8 *hexAddr,	Output value is passed back in the 4-byte Hex Address
 * @return		none
 */
void rak_asciiDotAddressTo4Bytes(uint8 *hexAddr, int8 *asciiDotAddress, uint8 length)
{
    uint8			i;
    // loop counter
    uint8			cBufPos;
    // which char in the ASCII representation
    uint8			byteNum;
    // which byte in the 32BitHexAddress
    uint8			cBuf[4];
    // character buffer

    byteNum = 0;
    cBufPos = 0;
    for (i = 0; i < length; i++) {
        // this will take care of the first 3 octets
        if (asciiDotAddress[i] == '.')
        {
            // we are at the end of the address octet
            cBuf[cBufPos] = 0;
            // terminate the string
            cBufPos = 0;
            // reset for the next char
            hexAddr[byteNum++] = (uint8)atoi((char *)cBuf);
            // convert the strint to an integer
        }
        else {
            cBuf[cBufPos++] = asciiDotAddress[i];
        }
    }
    // handle the last octet
    // // we are at the end of the string with no .
    cBuf[cBufPos] = 0x00;
    // terminate the string
    hexAddr[byteNum] = (uint8)atoi((char *)cBuf);

    // convert the strint to an integer
}

/**
 *  brief.		turn hex array to ascii ex:	{0x11,0x22,0x33} to "11 22 33"
 * @param[in]	src		source hex buf
 * @param[in]	len		hex data len
 * @param[out]	dst		dst buf
 * @return		none
 */
void rak_hexArrayToStr(char *src, char *dst, uint8 len)
{
    char *p = dst;
    int i=0;
    for(i=0; i<len; i++) {
        sprintf(p, "%02x ", src[i]);
        p += 3;
    }
}
