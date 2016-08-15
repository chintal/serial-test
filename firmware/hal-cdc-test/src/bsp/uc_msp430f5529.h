/* 
   Copyright (c) 
     (c) 2015-2016 Chintalagiri Shashank, Quazar Technologies Pvt. Ltd.
   
   This file is part of
   Embedded bootstraps : uC Support Package : MSP430F5529
   
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

/**
 * @file uc_msp430f5529.h
 * @brief uC PUM for MSP430F5529
 *
 * This file contains the peripheral usage map for the MSP430F5529.
 */

#ifndef uC_PUM_SPECIFIC_H
#define uC_PUM_SPECIFIC_H

/**
 * @name Watchdog Configuration
 */
/**@{*/ 
#define uC_HAS_WDT             1
#define uC_WDT_PROVIDER        MSP_WDT_A
/**@}*/ 


/**
 * @name Clock System Configuration
 */
/**@{*/ 
    /**
    * @name System Clock Configuration
    */
    /**@{*/ 
    
    #ifndef uC_FLL_FRQ_HZ
        #define uC_FLL_FRQ_HZ        24000000UL
    #endif
    #define uC_FLL_FRQ_KHZ       (uC_FLL_FRQ_HZ / 1000)
    #define uC_FLL_MULT          (uC_FLL_FRQ_HZ / uC_HF_EXT_CLK_FRQ)
    
    #ifndef uC_MCLK_FRQ_HZ
        #define uC_MCLK_FRQ_HZ       24000000UL
    #endif
    #define uC_MCLK_FRQ_KHZ      (uC_MCLK_FRQ_HZ / 1000)
    #define uC_MCLK_FRQ_MHZ      (uC_MCLK_FRQ_KHZ / 1000)
    
    #ifndef uC_SMCLK_FRQ_HZ
        #define uC_SMCLK_FRQ_HZ      24000000UL
    #endif
    #define uC_SMCLK_FRQ_KHZ     (uC_SMCLK_FRQ_HZ / 1000)
    #define uC_SMCLK_FRQ_MHZ     (uC_SMCLK_FRQ_KHZ / 1000)
    
    #ifndef uC_ACLK_FRQ_HZ
        #define uC_ACLK_FRQ_HZ       32768UL
    #endif
    #define uC_ACLK_FRQ_KHZ      (uC_ACLK_FRQ_HZ / 1000)
    
    
    #define CLKSOURCE_MCLK          0
    #define CLKSOURCE_SMCLK         1
    #define CLKSOURCE_ACLK          2
    
    /**@}*/ 
    
/**@}*/ 


/**
 * @name UART Configuration
 */
/**@{*/ 

    #define uC_AVAILABLE_UARTS     2

    /**
    * @name UART 0 Configuration
    */
    /**@{*/ 
    #define uC_HAS_UART0               1
    #ifndef uC_UART0_BAUD          
        #define uC_UART0_BAUD          19200
    #endif
    #ifndef uC_UART0_TXBUF_LEN
        #define uC_UART0_TXBUF_LEN     10
    #endif
    #ifndef uC_UART0_RXBUF_LEN
        #define uC_UART0_RXBUF_LEN     10
    #endif
    
    #define uC_UART0_TYPE          UART_HWIF_USCI_A
    #define uC_UART0_BASE          USCI_A0_BASE
    #define uC_UART0_VECTOR        USCI_A0_VECTOR

    #define uC_PORTSEL_UART0       GPIO_PORT_P3
    #define uC_PINSEL_UART0_TX     GPIO_PIN3
    #define uC_PINSEL_UART0_RX     GPIO_PIN4

    /**@}*/ 
    
    /**
    * @name UART 1 Configuration
    */
    /**@{*/ 
    #define uC_HAS_UART1               1
    #ifndef uC_UART1_BAUD          
        #define uC_UART1_BAUD          19200
    #endif
    #ifndef uC_UART1_TXBUF_LEN
        #define uC_UART1_TXBUF_LEN     10
    #endif
    #ifndef uC_UART1_RXBUF_LEN
        #define uC_UART1_RXBUF_LEN     10
    #endif
    
    #define uC_UART1_TYPE          UART_HWIF_USCI_A
    #define uC_UART1_BASE          USCI_A1_BASE
    #define uC_UART1_VECTOR        USCI_A1_VECTOR

    #define uC_PORTSEL_UART1       GPIO_PORT_P4
    #define uC_PINSEL_UART1_TX     GPIO_PIN4
    #define uC_PINSEL_UART1_RX     GPIO_PIN5

    /**@}*/ 
    
/**@}*/ 


/**
 * @name Timer Configuration
 */
/**@{*/ 

    #define uC_AVAILABLE_TIMERS               4
    #define uC_TIMER_DEFAULT_CLKSOURCE        TASSEL__SMCLK
    #define uC_TIMER_DEFAULT_CLKSOURCE_FREQ   uC_SMCLK_FRQ_HZ

    /**
    * @name TIMER 0 Configuration
    */
    /**@{*/ 
    #define uC_HAS_TIMER0              1
    
    #define uC_TIMER0_TYPE             TIMER_HWIF_TIMER_A
    #define uC_TIMER0_BASE             TIMER_A0_BASE
    #define uC_TIMER0_PRIMARY_VECTOR   TIMER0_A0_VECTOR
    #define uC_TIMER0_SECONDARY_VECTOR TIMER0_A1_VECTOR
    #define uC_TIMER0_CHANNELS         5
    /**@}*/
    
    /**
    * @name TIMER 1 Configuration
    */
    /**@{*/ 
    #define uC_HAS_TIMER1              1
    
    #define uC_TIMER1_TYPE             TIMER_HWIF_TIMER_A
    #define uC_TIMER1_BASE             TIMER_A1_BASE
    #define uC_TIMER1_PRIMARY_VECTOR   TIMER1_A0_VECTOR
    #define uC_TIMER1_SECONDARY_VECTOR TIMER1_A1_VECTOR
    #define uC_TIMER1_CHANNELS         3
    /**@}*/ 
    
    /**
    * @name TIMER 2 Configuration
    */
    /**@{*/ 
    #define uC_HAS_TIMER2              1
    
    #define uC_TIMER2_TYPE             TIMER_HWIF_TIMER_A
    #define uC_TIMER2_BASE             TIMER_A2_BASE
    #define uC_TIMER2_PRIMARY_VECTOR   TIMER2_A0_VECTOR
    #define uC_TIMER2_SECONDARY_VECTOR TIMER2_A1_VECTOR
    #define uC_TIMER2_CHANNELS         3
    /**@}*/ 
    
    /**
    * @name TIMER 3 Configuration
    */
    /**@{*/ 
    #define uC_HAS_TIMER3              1
    #ifndef uC_TIMER3_CLKSOURCE
        #define uC_TIMER3_CLKSOURCE    CLKSOURCE_SMCLK
    #endif
    
    #define uC_TIMER3_TYPE             TIMER_HWIF_TIMER_B
    #define uC_TIMER3_BASE             TIMER_B0_BASE
    #define uC_TIMER3_PRIMARY_VECTOR   TIMER0_B0_VECTOR
    #define uC_TIMER3_SECONDARY_VECTOR TIMER0_B1_VECTOR
    #define uC_TIMER3_CHANNELS         7
    /**@}*/ 
        
/**@}*/ 

#endif