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
 * ======== UsbCdc.h ========
 */

#ifndef _UsbCdc_H_
#define _UsbCdc_H_

#ifdef __cplusplus
extern "C"
{
#endif
    
#include "../../descriptors.h"
#include <msp430.h>


#define USBCDC_SEND_STARTED             0x01
#define USBCDC_SEND_COMPLETE            0x02
#define USBCDC_INTERFACE_BUSY_ERROR     0x03
#define USBCDC_RECEIVE_STARTED          0x04
#define USBCDC_RECEIVE_COMPLETED        0x05
#define USBCDC_RECEIVE_IN_PROGRESS      0x06
#define USBCDC_GENERAL_ERROR            0x07
#define USBCDC_BUS_NOT_AVAILABLE        0x08

#define USBCDC_ALL_CDC_EVENTS           0xFF
    
#define USBCDC_SEND_WAITING             0x01
#define USBCDC_SEND_AVAILABLE           0x02
#define USBCDC_SEND_BLOCKED             0x20
#define USBCDC_RECIEVE_WAITING          0x04
#define USBCDC_RECIEVE_AVAILABLE        0x10
#define USBCDC_BUS_NOT_AVAILABLE        0x08

    
typedef struct CDC_WRITE_BUFFER_CTRL_t{
    uint8_t * pStart;
    uint8_t * pNext;
    uint8_t * pEnd;
    volatile uint8_t bAvailable;
    volatile uint8_t * pEpbcnt;
}CdcWriteBufferCtrl_t;

typedef struct CDC_WRITE_CTRL_t {
    uint16_t mIntMask;
    CdcWriteBufferCtrl_t * sCurrentInBuffer;
    CdcWriteBufferCtrl_t * sNextInBuffer;
    uint8_t (*cbBufferSent)(void);
}CdcWriteCtrl_t;

typedef struct CDC_READ_BUFFER_CTRL_t{
    uint8_t * pStart;
    uint8_t * pNext;
    uint8_t cLen;
    volatile uint8_t bValid;
    volatile uint8_t * pEpbcnt;
}CdcReadBufferCtrl_t;

typedef struct CDC_READ_CTRL_t {
    CdcReadBufferCtrl_t * sCurrentOutBuffer;
    CdcReadBufferCtrl_t * sNextOutBuffer;
    uint8_t (*cbBufferRecieved)(void);
}CdcReadCtrl_t;

typedef struct CDC_CONTROL_t {
    uint32_t lBaudrate;
    uint8_t bDataBits;
    uint8_t bStopBits;
    uint8_t bParity;
}CdcControl_t;

extern CdcControl_t         CdcControl[CDC_NUM_INTERFACES];
extern CdcWriteCtrl_t       CdcWriteCtrl[CDC_NUM_INTERFACES];
extern CdcWriteBufferCtrl_t CdcWriteBufferCtrl[CDC_NUM_INTERFACES * 2];
extern CdcReadCtrl_t        CdcReadCtrl[CDC_NUM_INTERFACES];
extern CdcReadBufferCtrl_t  CdcReadBufferCtrl[CDC_NUM_INTERFACES * 2];

//Local Macros
#define INTFNUM_OFFSET(X)   (X - CDC0_INTFNUM)  //Get the CDC offset

/*----------------------------------------------------------------------------
 * These functions can be used in application
 +----------------------------------------------------------------------------*/
//*****************************************************************************
//
//! \addtogroup cdc_api
//! @{
//
//******************************************************************************

void USBCDC_resetData (void);

void USBCDC_resetWriteCtrl(uint8_t intfNum);

void USBCDC_resetReadCtrl(uint8_t intfNum);

void USBCDC_installSendHandler(uint8_t intfNum, uint8_t handlerfunc(void));

void USBCDC_uninstallSendHandler(uint8_t intfNum);

void USBCDC_installRecieveHandler(uint8_t intfNum, uint8_t handlerfunc(void));

void USBCDC_uninstallRecieveHandler(uint8_t intfNum);

uint8_t USBCDC_getInterfaceStatus_SendAvailable(uint8_t intfNum);

uint8_t USBCDC_getInterfaceStatus_Send(uint8_t intfNum, uint8_t* bSendAvailable);

uint8_t USBCDC_getInterfaceStatus_RecieveWaiting(uint8_t intfNum);

uint8_t USBCDC_getInterfaceStatus_Recieve(uint8_t intfNum, uint8_t* bReceivedWaiting);

//*****************************************************************************
//
//! Indicates the Status of the CDC Interface.
//!
//! \param intfNum is the interface number for which status is being retrieved.
//! \param bytesSent If a send operation is underway, the number of bytes that
//!     send have been transferred to the host is returned in this location. If
//!     no operation is underway, this returns zero.
//! \param bytesReceived If a receive operation is underway, the number of bytes
//!     that have been transferred to the assigned memory location is returned
//!     in this location. If no receive operation is underway, this returns
//!     zero.
//!
//! Indicates the status of the CDC interface \b intfNum. If a send operation is
//! active for this interface, the function also returns the number of bytes
//! that have been transmitted to the host. If a receive operation is active for
//! this interface, the function also returns the number of bytes that have been
//! received from the host and are waiting at the assigned address.
//!
//! Because multiple flags can be returned, the possible values can be masked
//! together.
//!
//! \return Any combination of the following:
//!         - \b USBCDC_SEND_WAITING: Indicates that bytes are waiting for 
//!             transmission to the host.
//!           \b USBCDC_SEND_AVAILABLE: Indicates that the interface can accept
//!             atleast one byte for transmission to the host.
//!         - \b USBCDC_RECIEVE_WAITING: Indicates that data has been received
//!             from the host for this interface, waiting in the USB receive
//!             buffers, lacking an open receive operation to accept it.
//!           \b USBCDC_RECIEVE_AVAILABLE: Indicates that the interface can 
//!             accept atleast one byte from the host.
//!         - \b USBCDC_BUS_NOT_AVAILABLE: Indicates that the bus is either
//!             suspended or disconnected. Any operations that had previously
//!             been underway are now aborted.
//
//*****************************************************************************
uint8_t USBCDC_getInterfaceStatus (uint8_t intfNum, uint8_t* bSendAvailable, 
                                   uint8_t* bReceivedWaiting);

static inline void USBCDC_sendTrigger(uint8_t intfNum);

static inline void USBCDC_sendTrigger(uint8_t intfNum)
{
    //trigger Endpoint Interrupt - to start send operation
    USBIEPIFG |= CdcWriteCtrl[intfNum].mIntMask;
}


void USBCDC_sendFlush(uint8_t intfNum);

uint8_t USBCDC_sendChar(uint8_t intfNum, uint8_t byte);

uint8_t USBCDC_sendBuffer(uint8_t intfNum, uint8_t * buffer, uint8_t len);

uint8_t USBCDC_abortSend(uint8_t intfNum);

uint8_t USBCDC_recieveChar(uint8_t intfNum);

uint8_t USBCDC_recieveBuffer(uint8_t intfNum, uint8_t * buffer, uint8_t len);

//*****************************************************************************
//
//! Rejects the Data Received from the Host.
//!
//! This function rejects data that has been received from the host, for
//! interface inftNum, that does not have an active receive operation underway.
//! It resides in the USB endpoint buffer and blocks further data until a
//! receive operation is opened, or until rejected. When this function is
//! called, the buffer for this interface is purged, and the data lost. This
//! frees the USB path to resume communication.
//!
//! See Sec. 7.2 of \e "Programmer's Guide: MSP430 USB API Stack for CDC/PHDC/HID/MSC" 
//! for a detailed discussion of receive operations.
//!
//! \return \b USB_SUCCEED
//
//*****************************************************************************
uint8_t USBCDC_rejectData(uint8_t intfNum);

//*****************************************************************************
//
//! USBCDC Send Endpoint Interrupt Handler
//!
//! This function is used only by USB interrupt. The bulk of this is eliminated 
//! and USB CDC flow control is left to the combination of the application code 
//! and the HAL. This function clears the write buffer control structures for 
//! buffers that have already been sent to the host, marking them available to be
//! filled in with the next chunk of data.
//!
//! If the application wants a function to be called when this event occurs, the 
//! cbBufferEmpty() handler should be installed. 
//!
//! Note : Zero packet transmission is also the application / HAL's 
//! responsibility. See the send_flush HAL API function for details
//!
//! \return Return value of the Buffer Empty Handler, or FALSE for no wakeup 
//!         after interrupt.
//
//*****************************************************************************
int16_t CdcToHostFromBuffer (uint8_t intfNum);

//*****************************************************************************
//
//! USBCDC Recieve Endpoint Interrupt Handler
//!
//! This function is used only by USB interrupt. The bulk of this is eliminated 
//! and USB CDC flow control is left to the combination of the application code 
//! and the HAL. 
//!
//! If the application wants a function to be called when this event occurs, the 
//! cbBufferFull() handler should be installed. 
//!
//! \return Return value of the Buffer Full Handler, or TRUE for wakeup 
//!         after interrupt.
//
//*****************************************************************************
int16_t CdcToBufferFromHost (uint8_t intfNum);

//******************************************************************************
//
// Close the Doxygen group.
//! @}
//
//******************************************************************************


#ifdef __cplusplus
}
#endif
#endif  //_UsbCdc_H_
//Released_Version_5_10_00_17
