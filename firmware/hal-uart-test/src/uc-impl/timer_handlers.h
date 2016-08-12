/* 
   Copyright (c)
    (c) 2016 Chintalagiri Shashank, Quazar Technologies Pvt. Ltd.
      
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

#ifndef TIMER_HANDLERS_H
#define TIMER_HANDLERS_H

extern volatile uint8_t __timer_handler_inclusion;

/**
 * @name Timer Interrupt Handlers
 */
/**@{*/ 
#if uC_TIMER0_ENABLED
void _timer0_primary_irqhandler(void);
void _timer0_secondary_irqhandler(void);
#endif

#if uC_TIMER1_ENABLED
void _timer1_primary_irqhandler(void);
void _timer1_secondary_irqhandler(void);
#endif

#if uC_TIMER2_ENABLED
void _timer2_primary_irqhandler(void);
void _timer2_secondary_irqhandler(void);
#endif

#if uC_TIMER3_ENABLED
void _timer3_primary_irqhandler(void);
void _timer3_secondary_irqhandler(void);
#endif
/**@}*/ 
#endif
