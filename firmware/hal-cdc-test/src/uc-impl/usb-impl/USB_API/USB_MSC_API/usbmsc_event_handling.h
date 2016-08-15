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
 * ======== UsbMsc.h ========
 */
#include <stdint.h>

#ifndef _USB_MSC_EVENT_HANDLERS_H_
#define _USB_MSC_EVENT_HANDLERS_H_

#include "usbmsc.h"

#ifdef __cplusplus
extern "C"
{
#endif


//*****************************************************************************
//
//! \addtogroup event_handling_api
//! @{
//
//******************************************************************************

//*****************************************************************************
//
//! API Requests a Buffer
//!
//! This event occurs when the API requests a buffer. Immediately prior to this,
//! the API sets the operation field of the USBMSC_RWBuf_Info structure
//! corresponding with the request, and also clears the low-power-mode bits of
//! the MCU's status register to ensure the CPU remains awake to process the
//! buffer after the event occurs. 
//! 
//! NOTE: This means the return value of this event has no effect; the CPU will 															<-- BECAUSE OF THIS...
//! remain awake even if this function returns FALSE.
//!
//! \return FALSE to go to sleep after interrupt
//
//*****************************************************************************
uint8_t USBMSC_handleBufferEvent(void);

//******************************************************************************
//
// Close the Doxygen group.
//! @}
//
//******************************************************************************

#ifdef __cplusplus
}
#endif
#endif  //_USB_MSC_H_
//Released_Version_5_10_00_17
