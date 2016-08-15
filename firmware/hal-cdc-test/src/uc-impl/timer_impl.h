/* 
   Copyright (c)
     (c) 2015-2016 Chintalagiri Shashank, Quazar Technologies Pvt. Ltd.
   
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

#ifndef TIMER_IMPL_H
#define TIMER_IMPL_H

#include "hal_uc_map.h"
#include "msp430-driverlib/MSP430F5xx_6xx/driverlib.h"

#ifdef uC_INCLUDE_TIMER_IFACE

typedef enum {
    TIMER_HWIF_UNDEF, 
    TIMER_HWIF_TIMER_A, 
    TIMER_HWIF_TIMER_B
} TIMER_HWIF_TYPE;
    
typedef enum {
    TIMER_MODE_STOPPED, 
    TIMER_MODE_PERIODIC, 
    TIMER_MODE_COUNTING, 
    TIMER_MODE_PWM
} TIMER_MODE;

typedef enum {
    TIMER_PRESCALER_DIV1, 
    TIMER_PRESCALER_DIV2, 
    TIMER_PRESCALER_DIV3,
    TIMER_PRESCALER_DIV4, 
    TIMER_PRESCALER_DIV5, 
    TIMER_PRESCALER_DIV6,
    TIMER_PRESCALER_DIV7, 
    TIMER_PRESCALER_DIV8,
    TIMER_PRESCALER_DIV10, 
    TIMER_PRESCALER_DIV12, 
    TIMER_PRESCALER_DIV14,
    TIMER_PRESCALER_DIV16, 
    TIMER_PRESCALER_DIV20, 
    TIMER_PRESCALER_DIV24,
    TIMER_PRESCALER_DIV28, 
    TIMER_PRESCALER_DIV32,
    TIMER_PRESCALER_DIV40, 
    TIMER_PRESCALER_DIV48,
    TIMER_PRESCALER_DIV56, 
    TIMER_PRESCALER_DIV64,
} TIMER_PRESCALER;


typedef struct _TIMER_HWIF_t{
    const TIMER_HWIF_TYPE type;
    const HAL_ADDRESS_t base;
    const HAL_BASE_t primary_vector;
    const HAL_BASE_t secondary_vector;
}_timer_hwif_t;

typedef struct _TIMER_STATE_t{
    TIMER_MODE mode;
}timer_state_t;

typedef struct TIMER_IF_t{
    const _timer_hwif_t *const hwif;
    timer_state_t *const state;
}timer_if_t;

extern const timer_if_t *const timer_if[];

#if uC_TIMER0_ENABLED
    extern const timer_if_t timer0_if;
#endif
#if uC_TIMER1_ENABLED
    extern const timer_if_t timer1_if;
#endif
#if uC_TIMER2_ENABLED
    extern const timer_if_t timer2_if;
#endif
#if uC_TIMER3_ENABLED
    extern const timer_if_t timer3_if;
#endif

    #ifdef uC_INCLUDE_TIMER_A_IFACE
    void timer_A_init(uint8_t intfnum);
    #endif
    
    #ifdef uC_INCLUDE_TIMER_B_IFACE
    void timer_B_init(uint8_t intfnum);
    #endif


static inline void timer_init(uint8_t intfnum){
    if (timer_if[intfnum]->hwif->type == TIMER_HWIF_UNDEF){
        ;
    }
    #ifdef uC_INCLUDE_TIMER_A_IFACE
    else if (timer_if[intfnum]->hwif->type == TIMER_HWIF_TIMER_A){
        timer_A_init(intfnum);
    }
    #endif
    #ifdef uC_INCLUDE_TIMER_B_IFACE
    else if(timer_if[intfnum]->hwif->type == TIMER_HWIF_TIMER_B){
        timer_B_init(intfnum);
    }
    #endif
}

static inline void timer_set_prescaler(uint8_t intfnum, uint8_t prescaler){
    uint16_t lprescaler=0, lexdiv=0;
    HWREG16(timer_if[intfnum]->hwif->base + OFS_TAxCTL) &= ~(ID0 | ID1);
    switch(prescaler){
        case TIMER_PRESCALER_DIV1:
            lexdiv = TBIDEX__1;
            lprescaler = ID__1;
            break;
        case TIMER_PRESCALER_DIV2:
            lexdiv = TBIDEX__1;
            lprescaler = ID__2;
            break;
        case TIMER_PRESCALER_DIV3:
            lexdiv = TBIDEX__3;
            lprescaler = ID__1;
            break;
        case TIMER_PRESCALER_DIV4:
            lexdiv = TBIDEX__1;
            lprescaler = ID__4;
            break;
        case TIMER_PRESCALER_DIV5:
            lexdiv = TBIDEX__5;
            lprescaler = ID__1;
            break;
        case TIMER_PRESCALER_DIV6:
            lexdiv = TBIDEX__3;
            lprescaler = ID__2;
            break;
        case TIMER_PRESCALER_DIV7:
            lexdiv = TBIDEX__7;
            lprescaler = ID__1;
            break;
        case TIMER_PRESCALER_DIV8:
            lexdiv = TBIDEX__1;
            lprescaler = ID__8;
            break;
        case TIMER_PRESCALER_DIV10:
            lexdiv = TBIDEX__5;
            lprescaler = ID__2;
            break;
        case TIMER_PRESCALER_DIV12:
            lexdiv = TBIDEX__3;
            lprescaler = ID__4;
            break;
        case TIMER_PRESCALER_DIV14:
            lexdiv = TBIDEX__7;
            lprescaler = ID__2;
            break;
        case TIMER_PRESCALER_DIV16:
            lexdiv = TBIDEX__2;
            lprescaler = ID__8;
            break;
        case TIMER_PRESCALER_DIV20:
            lexdiv = TBIDEX__5;
            lprescaler = ID__4;
            break;
        case TIMER_PRESCALER_DIV24:
            lexdiv = TBIDEX__6;
            lprescaler = ID__4;
            break;
        case TIMER_PRESCALER_DIV28:
            lexdiv = TBIDEX__7;
            lprescaler = ID__4;
            break;
        case TIMER_PRESCALER_DIV32:
            lexdiv = TBIDEX__4;
            lprescaler = ID__8;
            break;
        case TIMER_PRESCALER_DIV40:
            lexdiv = TBIDEX__5;
            lprescaler = ID__8;
            break;
        case TIMER_PRESCALER_DIV48:
            lexdiv = TBIDEX__6;
            lprescaler = ID__8;
            break;
        case TIMER_PRESCALER_DIV56:
            lexdiv = TBIDEX__7;
            lprescaler = ID__8;
            break;
        case TIMER_PRESCALER_DIV64:
            lexdiv = TBIDEX__8;
            lprescaler = ID__8;
            break;
        default:
            return;
    }
    HWREG16(timer_if[intfnum]->hwif->base + OFS_TAxEX0) = lexdiv;
    HWREG16(timer_if[intfnum]->hwif->base + OFS_TAxCTL) |= lprescaler;
    HWREG16(timer_if[intfnum]->hwif->base + OFS_TAxCTL) |= TACLR;
    return;
}

static inline void timer_set_mode(uint8_t intfnum, uint8_t mode){
    uint16_t lmode;
    timer_if[intfnum]->state->mode = mode;
    HWREG16(timer_if[intfnum]->hwif->base + OFS_TAxCTL) &= ~(MC0 | MC1);
    switch(mode){
        case TIMER_MODE_STOPPED:
            return;
            break;
        case TIMER_MODE_PERIODIC:
            lmode = MC__UP;
            break;
        case TIMER_MODE_COUNTING:
            lmode = MC__CONTINUOUS;
            break;
        case TIMER_MODE_PWM:
            lmode = MC__UPDOWN;
            break;
        default:
            return;
    }
    HWREG16(timer_if[intfnum]->hwif->base + OFS_TAxCTL) |= (lmode | TACLR);
    return;
}

static inline void timer_enable_int_overflow(uint8_t intfnum){
    HWREG16(timer_if[intfnum]->hwif->base + OFS_TAxCTL) |= TAIE;
};

static inline void timer_disable_int_overflow(uint8_t intfnum){
    HWREG16(timer_if[intfnum]->hwif->base + OFS_TAxCTL) &= ~TAIE;
};

static inline void timer_set_top(uint8_t intfnum, uint16_t top){
    timer_set_cmr_ch(intfnum, 0, top);
}

static inline void timer_enable_int_top(uint8_t intfnum){
    timer_enable_int_ch(intfnum, 0);
};

static inline void timer_disable_int_top(uint8_t intfnum){
    timer_disable_int_ch(intfnum, 0);
};

static inline uint16_t timer_chctl_offset(uint8_t channel);

static inline uint16_t timer_chctl_offset(uint8_t channel){
    return (0x02 + 2 * channel);
}

static inline void timer_enable_int_ch ( uint8_t intfnum, uint8_t channel){
    HWREG16(timer_if[intfnum]->hwif->base + timer_chctl_offset(channel)) |= CCIE;
}

static inline void timer_disable_int_ch( uint8_t intfnum, uint8_t channel){
    HWREG16(timer_if[intfnum]->hwif->base + timer_chctl_offset(channel)) &= ~CCIE;
}

static inline void timer_enable_out_ch ( uint8_t intfnum, uint8_t channel){
    ;
}

static inline void timer_disable_out_ch( uint8_t intfnum, uint8_t channel){
    ;
}

static inline uint16_t timer_chccr_offset(uint8_t channel);

static inline uint16_t timer_chccr_offset(uint8_t channel){
    return (OFS_TAxR + 0x02 + 2 * channel);
}

static inline void timer_set_cmr_ch(uint8_t intfnum, uint8_t channel, uint16_t cm){
    HWREG16(timer_if[intfnum]->hwif->base + timer_chccr_offset(channel)) = cm;
}
    
#endif

#endif