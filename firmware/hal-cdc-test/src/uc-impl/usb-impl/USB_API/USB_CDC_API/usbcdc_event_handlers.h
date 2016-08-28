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
 * 
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/* 
 * ======== UsbCdc.h ========
 */

#ifndef _UsbCdc_event_handlers_H_
#define _UsbCdc_event_handlers_H_

#ifdef __cplusplus
extern "C"
{
#endif
    
#include "usbcdc.h"


/*----------------------------------------------------------------------------
 * Event-Handling routines
 +----------------------------------------------------------------------------*/
//*****************************************************************************
//
//! \addtogroup event_handling_api
//! @{
//
//******************************************************************************

//*****************************************************************************
//
//! Indicates Data has been Received for CDC Interface
//!
//! \param intfNum is which CDC interface is being used.
//! 
//! This event indicates that data has been received for CDC interface intfNum
//! with no receive operation underway. Effectively, the API doesn???t know what
//! to do with this data and is asking for instructions. The application can
//! respond by either initiating a receive operation or rejecting the data.
//! Until one of these is performed, USB data reception cannot continue; any
//! packets received from the USB host will be NAK???ed.
//! 
//! Therefore, this event should be handled quickly. A receive operation cannot
//! be started directly out of this event, since USBCDC_receiveData() cannot be
//! called from the event handlers. However, the handler can set a flag for
//! main() to begin the receive operation. After this function exits, a call to
//! USBCDC_getInterfaceStatus() for this CDC interface will return kUSBDataWaiting.
//! 
//! If the application is written so that a receive operation is always begun
//! prior to data arriving from the host, this event will never occur. The
//! software designer generally has a choice of whether to use this event as
//! part of code flow (initiating receive operations after data is received), or
//! to always keep a receive operation open in case data arrives. (See Sec. 11
//! of \e "Programmer???s Guide: MSP430 USB API Stack for CDC/PHDC/HID/MSC" for
//! more discussion.)
//! 
//! \return TRUE to wake up after data was received.
//! \return FALSE to not wake up
//
//*****************************************************************************
uint8_t USBCDC_handleDataReceived (uint8_t intfNum);


//*****************************************************************************
//
//! Receive Operation on CDC Interface has Completed
//!
//! \param intfNum is which HID interface is being used.
//! 
//! This event indicates that a receive operation on CDC interface intfNum has
//! just been completed, and the data is therefore available in the user buffer
//! assigned when the call was made to USBCDC_receiveData(). If this event
//! occurs, it means that the entire buffer is full, according to the size value
//! that was requested.
//! 
//! The designer may wish to use this event to trigger another receive
//! operation. This cannot be done directly out of this event, since
//! USBCDC_receiveData() cannot be called from the event handlers. However, the
//! handler can set a flag for main() to begin the operation.
//! 
//! \return FALSE to go asleep after interrupt (in the case the CPU slept before
//! interrupt).
//
//*****************************************************************************
uint8_t USBCDC_handleReceiveCompleted (uint8_t intfNum);


/*
 * Toggle state variable for CTS in USB Stack
 */
void USBCDC_setCTS(uint8_t state);


//*****************************************************************************
//
//! New Line Coding Parameters have been Received from the Host
//!
//! \param intfNum is which CDC interface is being used.
//! \param lBaudrate had COMport baud rate values such as 9600, 19200 etc 
//!
//! This event indicates that a SetLineCoding request has been received from the
//! host and new values for baud rate are available.
//! 
//! The application can use the new baud rate value to re-configure the Uart
//! in the case of a usb to uart bridge application. 
//! 
//! \return FALSE to go asleep after interrupt (in the case the CPU slept before
//! interrupt).
//
//*****************************************************************************
uint8_t USBCDC_handleSetLineCoding (uint8_t intfNum, uint32_t lBaudrate);


//*****************************************************************************
//
//! New Line State has been Received from the Host
//!
//! \param intfNum is which CDC interface is being used.
//! \param lineState BIT0 is DTR_PRESENT(1) or DTR_NOT_PRESENT(0)
//!                  BIT1 is RTS_PRESETNT(1) or RTS_NOT_PRESENT(0)
//!
//! This event indicates that a SetControlLineState request has been received
//! from the host and new values for RTS are available.
//! 
//! The application can use the new RTS value to flow off the uart. 
//! 
//! \return FALSE to go asleep after interrupt (in the case the CPU slept before
//! interrupt).
//
//*****************************************************************************
uint8_t USBCDC_handleSetControlLineState (uint8_t intfNum, uint8_t lineState);


//******************************************************************************
//
// Close the Doxygen group.
//! @}
//
//******************************************************************************


/*----------------------------------------------------------------------------
 * These functions is to be used ONLY by USB stack, and not by application
 +----------------------------------------------------------------------------*/

/**
 * Send a packet with the settings of the second uart back to the usb host
 */
uint8_t usbGetLineCoding(void);

/**
 * Prepare EP0 to receive a packet with the settings for the second uart
 */
uint8_t usbSetLineCoding(void);

/**
 * Function set or reset RTS
 */
uint8_t usbSetControlLineState(void);

/**
 * Readout the settings (send from usb host) for the second uart
 */
uint8_t Handler_SetLineCoding(void);

#ifdef __cplusplus
}
#endif
#endif  //_UsbCdc_H_
//Released_Version_5_10_00_17
