/* --COPYRIGHT--,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/

/*
 * ======== UsbCdc.c ========
 */
#include "usbcdc.h"
#include "usbcdc_event_handlers.h"

#ifdef _CDC_

//----------------------------------------------------------------------------
//Line Coding Structure
//dwDTERate     | 4 | Data terminal rate, in bits per second
//bCharFormat   | 1 | Stop bits, 0 = 1 Stop bit, 1 = 1,5 Stop bits, 2 = 2 Stop bits
//bParityType   | 1 | Parity, 0 = None, 1 = Odd, 2 = Even, 3= Mark, 4 = Space
//bDataBits     | 1 | Data bits (5,6,7,8,16)
//----------------------------------------------------------------------------
uint8_t usbGetLineCoding (void)
{
    uint8_t infIndex;

    if(tSetupPacket.wIndex % 2)
    {
        infIndex = (tSetupPacket.wIndex-1) / 2;
    }
    else
    {
        infIndex = (tSetupPacket.wIndex) / 2;
    }

    abUsbRequestReturnData[6] = CdcControl[infIndex].bDataBits;          //Data bits = 8
    abUsbRequestReturnData[5] = CdcControl[infIndex].bParity;            //No Parity
    abUsbRequestReturnData[4] = CdcControl[infIndex].bStopBits;          //Stop bits = 1
    abUsbRequestReturnData[3] = CdcControl[infIndex].lBaudrate >> 24;
    abUsbRequestReturnData[2] = CdcControl[infIndex].lBaudrate >> 16;
    abUsbRequestReturnData[1] = CdcControl[infIndex].lBaudrate >> 8;
    abUsbRequestReturnData[0] = CdcControl[infIndex].lBaudrate;

    wBytesRemainingOnIEP0 = 0x07;                                        //amount of data to be send over EP0 to host
    usbSendDataPacketOnEP0((uint8_t*)&abUsbRequestReturnData[0]);        //send data to host

    return (FALSE);
}

//----------------------------------------------------------------------------

uint8_t usbSetLineCoding (void)
{
    usbReceiveDataPacketOnEP0((uint8_t*)&abUsbRequestIncomingData);      //receive data over EP0 from Host
    return (FALSE);
}

//----------------------------------------------------------------------------

uint8_t usbSetControlLineState (void)
{
	USBCDC_handleSetControlLineState((uint8_t)tSetupPacket.wIndex,
                                     (uint8_t)tSetupPacket.wValue);
    usbSendZeroLengthPacketOnIEP0();                                    //Send ZLP for status stage
    return (FALSE);
}

//----------------------------------------------------------------------------

uint8_t Handler_SetLineCoding (void)
{
    uint8_t bWakeUp;
    volatile uint8_t infIndex;

    if(tSetupPacket.wIndex % 2)
    {
        infIndex = (tSetupPacket.wIndex-1) / 2;
    }
    else
    {
        infIndex = (tSetupPacket.wIndex) / 2;
    }

    //Baudrate Settings
    CdcControl[infIndex].lBaudrate =
        (uint32_t)abUsbRequestIncomingData[3] << 24 |
        (uint32_t)abUsbRequestIncomingData[2] << 16 |
        (uint32_t)abUsbRequestIncomingData[1] << 8  | 
                  abUsbRequestIncomingData[0];
                  
    bWakeUp = USBCDC_handleSetLineCoding(tSetupPacket.wIndex, CdcControl[infIndex].lBaudrate);
    return (bWakeUp);
}

#endif


/*----------------------------------------------------------------------------+
 | End of source file                                                          |
 +----------------------------------------------------------------------------*/
/*------------------------ Nothing Below This Line --------------------------*/
//Released_Version_5_10_00_17
