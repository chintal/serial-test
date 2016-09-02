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
#include "../../descriptors.h"

#ifdef _CDC_


#include "../USB_Common/device.h"
#include "../USB_Common/defMSP430USB.h"
#include "../USB_Common/usb.h"                  //USB-specific Data Structures

#include "usbcdc.h"
#include "usbcdc_event_handlers.h"

#include <string.h>
#define EP_MAX_PACKET_SIZE_CDC      0x40

CdcControl_t         CdcControl  [CDC_NUM_INTERFACES];
CdcWriteCtrl_t       CdcWriteCtrl[CDC_NUM_INTERFACES];
CdcWriteBufferCtrl_t CdcWriteBufferCtrl[CDC_NUM_INTERFACES * 2];
CdcReadCtrl_t        CdcReadCtrl [CDC_NUM_INTERFACES];
CdcReadBufferCtrl_t  CdcReadBufferCtrl[CDC_NUM_INTERFACES * 2];

extern uint16_t wUsbEventMask;

extern void *(*USB_TX_memcpy)(void * dest, const void * source, size_t count);
extern void *(*USB_RX_memcpy)(void * dest, const void * source, size_t count);

/*----------------------------------------------------------------------------+
 | Global Variables                                                            |
 +----------------------------------------------------------------------------*/

extern __no_init tEDB __data16 tInputEndPointDescriptorBlock[];
extern __no_init tEDB __data16 tOutputEndPointDescriptorBlock[];

void USBCDC_resetWriteCtrl(uint8_t intfNum)
{
    CdcWriteCtrl[intfNum].sCurrentInBuffer = &CdcWriteBufferCtrl[2*intfNum];
    CdcWriteCtrl[intfNum].sCurrentInBuffer->bAvailable = TRUE;
    CdcWriteCtrl[intfNum].sCurrentInBuffer->pStart = 
            (uint8_t*)stUsbHandle[intfNum].iep_X_Buffer;
    CdcWriteCtrl[intfNum].sCurrentInBuffer->pNext = 
            (uint8_t*)stUsbHandle[intfNum].iep_X_Buffer;
    CdcWriteCtrl[intfNum].sCurrentInBuffer->pEnd = 
            (uint8_t*)stUsbHandle[intfNum].iep_X_Buffer + EP_MAX_PACKET_SIZE_CDC;
    CdcWriteCtrl[intfNum].sCurrentInBuffer->pEpbcnt = 
            &(tInputEndPointDescriptorBlock[stUsbHandle[intfNum].edb_Index].bEPBCTX);
    *(CdcWriteCtrl[intfNum].sCurrentInBuffer->pEpbcnt) = 0;
    
    CdcWriteCtrl[intfNum].sNextInBuffer = &CdcWriteBufferCtrl[2*intfNum + 1];
    CdcWriteCtrl[intfNum].sNextInBuffer->bAvailable = TRUE;
    CdcWriteCtrl[intfNum].sNextInBuffer->pStart = 
            (uint8_t*)stUsbHandle[intfNum].iep_Y_Buffer;
    CdcWriteCtrl[intfNum].sNextInBuffer->pNext = 
            (uint8_t*)stUsbHandle[intfNum].iep_Y_Buffer;
    CdcWriteCtrl[intfNum].sNextInBuffer->pEnd = 
            (uint8_t*)stUsbHandle[intfNum].iep_Y_Buffer + EP_MAX_PACKET_SIZE_CDC;
    CdcWriteCtrl[intfNum].sNextInBuffer->pEpbcnt = 
            &(tInputEndPointDescriptorBlock[stUsbHandle[intfNum].edb_Index].bEPBCTY);
    *(CdcWriteCtrl[intfNum].sNextInBuffer->pEpbcnt) = 0;
    
    CdcWriteCtrl[intfNum].mIntMask = 1 << (stUsbHandle[intfNum].edb_Index + 1);
}

void USBCDC_resetReadCtrl(uint8_t intfNum)
{
    CdcReadCtrl[intfNum].sCurrentOutBuffer = &CdcReadBufferCtrl[2*intfNum];
    CdcReadCtrl[intfNum].sCurrentOutBuffer->bValid = FALSE;
    CdcReadCtrl[intfNum].sCurrentOutBuffer->cLen = 0x00;
    CdcReadCtrl[intfNum].sCurrentOutBuffer->pStart = 
            (uint8_t*)stUsbHandle[intfNum].oep_X_Buffer;
    CdcReadCtrl[intfNum].sCurrentOutBuffer->pNext = 
            (uint8_t*)stUsbHandle[intfNum].oep_X_Buffer;
    CdcReadCtrl[intfNum].sCurrentOutBuffer->pEpbcnt = 
            &(tOutputEndPointDescriptorBlock[stUsbHandle[intfNum].edb_Index].bEPBCTX);
    
    CdcReadCtrl[intfNum].sNextOutBuffer = &CdcReadBufferCtrl[2*intfNum + 1];
    CdcReadCtrl[intfNum].sNextOutBuffer->bValid = FALSE;
    CdcReadCtrl[intfNum].sNextOutBuffer->cLen = 0x00;
    CdcReadCtrl[intfNum].sNextOutBuffer->pStart = 
            (uint8_t*)stUsbHandle[intfNum].oep_Y_Buffer;
    CdcReadCtrl[intfNum].sNextOutBuffer->pNext = 
            (uint8_t*)stUsbHandle[intfNum].oep_Y_Buffer;
    CdcReadCtrl[intfNum].sNextOutBuffer->pEpbcnt = 
            &(tOutputEndPointDescriptorBlock[stUsbHandle[intfNum].edb_Index].bEPBCTY);
}

void USBCDC_resetData (void)
{
    int16_t i;

    //indicates which buffer is used by host to transmit data via OUT endpoint3 - X buffer is first
    //CdcReadCtrl[intfIndex].bCurrentBufferXY = X_BUFFER;

    memset(&CdcControl,   0, sizeof(CdcControl));
    
    for (i = 0; i < CDC_NUM_INTERFACES; i++){
        CdcControl[i].bDataBits = 8;
        USBCDC_resetWriteCtrl(i);
        USBCDC_resetReadCtrl(i);
    }
}

void USBCDC_installSendHandler(uint8_t intfNum, uint8_t handlerfunc(void)){
    CdcWriteCtrl[intfNum].cbBufferSent = handlerfunc;
}

void USBCDC_uninstallSendHandler(uint8_t intfNum){
    CdcWriteCtrl[intfNum].cbBufferSent = NULL;
}

void USBCDC_installRecieveHandler(uint8_t intfNum, uint8_t handlerfunc(void)){
    CdcReadCtrl[intfNum].cbBufferRecieved = handlerfunc;
}

void USBCDC_uninstallRecieveHandler(uint8_t intfNum){
    CdcReadCtrl[intfNum].cbBufferRecieved = NULL;
}


uint8_t USBCDC_getInterfaceStatus_SendAvailable(uint8_t intfNum)
{
    uint8_t ret = 0;
    
    if (!(bFunctionSuspended) && (bEnumerationStatus == ENUMERATION_COMPLETE)){
        if (CdcWriteCtrl[intfNum].sCurrentInBuffer->bAvailable){
            ret = (CdcWriteCtrl[intfNum].sCurrentInBuffer->pEnd - 
            CdcWriteCtrl[intfNum].sCurrentInBuffer->pNext);
            if (CdcWriteCtrl[intfNum].sNextInBuffer->bAvailable){
                ret += EP_MAX_PACKET_SIZE_CDC;
            }
        }
    }
    
    return (ret);
}

uint8_t USBCDC_getInterfaceStatus_Send(uint8_t intfNum, uint8_t* bSendAvailable)
{
    uint8_t ret = 0;
    
    if ((bFunctionSuspended) || (bEnumerationStatus != ENUMERATION_COMPLETE)){
        ret = USBCDC_BUS_NOT_AVAILABLE;
    }
    else{        
        *bSendAvailable = USBCDC_getInterfaceStatus_SendAvailable(intfNum);
        
        if(!(CdcWriteCtrl[intfNum].sCurrentInBuffer->bAvailable & CdcWriteCtrl[intfNum].sNextInBuffer->bAvailable)){
            // Atleast one buffer has NAK cleared, waiting for USB send.
            ret |= USBCDC_SEND_WAITING;
        }
        else{
            if( *bSendAvailable < EP_MAX_PACKET_SIZE_CDC * 2 ){
                // There are some bytes in a partial buffer, 
                // not yet released for send.
                ret |= USBCDC_SEND_BLOCKED;
            }
        }
        
        if(*bSendAvailable){
            ret |= USBCDC_SEND_AVAILABLE;
        }
    }
    
    __no_operation();
    return (ret);
}

uint8_t USBCDC_getInterfaceStatus_RecieveWaiting(uint8_t intfNum)
{
    uint8_t ret = 0;
    
    if (!(bFunctionSuspended) && (bEnumerationStatus == ENUMERATION_COMPLETE)){     
        if (CdcReadCtrl[intfNum].sCurrentOutBuffer->bValid){
            ret = CdcReadCtrl[intfNum].sCurrentOutBuffer->cLen;
            if (CdcReadCtrl[intfNum].sNextOutBuffer->bValid){
                ret += CdcReadCtrl[intfNum].sNextOutBuffer->cLen;
            }
        }
    }
    
    return (ret);
}

uint8_t USBCDC_getInterfaceStatus_Recieve(uint8_t intfNum, uint8_t* bReceivedWaiting)
{
    uint8_t ret = 0;
    
    if ((bFunctionSuspended) || (bEnumerationStatus != ENUMERATION_COMPLETE)){
        //if suspended or not enumerated - report no other tasks pending
        ret = USBCDC_BUS_NOT_AVAILABLE;
    }
    else{
                
        *bReceivedWaiting = USBCDC_getInterfaceStatus_RecieveWaiting(intfNum);
        
        if(!(CdcReadCtrl[intfNum].sCurrentOutBuffer->bValid) || !(CdcReadCtrl[intfNum].sNextOutBuffer->bValid)){
            // Atleast one buffer has NAK cleared.
            ret |= USBCDC_RECIEVE_AVAILABLE;
        }
        
        if(*bReceivedWaiting){
            ret |= USBCDC_RECIEVE_WAITING;
        }
    }
    
    return (ret);
}

uint8_t USBCDC_getInterfaceStatus (uint8_t intfNum, uint8_t* bSendAvailable, 
                                   uint8_t* bReceivedWaiting)
{
    uint8_t ret = 0;
    uint16_t stateIn, stateOut;
    uint8_t edbIndex;
    
    edbIndex = stUsbHandle[intfNum].edb_Index;
    
    stateIn = usbDisableInEndpointInterrupt(edbIndex);
    stateOut = usbDisableOutEndpointInterrupt(edbIndex);
    
    if ((bFunctionSuspended) || (bEnumerationStatus != ENUMERATION_COMPLETE)){
        //if suspended or not enumerated - report no other tasks pending
        ret = USBCDC_BUS_NOT_AVAILABLE;
    }
    else{
        ret |= USBCDC_getInterfaceStatus_Send(intfNum, bSendAvailable);
        ret |= USBCDC_getInterfaceStatus_Recieve(intfNum, bReceivedWaiting);
    }
    
    //restore interrupt status
    usbRestoreInEndpointInterrupt(stateIn);
    usbRestoreOutEndpointInterrupt(stateOut);
    
    __no_operation();
    return (ret);
}



static inline void USBCDC_commitWriteBuffer(uint8_t intfNum);
static inline void USBCDC_resetWriteBuffer(CdcWriteBufferCtrl_t* pBufferCtrl);
static inline void USBCDC_releaseReadBuffer(uint8_t intfNum);
static inline void USBCDC_acquireReadBuffer(CdcReadBufferCtrl_t* pBufferCtrl);


static inline void USBCDC_commitWriteBuffer(uint8_t intfNum)
{
    CdcWriteBufferCtrl_t * pBufferCtrl;
    pBufferCtrl = CdcWriteCtrl[intfNum].sCurrentInBuffer;
    pBufferCtrl->bAvailable = FALSE;
    *(pBufferCtrl->pEpbcnt) = (pBufferCtrl->pNext - pBufferCtrl->pStart);
    CdcWriteCtrl[intfNum].sCurrentInBuffer = CdcWriteCtrl[intfNum].sNextInBuffer;
    CdcWriteCtrl[intfNum].sNextInBuffer = pBufferCtrl;
}

static inline void USBCDC_resetWriteBuffer(CdcWriteBufferCtrl_t* pBufferCtrl)
{
    pBufferCtrl->pNext = pBufferCtrl->pStart;
    pBufferCtrl->bAvailable = TRUE;
}

static inline void USBCDC_releaseReadBuffer(uint8_t intfNum)
{
    CdcReadBufferCtrl_t * pBufferCtrl;
    pBufferCtrl = CdcReadCtrl[intfNum].sCurrentOutBuffer;
    pBufferCtrl->bValid = FALSE;
    *(pBufferCtrl->pEpbcnt) = 0x00;
    CdcReadCtrl[intfNum].sCurrentOutBuffer = CdcReadCtrl[intfNum].sNextOutBuffer;
    CdcReadCtrl[intfNum].sNextOutBuffer = pBufferCtrl;
}

static inline void USBCDC_acquireReadBuffer(CdcReadBufferCtrl_t* pBufferCtrl){
    pBufferCtrl->cLen = *(pBufferCtrl->pEpbcnt) & EPBCNT_BYTECNT_MASK;
    pBufferCtrl->pNext = pBufferCtrl->pStart;
    pBufferCtrl->bValid = TRUE;
}

void USBCDC_sendFlush(uint8_t intfNum)
{
    if (CdcWriteCtrl[intfNum].sCurrentInBuffer->pNext == 
            CdcWriteCtrl[intfNum].sCurrentInBuffer->pStart){
        // Last buffer was already committed. A zero packet needs to be sent. 
        // Wait for a free send buffer to be available. 
        while(!(CdcWriteCtrl[intfNum].sCurrentInBuffer->bAvailable));
    }
    else{
        // Finalize the current in buffer, if it hasn't already been.
        ;
    }
    USBCDC_commitWriteBuffer(intfNum);
    // Trigger Endpoint Interrupt - to start send operation
    USBIEPIFG |= CdcWriteCtrl[intfNum].mIntMask;
}

uint8_t USBCDC_sendChar(uint8_t intfNum, uint8_t byte)
{
    CdcWriteBufferCtrl_t * pBufferCtrl = CdcWriteCtrl[intfNum].sCurrentInBuffer;
    // Caller is expected to make sure there is enough room. 
    // We're not checking again here.
    // if (pBufferCtrl->bAvailable){
    if(TRUE){
        *(pBufferCtrl->pNext++) = byte;
        if (pBufferCtrl->pNext == pBufferCtrl->pEnd){
            USBCDC_commitWriteBuffer(intfNum);
        }
        return 0x01;
    }
    else{
        return 0x00;
    }
}

uint8_t USBCDC_sendBuffer(uint8_t intfNum, uint8_t * buffer, uint8_t len)
{
    uint8_t avail, t1len;
    CdcWriteBufferCtrl_t * pBufferCtrl = CdcWriteCtrl[intfNum].sCurrentInBuffer;
    // Caller is expected to make sure there is enough room. 
    // We're not checking again here.
    // if (pBufferCtrl->bAvailable){
    if (TRUE){
        
        // Some space is in this present buffer
        avail = pBufferCtrl->pEnd - pBufferCtrl->pNext;
        t1len = (avail >= len) ? len : avail;
        
        memcpy((void *)pBufferCtrl->pNext, (void *)buffer, t1len);
        pBufferCtrl->pNext += t1len;
        
        len -= t1len;
        if (pBufferCtrl->pNext == pBufferCtrl->pEnd){
            USBCDC_commitWriteBuffer(intfNum);
        }
        
        if (!(len)){
            return t1len;
        }
        
        pBufferCtrl = CdcWriteCtrl[intfNum].sCurrentInBuffer;
        // Caller is expected to make sure there is enough room. 
        // We're not checking again here.
        //if (!(pBufferCtrl->bAvailable)){
            // The next buffer isn't free anyway.
            // This condition should not actually occur.
            //return t1len;
        //}
        
        //avail = EP_MAX_PACKET_SIZE_CDC;
        //t2len = (avail >= len) ? len : avail;
        
        memcpy((void *)pBufferCtrl->pNext, (void *)(buffer + t1len), len);
        pBufferCtrl->pNext += len;
        
        if (pBufferCtrl->pNext == pBufferCtrl->pEnd){
            USBCDC_commitWriteBuffer(intfNum);
        }
        
        return (t1len + len);
    }
    else{
        return 0x00;
    }
}

uint8_t USBCDC_abortSend (uint8_t intfNum)
{
    // TODO Implement this
    return 0;
}

uint8_t USBCDC_recieveChar(uint8_t intfNum){
    uint8_t ret;
    if (CdcReadCtrl[intfNum].sCurrentOutBuffer->bValid){
        ret = *(CdcReadCtrl[intfNum].sCurrentOutBuffer->pNext++);
        CdcReadCtrl[intfNum].sCurrentOutBuffer->cLen--;
        if (!(CdcReadCtrl[intfNum].sCurrentOutBuffer->cLen)){
            USBCDC_releaseReadBuffer(intfNum);
        }
        return ret;
    }
    else{
        return 0x00;
    }
}

uint8_t USBCDC_recieveBuffer(uint8_t intfNum, uint8_t * buffer, uint8_t len){
    // TODO Replace with memcpy version.
    uint8_t recieved = 0x00;
    if (CdcReadCtrl[intfNum].sCurrentOutBuffer->bValid){
        // Some data is in this present buffer
        while (CdcReadCtrl[intfNum].sCurrentOutBuffer->cLen && recieved < len){
            *(buffer++) = *(CdcReadCtrl[intfNum].sCurrentOutBuffer->pNext++);
            CdcReadCtrl[intfNum].sCurrentOutBuffer->cLen--;
            recieved ++;
        }
        if (!(CdcReadCtrl[intfNum].sCurrentOutBuffer->cLen)){
            USBCDC_releaseReadBuffer(intfNum);
        }
        if (recieved == len || !(CdcReadCtrl[intfNum].sCurrentOutBuffer->bValid)){
            return recieved;
        }
        while (CdcReadCtrl[intfNum].sCurrentOutBuffer->cLen && recieved < len){
            *(buffer++) = *(CdcReadCtrl[intfNum].sCurrentOutBuffer->pNext++);
            CdcReadCtrl[intfNum].sCurrentOutBuffer->cLen --;
            recieved ++;
        }
        if (!(CdcReadCtrl[intfNum].sCurrentOutBuffer->cLen)){
            USBCDC_releaseReadBuffer(intfNum);
        }
        return recieved;
    }
    else{
        return 0x00;
    }
}

uint8_t USBCDC_rejectData (uint8_t intfNum)
{
    if (CdcReadCtrl[intfNum].sCurrentOutBuffer->bValid){
        USBCDC_releaseReadBuffer(intfNum);
        if (CdcReadCtrl[intfNum].sCurrentOutBuffer->bValid){
            USBCDC_releaseReadBuffer(intfNum);
        }
    }
    return (USB_SUCCEED);
}

int16_t CdcToHostFromBuffer (uint8_t intfNum){
    if (CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].sCurrentInBuffer->bAvailable == FALSE && 
            *(CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].sCurrentInBuffer->pEpbcnt) & EPBCNT_NAK){
        USBCDC_resetWriteBuffer(CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].sCurrentInBuffer);
    }
    if (CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].sNextInBuffer->bAvailable == FALSE && 
            *(CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].sNextInBuffer->pEpbcnt) & EPBCNT_NAK){
        USBCDC_resetWriteBuffer(CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].sNextInBuffer);
    }
    if (CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].cbBufferSent){
        return (CdcWriteCtrl[INTFNUM_OFFSET(intfNum)].cbBufferSent());
    }
    return FALSE;   //TRUE for wake up after interrupt
}

int16_t CdcToBufferFromHost (uint8_t intfNum)
{
    if (CdcReadCtrl[INTFNUM_OFFSET(intfNum)].sCurrentOutBuffer->bValid == FALSE &&
            *(CdcReadCtrl[INTFNUM_OFFSET(intfNum)].sCurrentOutBuffer->pEpbcnt) & EPBCNT_NAK){
        USBCDC_acquireReadBuffer(CdcReadCtrl[INTFNUM_OFFSET(intfNum)].sCurrentOutBuffer);
    }
    if (CdcReadCtrl[INTFNUM_OFFSET(intfNum)].sNextOutBuffer->bValid == FALSE &&
            *(CdcReadCtrl[INTFNUM_OFFSET(intfNum)].sNextOutBuffer->pEpbcnt) & EPBCNT_NAK){
        USBCDC_acquireReadBuffer(CdcReadCtrl[INTFNUM_OFFSET(intfNum)].sNextOutBuffer);
    }
    if (CdcReadCtrl[INTFNUM_OFFSET(intfNum)].cbBufferRecieved){
        return (CdcReadCtrl[INTFNUM_OFFSET(intfNum)].cbBufferRecieved());
    }
    return FALSE;   //TRUE for wake up after interrupt
}


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

    wBytesRemainingOnIEP0 = 0x07;                                           //amount of data to be send over EP0 to host
    usbSendDataPacketOnEP0((uint8_t*)&abUsbRequestReturnData[0]);              //send data to host

    return (FALSE);
}

//----------------------------------------------------------------------------

uint8_t usbSetLineCoding (void)
{
    usbReceiveDataPacketOnEP0((uint8_t*)&abUsbRequestIncomingData);            //receive data over EP0 from Host

    return (FALSE);
}

//----------------------------------------------------------------------------

uint8_t usbSetControlLineState (void)
{
	USBCDC_handleSetControlLineState((uint8_t)tSetupPacket.wIndex,
                                     (uint8_t)tSetupPacket.wValue);
    usbSendZeroLengthPacketOnIEP0();                                        //Send ZLP for status stage

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

#endif  //ifdef _CDC_


/*----------------------------------------------------------------------------+
 | End of source file                                                          |
 +----------------------------------------------------------------------------*/
/*------------------------ Nothing Below This Line --------------------------*/
//Released_Version_5_10_00_17
