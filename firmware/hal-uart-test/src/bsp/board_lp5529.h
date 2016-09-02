/* 
   Copyright (c) 
     (c) 2015-2016 Chintalagiri Shashank, Quazar Technologies Pvt. Ltd.
   
   This file is part of
   Embedded bootstraps : Board Support Package : TI Launchpad MSP420F5529
   
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
 * @file board_lp5529.h
 * @brief Board PUM for TI Launchpad based on MSP430F5529
 *
 * This file contains the map for the various subsystems in the context
 * of TI Launchpad based on MSP430F5529.
 */

#ifndef BOARD_SPECIFIC_H
#define BOARD_SPECIFIC_H

// Configure available subsystems. Application-specific subsystem definitions 
// go into application.h instead. Interfaces defined here can be disabled using
// the configuration of application.h

/**
 * @name Clock System Configuration
 */
/**@{*/ 

    /**
    * @name High Frequency External Clock Source
    * 
    * This clock frequency should be chosen for each board to be able to 
    * provide the master clock frequency defined / required by the peripheral 
    * implementation. See the clock system configuration implementation for 
    * the target in hal_uc_core.h and core_impl.c and the uC peripherals 
    * library configuration in uc_map_impl.h to determine what the 
    * allowed values are.
    */
    /**@{*/ 
    #define uC_HAS_HF_EXT_CLK      1
    #define uC_HF_EXT_CLK_FRQ      4000000
    #define uC_HF_EXT_TIMEOUT      50000
    /**@}*/ 
    
    /**
    * @name Low Frequency External Clock Source
    */
    /**@{*/ 
    #define uC_HAS_LF_EXT_CLK      1
    #define uC_LF_EXT_CLK_FRQ      32768
    #define uC_LF_EXT_TIMEOUT      50000
    /**@}*/ 
    
/**@}*/ 


/**
 * @name Backchannel Interface Configuration
 */
/**@{*/ 
    /**
    * @name Backchannel Interface Constants
    */
    /**@{*/ 
    #define BCI_UART                  1
    #define BCI_USBCDC                2
    /**@}*/ 
    /**
    * @name Backchannel Interface Configuration
    */
    /**@{*/ 
    #define BOARD_HAS_BCIFACE         1
    #define BOARD_BCIFACE_TYPE        BCI_UART
    #define BOARD_BCIFACE_INTFNUM     1
    /**@}*/ 
/**@}*/ 

/**
 * @name LED Configuration
 * 
 * For use with standard GPIO functions. Special functions
 * to deal with LEDs can be written, but probably more work
 * than is justified.
 */
/**@{*/ 

    #define BOARD_HAS_RED_LED         1
    #define BOARD_RED_LED_PORT        1
    #define BOARD_RED_LED_PIN         _BV(0)
    #define BOARD_RED_LED_POLARITY    1
    
    #define BOARD_HAS_GREEN_LED       1
    #define BOARD_GREEN_LED_PORT      4
    #define BOARD_GREEN_LED_PIN       _BV(7)
    #define BOARD_GREEN_LED_POLARITY  1
    
    
/**@}*/ 


#endif