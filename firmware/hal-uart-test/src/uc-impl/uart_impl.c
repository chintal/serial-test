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

#include "hal_uc_map.h"
#include "hal_uc_uart.h"

#include "msp430-driverlib/MSP430F5xx_6xx/usci_a_uart.h"

#ifdef uC_INCLUDE_UART_IFACE

#if uC_UART0_ENABLED

    // BAUD RATE GENERATION
    #define _UART0_DIV_INT              (uC_SMCLK_FRQ_HZ / uC_UART0_BAUD)
    
    #define _UART0_DIV_FRAC_NUMERATOR   (uC_SMCLK_FRQ_HZ - (_UART0_DIV_INT * uC_UART0_BAUD))
    #define _UART0_DIV_FRAC_NUM_X_8     (_UART0_DIV_FRAC_NUMERATOR * 8)
    #define _UART0_DIV_FRAC_X_8         (_UART0_DIV_FRAC_NUM_X_8 / uC_UART0_BAUD)

    #if ((( _UART0_DIV_FRAC_NUM_X_8 - ( _UART0_DIV_FRAC_X_8 * uC_UART0_BAUD )) * 10) / uC_UART0_BAUD < 5)
        #define _UART0_BRS_VAL          (_UART0_DIV_FRAC_X_8)
    #else
        #define _UART0_BRS_VAL          (_UART0_DIV_FRAC_X_8 + 1)
    #endif
    
    uint8_t uart0_txbuffer[uC_UART0_TXBUF_LEN + 1];
    uint8_t uart0_rxbuffer[uC_UART0_RXBUF_LEN + 1];
    bytebuf uart0_txbuf;
    bytebuf uart0_rxbuf;
    uint16_t uart0_default_overrun_counter = 0;
    
    static const _uart_hwif_t _uart0_hwif = {
        uC_UART0_TYPE, uC_UART0_BASE, uC_UART0_VECTOR
    };
    
    uart_state_t uart0_state = {0, &uart0_default_overrun_counter};
    
    const uart_if_t uart0_if = {
        &_uart0_hwif,
        &uart0_state,
        &uart0_txbuf,
        &uart0_rxbuf,
    };
    
    void _uart0_init(void);
    static inline void _uart0_buffer_init(void);
    static inline void _uart0_gpio_init(void);
    static inline void _uart0_peripheral_init(void);

    static inline void _uart0_buffer_init(void){
        bytebuf_vInit(uart0_txbuffer, uC_UART0_TXBUF_LEN, &uart0_txbuf);
        bytebuf_vInit(uart0_rxbuffer, uC_UART0_RXBUF_LEN, &uart0_rxbuf);
    }
    
    static inline void _uart0_gpio_init(void){
        GPIO_setAsPeripheralModuleFunctionInputPin(
            uC_PORTSEL_UART0,
            (uC_PINSEL_UART0_TX | uC_PINSEL_UART0_RX)
        );
    }

    #if uC_UART0_TYPE == UART_HWIF_USCI_A
    static inline void _uart0_peripheral_init(void){
        USCI_A_UART_initParam param = {0};
        param.selectClockSource =   USCI_A_UART_CLOCKSOURCE_SMCLK;
        param.clockPrescalar =      _UART0_DIV_INT;
        param.firstModReg =         0;
        param.secondModReg =        (uint8_t)_UART0_BRS_VAL;
        param.parity =              USCI_A_UART_NO_PARITY;
        param.msborLsbFirst =       USCI_A_UART_LSB_FIRST;
        param.numberofStopBits =    USCI_A_UART_ONE_STOP_BIT;
        param.uartMode =            USCI_A_UART_MODE;
        param.overSampling =        USCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;
        
        USCI_A_UART_init(uC_UART0_BASE, &param);
        USCI_A_UART_enable(uC_UART0_BASE);  
        USCI_A_UART_clearInterrupt(uC_UART0_BASE, USCI_A_UART_RECEIVE_INTERRUPT);
        USCI_A_UART_enableInterrupt(uC_UART0_BASE, USCI_A_UART_RECEIVE_INTERRUPT);
    }
    #endif
    
    void _uart0_init(void){
        _uart0_buffer_init();
        _uart0_gpio_init();        
        _uart0_peripheral_init();
    }
#endif
    
#if uC_UART1_ENABLED

    // BAUD RATE GENERATION
    #define _UART1_DIV_INT              (uC_SMCLK_FRQ_HZ / uC_UART1_BAUD)
    
    #define _UART1_DIV_FRAC_NUMERATOR   (uC_SMCLK_FRQ_HZ - (_UART1_DIV_INT * uC_UART1_BAUD))
    #define _UART1_DIV_FRAC_NUM_X_8     (_UART1_DIV_FRAC_NUMERATOR * 8)
    #define _UART1_DIV_FRAC_X_8         (_UART1_DIV_FRAC_NUM_X_8 / uC_UART1_BAUD)

    #if ((( _UART1_DIV_FRAC_NUM_X_8 - ( _UART1_DIV_FRAC_X_8 * uC_UART1_BAUD )) * 10) / uC_UART1_BAUD < 5)
        #define _UART1_BRS_VAL          (_UART1_DIV_FRAC_X_8)
    #else
        #define _UART1_BRS_VAL          (_UART1_DIV_FRAC_X_8 + 1)
    #endif
        
    uint8_t uart1_txbuffer[uC_UART1_TXBUF_LEN + 1];
    uint8_t uart1_rxbuffer[uC_UART1_RXBUF_LEN + 1];
    bytebuf uart1_txbuf;
    bytebuf uart1_rxbuf;
    uint16_t uart1_default_overrun_counter = 0;
    
    static const _uart_hwif_t _uart1_hwif = {
        uC_UART1_TYPE, uC_UART1_BASE, uC_UART1_VECTOR
    };
    
    uart_state_t uart1_state = {0, &uart1_default_overrun_counter};
    
    const uart_if_t uart1_if = {
        &_uart1_hwif,
        &uart1_state,
        &uart1_txbuf,
        &uart1_rxbuf,
    };  
    
    void _uart1_init(void);
    static inline void _uart1_buffer_init(void);
    static inline void _uart1_gpio_init(void);

    static inline void _uart1_buffer_init(void){
        bytebuf_vInit(uart1_txbuffer, uC_UART1_TXBUF_LEN, &uart1_txbuf);
        bytebuf_vInit(uart1_rxbuffer, uC_UART1_RXBUF_LEN, &uart1_rxbuf);
    }
    
    static inline void _uart1_gpio_init(void){
        GPIO_setAsPeripheralModuleFunctionInputPin(
            uC_PORTSEL_UART1,
            (uC_PINSEL_UART1_TX | uC_PINSEL_UART1_RX)
        );
    }
    
    #if uC_UART1_TYPE == UART_HWIF_USCI_A
    static inline void _uart1_peripheral_init(void){
        USCI_A_UART_initParam param = {0};
        param.selectClockSource =   USCI_A_UART_CLOCKSOURCE_SMCLK;
        param.clockPrescalar =      _UART1_DIV_INT;
        param.firstModReg =         0;
        param.secondModReg =        (uint8_t)_UART1_BRS_VAL;
        param.parity =              USCI_A_UART_NO_PARITY;
        param.msborLsbFirst =       USCI_A_UART_LSB_FIRST;
        param.numberofStopBits =    USCI_A_UART_ONE_STOP_BIT;
        param.uartMode =            USCI_A_UART_MODE;
        param.overSampling =        USCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;
        
        USCI_A_UART_init(uC_UART1_BASE, &param);
        USCI_A_UART_enable(uC_UART1_BASE);  
        USCI_A_UART_clearInterrupt(uC_UART1_BASE, USCI_A_UART_RECEIVE_INTERRUPT);
        USCI_A_UART_enableInterrupt(uC_UART1_BASE, USCI_A_UART_RECEIVE_INTERRUPT);
    }
    #endif

    void _uart1_init(void){
        _uart1_buffer_init();
        _uart1_gpio_init();        
        _uart1_peripheral_init();
    }
#endif

const uart_if_t *const uart_if[2] = {
    #if uC_UART0_ENABLED
        &uart0_if
    #else
        NULL
    #endif
    ,
    #if uC_UART1_ENABLED
        &uart1_if
    #else
        NULL
    #endif
};

void (*const _uart_init_func[2])(void) = {
    #if uC_UART0_ENABLED
        _uart0_init
    #else
        NULL
    #endif
    ,
    #if uC_UART1_ENABLED
        _uart1_init
    #else
        NULL
    #endif
};

void uart_init(uint8_t intfnum){
    (*_uart_init_func[intfnum])();
}

uint8_t uart_vprintf_buf(uint8_t intfnum, const char *format, va_list * args){
        uint8_t stat, lstat;
        stat = bytebuf_cPushReqBlindLock(uart_if[intfnum]->txbuf, BYTEBUF_TOKEN_PRINT);
        if (stat){
            stat = vbprintf(uart_if[intfnum]->txbuf, format, *args);
            lstat = bytebuf_cPushRelinquishLock(uart_if[intfnum]->txbuf, BYTEBUF_TOKEN_PRINT);
            uart_send_trigger(intfnum);
            if (lstat){
                return stat;
            }
            else{
                return 0;
            }
        }
        else{
            return 0;
        }
}
#endif