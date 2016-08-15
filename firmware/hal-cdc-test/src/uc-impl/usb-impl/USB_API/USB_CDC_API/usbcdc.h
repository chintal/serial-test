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


#define USBCDC_SEND_STARTED             0x01
#define USBCDC_SEND_COMPLETE            0x02
#define USBCDC_INTERFACE_BUSY_ERROR     0x03
#define USBCDC_RECEIVE_STARTED          0x04
#define USBCDC_RECEIVE_COMPLETED        0x05
#define USBCDC_RECEIVE_IN_PROGRESS      0x06
#define USBCDC_GENERAL_ERROR            0x07
#define USBCDC_BUS_NOT_AVAILABLE        0x08
//returned by USBCDC_rejectData() if no data pending
#define USBCDC_NO_DATA_WAITING          0X01
#define USBCDC_WAITING_FOR_SEND         0x01
#define USBCDC_WAITING_FOR_RECEIVE      0x02
#define USBCDC_DATA_WAITING             0x04
#define USBCDC_BUS_NOT_AVAILABLE        0x08
#define USBCDC_ALL_CDC_EVENTS           0xFF

typedef struct CDC_WRITE_CTRL_t {
    uint16_t nCdcBytesToSend;           //holds counter of bytes to be sent
    uint16_t nCdcBytesToSendLeft;       //holds counter how many bytes is still to be sent
    const uint8_t* pUsbBufferToSend;    //holds the buffer with data to be sent
    uint8_t bCurrentBufferXY;           //is 0 if current buffer to write data is X, or 1 if current buffer is Y
    uint8_t bZeroPacketSent;            //= FALSE;
    uint8_t last_ByteSend;
}CdcWriteCtrl_t;

typedef struct CDC_READ_CTRL_t {
    uint8_t *pUserBuffer;                //holds the current position of user's receiving buffer. If NULL- no receiving
                                         //operation started
    uint8_t *pCurrentEpPos;              //current positon to read of received data from curent EP
    uint16_t nBytesToReceive;            //holds how many bytes was requested by receiveData() to receive
    uint16_t nBytesToReceiveLeft;        //holds how many bytes is still requested by receiveData() to receive
    uint8_t * pCT1;                      //holds current EPBCTxx register
    uint8_t * pCT2;                      //holds next EPBCTxx register
    uint8_t * pEP2;                      //holds addr of the next EP buffer
    uint8_t nBytesInEp;                  //how many received bytes still available in current EP
    uint8_t bCurrentBufferXY;            //indicates which buffer is used by host to transmit data via OUT endpoint3
}CdcReadCtrl_t;

typedef struct CDC_CONTROL_t {
    uint32_t lBaudrate;
    uint8_t bDataBits;
    uint8_t bStopBits;
    uint8_t bParity;
}CdcControl_t;

extern CdcControl_t   CdcControl  [CDC_NUM_INTERFACES];
extern CdcWriteCtrl_t CdcWriteCtrl[CDC_NUM_INTERFACES];
extern CdcReadCtrl_t  CdcReadCtrl [CDC_NUM_INTERFACES];

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
//*****************************************************************************
//
//! Begins a Send Operation to the USB Host.
//!
//! \param *data is an array of data to be sent.
//! \param size is the number of bytes to be sent, starting from address
//! 	\b data.
//! \param intfNum selects which data should be transmitted over.
//!
//! Initiates sending of a user buffer over CDC interface \b intfNum, of size
//! \b size and starting at address \b data. If \b size is larger than the
//! packet size, the function handles all packetization and buffer management.
//! \b size has no inherent upper limit (beyond being a 16-bit value).
//!
//! In most cases where a send operation is successfully started, the function
//! will return \b USBCDC_SEND_STARTED. A send operation is said to be underway. At
//! some point, either before or after the function returns, the send operation
//! will complete, barring any events that would preclude it. (Even if the
//! operation completes before the function returns, the return code will still
//! be \b USBCDC_SEND_STARTED.)
//! If the bus is not connected when the function is called, the function
//! returns \b USBCDC_BUS_NOT_AVAILABLE, and no operation is begun. If \b size is 0,
//! the function returns \b USBCDC_GENERAL_ERROR. If a previous send operation is
//! already underway for this data interface, the function returns with
//! \b USBCDC_INTERFACE_BUSY_ERROR.
//!
//! USB includes low-level mechanisms that ensure valid transmission of data.
//!
//! See Sec. 7.2 of \e "Programmer's Guide: MSP430 USB API Stack for CDC/PHDC/HID/MSC" 
//! for a detailed discussion of send operations.
//!
//! \return Any of the following:
//! 			- \b USBCDC_SEND_STARTED: a send operation was successfully
//! 				started
//! 			- \b USBCDC_INTERFACE_BUSY_ERROR: a previous send operation is
//! 				underway
//! 			- \b USBCDC_BUS_NOT_AVAILABLE: the bus is either suspended or
//! 				disconnected
//! 			- \b USBCDC_GENERAL_ERROR: \b size was zero, or other error
//
//*****************************************************************************
uint8_t USBCDC_sendData (const uint8_t* data, uint16_t size, uint8_t intfNum);


//*****************************************************************************
//
//! Begins a Receive Operation from the USB Host.
//!
//! \param *data is an array to contain the data received.
//! \param size is the number of bytes to be received.
//! \param intfNum is which data interface to receive from.
//!
//! Receives \b size bytes over CDC interface \b intfNum into memory starting at
//! address \b data. \b size has no inherent upper limit (beyond being a 16-bit
//! value).
//!
//! The function may return with \b USBCDC_RECEIVE_STARTED, indicating that a
//! receive operation is underway. The operation completes when \b size bytes
//! are received. The application should ensure that the data memory buffer be
//! available during the whole of the receive operation.
//!
//! The function may also return with \b USBCDC_RECEIVE_COMPLETED. This means that
//! the receive operation was complete by the time the function returned.
//!
//! If the bus is not connected when the function is called, the function
//! returns \b USBCDC_BUS_NOT_AVAILABLE, and no operation is begun. If \b size is 0,
//! the function returns \b USBCDC_GENERAL_ERROR. If a previous receive operation
//! is already underway for this data interface, the function returns
//! \b USBCDC_INTERFACE_BUSY_ERROR.
//!
//! USB includes low-level mechanisms that ensure valid transmission of data.
//!
//! See Sec. 7.2 of \e "Programmer's Guide: MSP430 USB API Stack for CDC/PHDC/HID/MSC" 
//! for a detailed discussion of receive operations.
//!
//! \return Any of the following:
//! 		- \b USBCDC_RECEIVE_STARTED: A receive operation has been
//! 			succesfully started.
//! 		- \b USBCDC_RECEIVE_COMPLETED: The receive operation is already
//! 			completed.
//! 		- \b USBCDC_INTERFACE_BUSY_ERROR: a previous receive operation is
//! 			underway.
//! 		- \b USBCDC_BUS_NOT_AVAILABLE: the bus is either suspended or
//! 			disconnected.
//! 		- \b USBCDC_GENERAL_ERROR: \b size was zero, or other error.
//
//*****************************************************************************
uint8_t USBCDC_receiveData (uint8_t* data, uint16_t size, uint8_t intfNum);


//*****************************************************************************
//
//! Aborts an Active Receive Operation.
//!
//! \param *size is the number of bytes that were received and are waiting
//! 		at the assigned address.
//! \param intfNum is the data interface for which the send should be
//! 		aborted.
//!
//! Aborts an active receive operation on CDC interface \b intfNum. Returns the
//! number of bytes that were received and transferred to the data location
//! established for this receive operation. The data moved to the buffer up to
//! that time remains valid.
//!
//! An application may choose to call this function if it decides it no longer
//! wants to receive data from the USB host. It should be noted that if a
//! continuous stream of data is being received from the host, aborting the
//! operation is akin to pressing a "pause" button; the host will be NAK'ed
//! until another receive operation is opened.
//!
//! See Sec. 7.2 of \e "Programmer's Guide: MSP430 USB API Stack for CDC/PHDC/HID/MSC" 
//! for a detailed discussion of receive operations.
//!
//! \return \b USB_SUCCEED
//
//*****************************************************************************
uint8_t USBCDC_abortReceive (uint16_t* size, uint8_t intfNum);


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
uint8_t USBCDC_rejectData (uint8_t intfNum);


//*****************************************************************************
//
//! Aborts an Active Send Operation.
//!
//! \param size is the number of bytes that were sent prior to the abort action.
//! \param intfNum is the data interface for which the send should be aborted.
//!
//! Aborts an active send operation on data interface \b intfNum. Returns the
//! number of bytes that were sent prior to the abort, in \b size.
//!
//! An application may choose to call this function if sending failed, due to
//! factors such as:
//! 		- a surprise removal of the bus
//! 		- a USB suspend event
//! 		- any send operation that extends longer than desired (perhaps due
//! 			to no open COM port on the host.)
//!
//! \return \b USB_SUCCEED
//
//*****************************************************************************
uint8_t USBCDC_abortSend (uint16_t* size, uint8_t intfNum);


//*****************************************************************************
//
//! Indicates the Status of the CDC Interface.
//!
//! \param intfNum is the interface number for which status is being retrieved.
//! \param bytesSent If a send operation is underway, the number of bytes that
//! 	send have been transferred to the host is returned in this location. If
//! 	no operation is underway, this returns zero.
//! \param bytesReceived If a receive operation is underway, the number of bytes
//! 	that have been transferred to the assigned memory location is returned
//! 	in this location. If no receive operation is underway, this returns
//! 	zero.
//!
//! Indicates the status of the CDC interface \b intfNum. If a send operation is
//! active for this interface, the function also returns the number of bytes
//! that have been transmitted to the host. If a receive operation is active for
//! this interface, the function also returns the number of bytes that have been
//! received from the host and are waiting at the assigned address.
//!
//! Because multiple flags can be returned, the possible values can be masked
//! together - for example, \b USBCDC_WAITING_FOR_SEND + \b USBCDC_DATA_WAITING.
//!
//! \return Any combination of the following:
//! 		- \b USBCDC_WAITING_FOR_SEND: Indicates that a send operation is
//! 			open ont his interface
//! 		- \b USBCDC_WAITING_FOR_RECEIVE: Indicates that a receive operation
//! 			is open on this interface
//! 		- \b USBCDC_DATA_WAITING: Indicates that data has been received
//! 			from the host for this interface, waiting in the USB receive
//! 			buffers, lacking an open receive operation to accept it.
//! 		- \b USBCDC_BUS_NOT_AVAILABLE: Indicates that the bus is either
//! 			suspended or disconnected. Any operations that had previously
//! 			been underway are now aborted.
//
//*****************************************************************************
uint8_t USBCDC_getInterfaceStatus (uint8_t intfNum, uint16_t* bytesSent, 
                                   uint16_t* bytesReceived);


//*****************************************************************************
//
//! Gives the Number of Bytes in the USB Endpoint Buffer.
//!
//! \param intfNum is the data interface whose buffer is to be checked.
//!
//! Returns the number of bytes waiting in the USB endpoint buffer for
//! \b intfNum. A non-zero value generally means that no receive operation is
//! open by which these bytes can be copied to a user buffer. If the value is
//! non-zero, the application should either open a receive operation so that the
//! data can be moved out of the endpoint buffer, or the data should be rejected
//! (USBCDC_rejectData()).
//!
//! \return The number of bytes waiting in this buffer.
//
//*****************************************************************************
uint8_t USBCDC_getBytesInUSBBuffer (uint8_t intfNum);


//******************************************************************************
//
// Close the Doxygen group.
//! @}
//
//******************************************************************************

static inline int16_t CdcIsReceiveInProgress (uint8_t intfNum);

//helper for USB interrupt handler
static inline int16_t CdcIsReceiveInProgress (uint8_t intfNum)
{
    return (CdcReadCtrl[INTFNUM_OFFSET(intfNum)].pUserBuffer != NULL);
}


#ifdef __cplusplus
}
#endif
#endif  //_UsbCdc_H_
//Released_Version_5_10_00_17
