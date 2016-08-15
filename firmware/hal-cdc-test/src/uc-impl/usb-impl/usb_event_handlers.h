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
 * ======== Extracted from usb.h ========
 */
#ifndef _USB_EVENT_HANDLERS_H_
#define _USB_EVENT_HANDLERS_H_

#ifdef __cplusplus
extern "C"
{
    #endif
    
/*
 * Event-Handling routines
 */

//*****************************************************************************
//
//! \addtogroup event_handling_api
//! @{
//
//******************************************************************************


//******************************************************************************
//
//! USB PLL has Failed
//!
//! This event signals that the output of the USB PLL has failed. This event may
//! have occurred because XT2, the source of the PLL's reference clock, has
//! failed or is unreliable. If this event occurs, the USB connection will 
//! likely be lost. It is best to handle it by calling USB_disconnect() and
//! attempting a re-connection.
//! 
//! Since this event is associated with a change in state, it's a good
//! practice to return TRUE so the main loop can adapt.
//!
//! \return TRUE to keep CPU awake
//
//******************************************************************************
uint8_t USB_handleClockEvent (void);


//*****************************************************************************
//
//! Valid Voltage Applied to VBUS
//!
//! If this function gets executed, it indicates that a valid voltage has been
//! applied to the VBUS pin; that is, the voltage on VBUS has transitioned from
//! low to high.
//! 
//! This usually means the device has been attached to an active USB host. It is
//! recommended to attempt a USB connection from this handler, as described in 
//! Sec. 6.3 of \e "Programmer???s Guide: MSP430 USB API Stack for CDC/PHDC/HID/MSC."
//! events.
//! 
//! Returns TRUE to wake the main loop (if LPM was entered), so that it can
//! take into account the change in state.
//!
//! \return TRUE to keep CPU awake
//
//*****************************************************************************
uint8_t USB_handleVbusOnEvent (void);


//*****************************************************************************
//
//! Valid Voltage Removed from VBUS
//!
//! This event indicates that a valid voltage has just been removed from the 
//! VBUS pin. That is, the voltage on VBUS has transitioned from high to low.
//! 
//! This generally means the device has been removed from an active USB host. It
//! might also mean the device is still physically attached to the host, but the
//! host went into a standby mode; or it was attached to a powered hub but the
//! host upstream from that hub became inactive. The API automatically responds
//! to a VBUS-off event by powering down the USB module and PLL, which is the
//! equivalent of calling USB_disable(). It then calls this handling function,
//! if enabled.
//! 
//! Since this event is associated with a change in state, it's a good
//! practice to return TRUE so the main loop can adapt.
//!
//! \return TRUE to keep CPU awake
//
//*****************************************************************************
uint8_t USB_handleVbusOffEvent (void);


//*****************************************************************************
//
//! USB Host Issued a Reset
//!
//! This event indicates that the USB host has issued a reset of this USB
//! device. The API handles this automatically, and no action is required by the
//! application to maintain USB operation. After handling the reset, the API
//! calls this handling function, if enabled. In most cases there is no
//! significant reason for the application to respond to bus resets.
//!
//! \return TRUE
//
//*****************************************************************************
uint8_t USB_handleResetEvent (void);


//*****************************************************************************
//
//! USB Host Suspends USB Device
//!
//! This event indicates that the USB host has chosen to suspend this USB device
//! after a period of active operation. It???s important that a bus-powered,
//! suspended USB device limit its consumption of power from VBUS during this
//! time. The API automatically shuts down USB-related circuitry inside the
//! MSP430???s USB module. However, the application may need to shut down other
//! circuitry drawing from VBUS. This handler is a good place to do this.
//! 
//! See Sec.11.1.3 of \e "Programmer???s Guide:
//! MSP430 USB API Stack for CDC/PHDC/HID/MSC." for a complete discussion
//! about handling suspend.
//! 
//! \returns TRUE so that the main loop can adapt.
//
//*****************************************************************************
uint8_t USB_handleSuspendEvent (void);


//*****************************************************************************
//
//! USB Host has Resumed this USB Device
//!
//! This event indicates that the USB host has resumed this USB device from
//! suspend mode. If the device is bus-powered, it is no longer restricted in
//! the amount of power it can draw from VBUS. The API automatically re-enables
//! any circuitry in the MSP430???s USB module that was disabled during suspend.
//! The application can use this handler to re-enable other circuitry as well.
//! 
//! Since this event is associated with a change in state, it's a good
//! practice to return TRUE so the main loop can adapt.
//!
//! \return TRUE
//
//*****************************************************************************
uint8_t USB_handleResumeEvent (void);


//*****************************************************************************
//
//! Device has Become Enumerated
//! 
//! This event indicates that the device has just become enumerated. This
//! corresponds with a state change to ST_ENUM_ACTIVE.
//! 
//! Since this event is associated with a change in state, it's a good
//! practice to return TRUE so the main loop can adapt.
//!
//! \return TRUE
//
//*****************************************************************************
uint8_t USB_handleEnumerationCompleteEvent (void);


#ifdef USE_TIMER_FOR_RESUME
/*
 * When this function gets executed, it indicates that a USB_resume is in progress and the USB
 * stack requires the application to use a timer to wait until the XT2 crystal has
 * stabilized. See crystal specific datasheet for delay times. When the crystal has 
 * stabilized the application needs to call the function USB_enable_PLL() to allow
 * resume to continue.
 */
void USB_handleCrystalStartedEvent(void);

/*
 * When this function gets executed, it indicates that a USB_resume is in progress and the USB
 * stack requires the application to use a timer to wait until the USB PLL has
 * stabilized. See device specific datasheet for PLL delay times. When the PLL has 
 * stabilized the application needs to call the function USB_enable_final() to allow resume
 * to complete.
 */
void USB_handlePLLStartedEvent(void);

#endif

//******************************************************************************
//
// Close the Doxygen group.
//! @}
//
//******************************************************************************

/*----------------------------------------------------------------------------+
 | End of header file        *                                                  |
 +----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif  /*
* _USB_H
*------------------------ Nothing Below This Line --------------------------
*/
//Released_Version_5_10_00_17