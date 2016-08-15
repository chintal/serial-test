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
#include <stddef.h>

#ifdef uC_INCLUDE_USB_IFACE

#include "usb-impl/USB_API/USB_Common/usb.h"
#include "usb-impl/USB_API/USB_CDC_API/usbcdc.h"

#if uC_USBCDC_ENABLED

static inline void usbcdc_init(uint8_t inftnum){
    ;
}

static inline void usbcdc_send_trigger(uint8_t intfnum){
    ;
}

static inline void usbcdc_send_flush(uint8_t intfnum){
    ;
}

static inline uint8_t usbcdc_reqlock(uint8_t intfnum, uint8_t len, uint8_t token){
    return 0;
}

static inline uint8_t usbcdc_putc(uint8_t intfnum, uint8_t byte, uint8_t token, uint8_t handlelock){
    return 0;
}

static inline uint8_t usbcdc_write(uint8_t intfnum, void *buffer, uint8_t len, uint8_t token){
    return 0;
}
                                  
static inline uint8_t usbcdc_txready(uint8_t intfNum){
    return 0;
}

static inline uint8_t usbcdc_getc(uint8_t intfnum){
    return 0;
}

static inline uint8_t usbcdc_read(uint8_t intfnum, void *buffer, uint8_t len){
    return 0;
}

static inline uint8_t usbcdc_population_rxb(uint8_t intfNum){
    return USBCDC_getBytesInUSBBuffer(intfNum);
}

static inline void usbcdc_discard_rxb(uint8_t intfNum){
    USBCDC_rejectData(intfNum);
}

#endif

#endif