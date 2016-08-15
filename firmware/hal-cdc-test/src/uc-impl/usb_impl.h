/* 
 * Copyright (c)
 *   (c) 2015-2016 Chintalagiri Shashank, Quazar Technologies Pvt. Ltd.
 *  
 * This file is part of
 * Embedded bootstraps : Peripheral driver implementations : MSP430
 * 
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 */

#include "hal_uc_map.h"
#include "hal_uc_timer.h"
#include <stddef.h>

#ifdef uC_INCLUDE_USB_IFACE
#include "usbcdc_impl.h"

#include "usb-impl/USB_API/USB_Common/usb.h"

static inline void usb_init(void){
    USB_setup(TRUE, TRUE);
}

static inline void usb_enable(void){
    USB_enable();
}

static inline void usb_disable(void){
    USB_disable();
}

static inline void usb_connect(void){
    USB_connect();
}

static inline void usb_disconnect(void){
    USB_disconnect();
}

static inline uint8_t usb_status(void){
    return USB_getConnectionState();
}

#endif