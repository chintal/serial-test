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
 * ======== UsbHid.h ========
 */
#include <stdint.h>

#ifndef _UsbHid_EVENT_HANDLERS_H_
#define _UsbHid_EVENT_HANDLERS_H_

#ifdef __cplusplus
extern "C"
{
#endif

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
//! Data has been Received for HID Interface
//!
//! \param intfNum is which HID interface is being used.
//! 
//! This event applies to HID-Datapipe only, as opposed to HID-Traditional.
//! It indicates that data has been received for HID interface intfNum with no
//! receive operation underway. Effectively, the API doesn???t know what to do
//! with this data and is asking for instructions. The application can respond
//! by either initiating a receive operation or rejecting the data. Until one of
//! these is performed, USB data reception cannot continue; any packets received
//! from the USB host will be NAK???ed.
//! 
//! Therefore, this event should be handled quickly. A receive operation cannot
//! be started directly out of this event, since USBHID_receiveData() cannot be
//! called from the event handlers. However, the handler can set a flag for
//! main() to begin the receive operation. After this function exits, a call to
//! USBHID_getInterfaceStatus() for this HID interface will return kUSBDataWaiting.
//! 
//! If the application is written so that a receive operation is always begun
//! prior to data arriving from the host, this event will never occur. The
//! software designer generally has a choice of whether to use this event as
//! part of code flow (initiating receive operations after data is received), or
//! to always keep a receive operation open in case data arrives. (See Sec. 11
//! of \e "Programmer???s Guide: MSP430 USB API Stack for CDC/PHDC/HID/MSC" more
//! discussion.)
//! 
//! \return FALSE to go asleep after interrupt (in the case the CPU slept before
//! interrupt).
//! \return TRUE to keep CPU awake
//
//*****************************************************************************
uint8_t USBHID_handleDataReceived (uint8_t intfNum);

//*****************************************************************************
//
//! Send Operation on Data Interface has been Completed
//!
//! \param intfNum is which HID interface is being used.
//! 
//! This event applies to HID-Datapipe only, as opposed to HID-Traditional. It
//! indicates that a send operation on data interface intfNum has just been
//! completed.
//! 
//! In applications sending a series of large blocks of data, the designer may
//! wish to use this event to trigger another send operation. This cannot be
//! done directly out of this event, since USBHID_sendData() cannot be called
//! from the event handlers. However, the handler can set a flag for main() to
//! begin the operation.
//! 
//! \return FALSE to go asleep after interrupt (in the case the CPU slept before
//! interrupt).
//! \return TRUE to keep CPU awake
//
//*****************************************************************************
uint8_t USBHID_handleSendCompleted (uint8_t intfNum);

//*****************************************************************************
//
//! Receive Operation has been Completed
//!
//! \param intfNum is which HID interface is being used.
//! 
//! This event applies to HID-Datapipe only, as opposed to HID-Traditional. It
//! indicates that a receive operation on HID interface intfNum has just been
//! completed, and the data is therefore available in the user buffer assigned
//! when the call was made to USBHID_receiveData(). If this event occurs, it
//! means that the entire buffer is full, according to the size value that was
//! requested.
//! 
//! The designer may wish to use this event to trigger another receive
//! operation. This cannot be done directly out of this event, since
//! USBHID_receiveData() cannot be called from the event handlers. However, the
//! handler can set a flag for main() to begin the operation. 
//! 
//! \return FALSE to go asleep after interrupt (in the case the CPU slept before
//! interrupt).
//! \return TRUE to keep CPU awake
//
//*****************************************************************************
uint8_t USBHID_handleReceiveCompleted (uint8_t intfNum);


//*****************************************************************************
//
//! Set_Protocol Request Received from the Host
//!
//! \param protocol indicates HID_BOOT_PROTOCOL or HID_REPORT_PROTOCOL 
//! \param intfNum is which HID interface is being used.
//! 
//! This event applies to HID Traditional only. It indicates that the host has
//! requested a change in the HID protocol ??? from Boot to Standard or Standard
//! to Boot. An application that maintains separate reports for boot and
//! standard protocols can switch to the appropriate report upon receiving this
//! request. The protocol field is either HID_BOOT_PROTOCOL or
//! HID_REPORT_PROTOCOL.
//! 
//! \return FALSE to go asleep after interrupt (in the case the CPU slept before
//! interrupt).
//! \return TRUE to keep CPU awake
//
//*****************************************************************************
uint8_t USBHID_handleBootProtocol (uint8_t protocol, uint8_t intfnum);

//*****************************************************************************
//
//! Set_Report request Received from the Host
//!
//! \param reportType is either USB_REQ_HID_INPUT, USB_REQ_HID_OUTPUT or
//!                             USB_REQ_HID_FEATURE
//! \param reportId is values defined by report descriptor
//! \param dataLength is length of report
//! \param intfNum is which HID interface is being used.
//! 
//! This event indicates that a Set_Report request was received from the
//! host. The application needs to supply a buffer to retrieve the report data
//! that will be sent as part of this request. This handler is passed the
//! reportType, reportId, the length of data phase as well as the interface
//! number.
//!
//! \return Pointer to input/output/feature Report Buffer
//
//*****************************************************************************
uint8_t *USBHID_handleEP0SetReport (uint8_t reportType, uint8_t reportId,
    uint16_t requestedLength,
    uint8_t intfnum);
//*****************************************************************************
//
//! Data as Part of Set_Report Request was Received from the Host
//!
//! \param intfNum is which HID interface is being used.
//! 
//! This event indicates that data as part of Set_Report request was received
//! from the host. If the application supplied a buffer as part of
//! USBHID_handleEP0SetReport, then this buffer will contain the Set Report data.
//! 
//! \return TRUE to wake up after data was received.
//
//*****************************************************************************
uint8_t USBHID_handleEP0SetReportDataAvailable (uint8_t intfnum);

//*****************************************************************************
//
//! Get_Report Request was Received from the Host
//!
//! \param reportType is either USB_REQ_HID_INPUT, USB_REQ_HID_OUTPUT or
//!                             USB_REQ_HID_FEATURE
//! \param reportId is values defined by report descriptor
//! \param requestedLength is length of report
//! \param intfNum is which HID interface is being used.
//!
//! This event indicates that a Get_Report request was received from the host.
//! The application can supply a buffer of data that will be sent to the host.
//! This handler is passed the reportType, reportId, the requested length as
//! well as the interface number.
//! 
//! \return Pointer to input input/output/feature buffer.
//
//*****************************************************************************
uint8_t *USBHID_handleEP0GetReport (uint8_t reportType, uint8_t reportId,
    uint16_t requestedLength,
    uint8_t intfnum);

//******************************************************************************
//
// Close the Doxygen group.
//! @}
//
//******************************************************************************

#ifdef __cplusplus
}
#endif
#endif  //_UsbHid_H_
//Released_Version_5_10_00_17
