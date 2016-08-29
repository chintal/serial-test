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
    USBCDC_sendTrigger(intfnum);
}

static inline void usbcdc_send_flush(uint8_t intfnum){
    USBCDC_sendFlush(intfnum);
}

static inline uint8_t usbcdc_reqlock(uint8_t intfnum, uint8_t len, uint8_t token){
    // TODO Implement locking mechanism
    uint8_t avail = USBCDC_getInterfaceStatus_SendAvailable(intfnum);
    if (len){
        if (avail >= len){
            return 0x01;
        }
        else{
            return 0x00;
        }
    }
    else {
        return avail;
    }
}

static inline uint8_t usbcdc_rellock(uint8_t intfnum, uint8_t token){
    return 1;
}

static inline uint8_t usbcdc_putc(uint8_t intfnum, uint8_t byte, uint8_t token, uint8_t handlelock){
    uint8_t stat;
    stat = USBCDC_sendChar(intfnum, byte);
    usbcdc_send_trigger(intfnum);
    return stat;
}

static inline uint8_t usbcdc_write(uint8_t intfnum, void *buffer, uint8_t len, uint8_t token){
    uint8_t rval;
    rval = USBCDC_sendBuffer(intfnum, (uint8_t *) buffer, len);
    usbcdc_send_trigger(intfnum);
    return rval;
}

static inline uint8_t usbcdc_getc(uint8_t intfnum){
    return USBCDC_recieveChar(intfnum);
}

static inline uint8_t usbcdc_read(uint8_t intfnum, void *buffer, uint8_t len){
    return USBCDC_recieveBuffer(intfnum, (uint8_t *) buffer, len);
}

static inline uint8_t usbcdc_population_rxb(uint8_t intfNum){
    return USBCDC_getInterfaceStatus_RecieveWaiting(intfNum);
}

static inline void usbcdc_discard_rxb(uint8_t intfNum){
    USBCDC_rejectData(intfNum);
}

#endif

#endif