/* 
   Copyright 2015 Quazar Technologies Pvt. Ltd.
   Copyright 2015 Chintalagiri Shashank
   
   This file is part of
   Embedded bootstraps : Peripheral driver implementations : MSP430
   
   This library is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published
   by the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/

#include "hal_uc_map.h"
#include "hal_uc_timer.h"
#include <stddef.h>

#include "msp430-driverlib/MSP430F5xx_6xx/timer_a.h"
#include "msp430-driverlib/MSP430F5xx_6xx/timer_b.h"

#ifdef uC_INCLUDE_TIMER_IFACE

#if uC_TIMER0_ENABLED
    static const _timer_hwif_t _timer0_hwif = {
        uC_TIMER0_TYPE, uC_TIMER0_BASE, 
        uC_TIMER0_PRIMARY_VECTOR, uC_TIMER0_SECONDARY_VECTOR
    };
        
    timer_state_t timer0_state = {0};
        
    const timer_if_t timer0_if = {
        &_timer0_hwif,
        &timer0_state,
    };  
#endif

#if uC_TIMER1_ENABLED
    static const _timer_hwif_t _timer1_hwif = {
        uC_TIMER1_TYPE, uC_TIMER1_BASE, 
        uC_TIMER1_PRIMARY_VECTOR, uC_TIMER1_SECONDARY_VECTOR
    };
        
    timer_state_t timer1_state = {0};
        
    const timer_if_t timer1_if = {
        &_timer1_hwif,
        &timer1_state,
    };  
#endif
    
#if uC_TIMER2_ENABLED
    static const _timer_hwif_t _timer2_hwif = {
        uC_TIMER2_TYPE, uC_TIMER2_BASE, 
        uC_TIMER2_PRIMARY_VECTOR, uC_TIMER2_SECONDARY_VECTOR
    };
        
    timer_state_t timer2_state = {0};
        
    const timer_if_t timer2_if = {
        &_timer2_hwif,
        &timer2_state,
    };
#endif
    
#if uC_TIMER3_ENABLED
    static const _timer_hwif_t _timer3_hwif = {
        uC_TIMER3_TYPE, uC_TIMER3_BASE, 
        uC_TIMER3_PRIMARY_VECTOR, uC_TIMER3_SECONDARY_VECTOR
    };
        
    timer_state_t timer3_state = {0};
        
    const timer_if_t timer3_if = {
        &_timer3_hwif,
        &timer3_state,
    };
#endif

const timer_if_t *const timer_if[4] = {
    #if uC_TIMER0_ENABLED
        &timer0_if
    #else
        NULL
    #endif
    ,
    #if uC_TIMER1_ENABLED
        &timer1_if
    #else
        NULL
    #endif
    ,
    #if uC_TIMER2_ENABLED
        &timer2_if
    #else
        NULL
    #endif
    ,
    #if uC_TIMER3_ENABLED
        &timer3_if
    #else
        NULL
    #endif
};


#ifdef uC_INCLUDE_TIMER_A_IFACE
    void timer_A_init(uint8_t intfnum){
        timer_set_mode(intfnum, TIMER_MODE_STOPPED);
        timer_set_prescaler(intfnum, TIMER_PRESCALER_DIV1);
        timer_disable_int_overflow(intfnum);
        timer_disable_int_top(intfnum);
        HWREG16(timer_if[intfnum]->hwif->base + OFS_TAxCTL) &= ~(TASSEL0 | TASSEL1);
        HWREG16(timer_if[intfnum]->hwif->base + OFS_TAxCTL) |= uC_TIMER_DEFAULT_CLKSOURCE;
        HWREG16(timer_if[intfnum]->hwif->base + OFS_TAxCTL) |= TACLR;
    }
#endif

#ifdef uC_INCLUDE_TIMER_B_IFACE
    void timer_B_init(uint8_t intfnum){
        timer_set_mode(intfnum, TIMER_MODE_STOPPED);
        timer_set_prescaler(intfnum, TIMER_PRESCALER_DIV1);
        timer_disable_int_overflow(intfnum);
        timer_disable_int_top(intfnum);
        HWREG16(timer_if[intfnum]->hwif->base + OFS_TAxCTL) &= ~(CNTL0 | CNTL1);
        HWREG16(timer_if[intfnum]->hwif->base + OFS_TAxCTL) &= ~(TASSEL0 | TASSEL1);
        HWREG16(timer_if[intfnum]->hwif->base + OFS_TAxCTL) |= uC_TIMER_DEFAULT_CLKSOURCE;
        HWREG16(timer_if[intfnum]->hwif->base + OFS_TAxCTL) |= TACLR;
    }
#endif

#endif