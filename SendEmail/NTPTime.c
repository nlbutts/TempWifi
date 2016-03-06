#include <stdio.h>
#include <stdint.h>
#include <time.h>

// simplelink includes
#include "simplelink.h"
#include "common.h"
#include "network_if.h"
#include "uart_if.h"


#define TIME2013                3565987200u      /* 113 years + 28 days(leap) */
#define YEAR2013                2013
#define SEC_IN_MIN              60
#define SEC_IN_HOUR             3600
#define SEC_IN_DAY              86400
#define SERVER_RESPONSE_TIMEOUT 10


//*****************************************************************************
//
//! Gets the current time from the selected SNTP server
//!
//! \brief  This function obtains the NTP time from the server.
//!
//! \param  data pointer to the NTP data
//!
//! \return 0 : success, -ve : failure
//!
//
//*****************************************************************************
static void DecodeSNTPTime(char * cDataBuf, struct tm * decodedTime)
{
  
/*
                            NTP Packet Header:


       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9  0  1
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |LI | VN  |Mode |    Stratum    |     Poll      |   Precision    |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                          Root  Delay                           |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                       Root  Dispersion                         |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                     Reference Identifier                       |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                                                                |
      |                    Reference Timestamp (64)                    |
      |                                                                |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                                                                |
      |                    Originate Timestamp (64)                    |
      |                                                                |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                                                                |
      |                     Receive Timestamp (64)                     |
      |                                                                |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                                                                |
      |                     Transmit Timestamp (64)                    |
      |                                                                |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                 Key Identifier (optional) (32)                 |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                                                                |
      |                                                                |
      |                 Message Digest (optional) (128)                |
      |                                                                |
      |                                                                |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
    unsigned long ulElapsedSec = 0;
    short isGeneralVar;
    unsigned long ulGeneralVar;
    unsigned long ulGeneralVar1;
    char acTimeStore[30];
    char *pcCCPtr;
    unsigned short uisCCLen;

    // Tuesday is the 1st day in 2013 - the relative year
    const char g_acDaysOfWeek2013[7][3] = {{"Tue"},
                                            {"Wed"},
                                            {"Thu"},
                                            {"Fri"},
                                            {"Sat"},
                                            {"Sun"},
                                            {"Mon"}};

    const char g_acMonthOfYear[12][3] = {{"Jan"},
                                          {"Feb"},
                                          {"Mar"},
                                          {"Apr"},
                                          {"May"},
                                          {"Jun"},
                                          {"Jul"},
                                          {"Aug"},
                                          {"Sep"},
                                          {"Oct"},
                                          {"Nov"},
                                          {"Dec"}};

    const char g_acNumOfDaysPerMonth[12] = {31, 28, 31, 30, 31, 30,
                                        31, 31, 30, 31, 30, 31};

    //
    // Confirm that the MODE is 4 --> server
    //
    if ((cDataBuf[0] & 0x7) == 4)    // expect only server response
    {
        unsigned char iIndex;

        //
        // Getting the data from the Transmit Timestamp (seconds) field
        // This is the time at which the reply departed the
        // server for the client
        //
        ulElapsedSec = cDataBuf[40];
        ulElapsedSec <<= 8;
        ulElapsedSec += cDataBuf[41];
        ulElapsedSec <<= 8;
        ulElapsedSec += cDataBuf[42];
        ulElapsedSec <<= 8;
        ulElapsedSec += cDataBuf[43];

        //
        // seconds are relative to 0h on 1 January 1900
        //
        ulElapsedSec -= TIME2013;

        pcCCPtr = &acTimeStore[0];

        //
        // day, number of days since beginning of 2013
        //
        isGeneralVar = ulElapsedSec/SEC_IN_DAY;
        memcpy(pcCCPtr,
               g_acDaysOfWeek2013[isGeneralVar%7], 3);
        decodedTime->tm_wday = (isGeneralVar + 2) % 7;
        pcCCPtr += 3;
        *pcCCPtr++ = '\x20';

        //
        // month
        //
        isGeneralVar %= 365;
        for (iIndex = 0; iIndex < 12; iIndex++)
        {
            isGeneralVar -= g_acNumOfDaysPerMonth[iIndex];
            if (isGeneralVar < 0)
                    break;
        }
        if(iIndex == 12)
        {
            iIndex = 0;
        }
        memcpy(pcCCPtr, g_acMonthOfYear[iIndex], 3);
        decodedTime->tm_mon = iIndex;
        pcCCPtr += 3;
        *pcCCPtr++ = '\x20';

        //
        // date
        // restore the day in current month
        //
        isGeneralVar += g_acNumOfDaysPerMonth[iIndex];
        uisCCLen = itoa(isGeneralVar + 1, pcCCPtr);
        pcCCPtr += uisCCLen;
        *pcCCPtr++ = '\x20';
        decodedTime->tm_mday = isGeneralVar + 1;

        //
        // time
        //
        ulGeneralVar = ulElapsedSec%SEC_IN_DAY;

        // number of seconds per hour
        ulGeneralVar1 = ulGeneralVar%SEC_IN_HOUR;

        // number of hours
        ulGeneralVar /= SEC_IN_HOUR;
        uisCCLen = itoa(ulGeneralVar,
                                   pcCCPtr);
        pcCCPtr += uisCCLen;
        *pcCCPtr++ = ':';
        decodedTime->tm_hour = ulGeneralVar;

        // number of minutes per hour
        ulGeneralVar = ulGeneralVar1/SEC_IN_MIN;

        // number of seconds per minute
        ulGeneralVar1 %= SEC_IN_MIN;
        uisCCLen = itoa(ulGeneralVar,
                                   pcCCPtr);
        pcCCPtr += uisCCLen;
        *pcCCPtr++ = ':';
        uisCCLen = itoa(ulGeneralVar1,
                                   pcCCPtr);
        pcCCPtr += uisCCLen;
        *pcCCPtr++ = '\x20';
        decodedTime->tm_min = ulGeneralVar;
        decodedTime->tm_sec = ulGeneralVar1;

        //
        // year
        // number of days since beginning of 2013
        //
        ulGeneralVar = ulElapsedSec/SEC_IN_DAY;
        ulGeneralVar /= 365;
        uisCCLen = itoa(YEAR2013 + ulGeneralVar,
                                   pcCCPtr);
        pcCCPtr += uisCCLen;

        *pcCCPtr++ = '\0';
        decodedTime->tm_year = (YEAR2013 + ulGeneralVar) - 1900;
        decodedTime->tm_isdst = 0;

        UART_PRINT("response from server: ");
        UART_PRINT(acTimeStore);
        UART_PRINT("\n\r\n\r");
    }
}

//*****************************************************************************
//
//! \brief     Get the NTP time
//!
//! \param    none
//!
//! \return void
//! \note
//! \warning
//
//*****************************************************************************
uint8_t GetTime(struct tm * decodedTime)
{
    const char * g_acSNTPserver = "wwv.nist.gov"; //Add any one of the above servers
    unsigned long ulDestinationIP;
    int iSocketDesc;
    long lRetVal;
    char cDataBuf[48];
    int iAddrSize;
    SlSockAddr_t sAddr;
    SlSockAddrIn_t sLocalAddr;

    //
    // Create UDP socket
    //
    iSocketDesc = sl_Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(iSocketDesc < 0)
    {
        ERR_PRINT(iSocketDesc);
        return 0;
    }
    //g_sAppData.iSockID = iSocketDesc;

    UART_PRINT("Socket created\n\r");

    //
    // Get the NTP server host IP address using the DNS lookup
    //
    lRetVal = Network_IF_GetHostIP((char*)g_acSNTPserver, &ulDestinationIP);

    if( lRetVal >= 0)
    {

        struct SlTimeval_t timeVal;
        timeVal.tv_sec =  SERVER_RESPONSE_TIMEOUT;    // Seconds
        timeVal.tv_usec = 0;     // Microseconds. 10000 microseconds resolution
        lRetVal = sl_SetSockOpt(iSocketDesc,SL_SOL_SOCKET,SL_SO_RCVTIMEO,\
                        (unsigned char*)&timeVal, sizeof(timeVal));
        if(lRetVal < 0)
        {
            ERR_PRINT(lRetVal);
            //
            // Close the socket
            //
            close(iSocketDesc);
            UART_PRINT("Socket closed\n\r");
            return 0;
        }

        //
        // Send a query ? to the NTP server to get the NTP time
        //
        memset(cDataBuf, 0, sizeof(cDataBuf));
        cDataBuf[0] = '\x1b';

        sAddr.sa_family = AF_INET;
        // the source port
        sAddr.sa_data[0] = 0x00;
        sAddr.sa_data[1] = 0x7B;    // UDP port number for NTP is 123
        sAddr.sa_data[2] = (char)((ulDestinationIP>>24)&0xff);
        sAddr.sa_data[3] = (char)((ulDestinationIP>>16)&0xff);
        sAddr.sa_data[4] = (char)((ulDestinationIP>>8)&0xff);
        sAddr.sa_data[5] = (char)(ulDestinationIP&0xff);

        lRetVal = sl_SendTo(iSocketDesc,
                         cDataBuf,
                         sizeof(cDataBuf), 0,
                         &sAddr, sizeof(sAddr));
        if (lRetVal != sizeof(cDataBuf))
        {
            // could not send SNTP request
            //ASSERT_ON_ERROR(SERVER_GET_TIME_FAILED);
            return 0;
        }

        //
        // Wait to receive the NTP time from the server
        //
        sLocalAddr.sin_family = SL_AF_INET;
        sLocalAddr.sin_port = 0;
        sLocalAddr.sin_addr.s_addr = 0;
        lRetVal = sl_Bind(iSocketDesc,
                (SlSockAddr_t *)&sLocalAddr,
                sizeof(SlSockAddrIn_t));

        iAddrSize = sizeof(SlSockAddrIn_t);

        lRetVal = sl_RecvFrom(iSocketDesc,
                           cDataBuf, sizeof(cDataBuf), 0,
                           (SlSockAddr_t *)&sLocalAddr,
                           (SlSocklen_t*)&iAddrSize);
        //ASSERT_ON_ERROR(lRetVal);

        if(lRetVal < 0)
        {
            UART_PRINT("Server Get Time failed\n\r");
            //
            // Close the socket
            //
            close(iSocketDesc);
            UART_PRINT("Socket closed\n\r");
            return 0;
        }

        // We received the time
        DecodeSNTPTime(cDataBuf, decodedTime);
    }
    else
    {
        UART_PRINT("DNS lookup failed. \n\r");
    }

    //
    // Close the socket
    //
    close(iSocketDesc);
    UART_PRINT("Socket closed\n\r");
    return 1;
}

void testTime()
{
  time_t now1, now2;
  struct tm * tmPtr;
  now1 = time(NULL);
  tmPtr = gmtime(&now1);
  UART_PRINT("Time now = %u\n", now1);
  UART_PRINT("Time= %s\n", asctime(tmPtr));
  UART_PRINT("tm_sec   %d\n", tmPtr->tm_sec  );
  UART_PRINT("tm_min   %d\n", tmPtr->tm_min  );
  UART_PRINT("tm_hour  %d\n", tmPtr->tm_hour );
  UART_PRINT("tm_mday  %d\n", tmPtr->tm_mday );
  UART_PRINT("tm_mon   %d\n", tmPtr->tm_mon  );
  UART_PRINT("tm_year  %d\n", tmPtr->tm_year );
  UART_PRINT("tm_wday  %d\n", tmPtr->tm_wday );
  UART_PRINT("tm_yday  %d\n", tmPtr->tm_yday );
  UART_PRINT("tm_isdst %d\n", tmPtr->tm_isdst);

  struct tm tmNow;
  
  tmNow.tm_sec   = 0;
  tmNow.tm_min   = 26;
  tmNow.tm_hour  = 19;
  tmNow.tm_mday  = 2;
  tmNow.tm_mon   = 2;
  tmNow.tm_year  = 116;
  tmNow.tm_wday  = 0;
  tmNow.tm_yday  = 0;
  tmNow.tm_isdst = 0;
  now2 = mktime(&tmNow);
  UART_PRINT("Converted time now = %u\n",  now2);
  UART_PRINT("now1-now2 = %u", now1-now2);
}
