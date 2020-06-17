/*
 *  Copyright 2020 Andreas Messer <andi@bastelmap.de>
 *
 *  This file is part of the Embedded C++ Platform Project.
 *
 *  Embedded C++ Platform Project (ECPP) is free software: you can
 *  redistribute it and/or modify it under the terms of the GNU General
 *  Public License as published by the Free Software Foundation,
 *  either version 3 of the License, or (at your option) any later
 *  version.
 *
 *  Embedded C++ Platform Project is distributed in the hope that it
 *  will be useful, but WITHOUT ANY WARRANTY; without even the implied
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ECPP.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  As a special exception, the copyright holders of ECPP give you
 *  permission to link ECPP with independent modules to produce an
 *  executable, regardless of the license terms of these independent
 *  modules, and to copy and distribute the resulting executable under
 *  terms of your choice, provided that you also meet, for each linked
 *  independent module, the terms and conditions of the license of that
 *  module.  An independent module is a module which is not derived from
 *  or based on ECPP.  If you modify ECPP, you may extend this exception
 *  to your version of ECPP, but you are not obligated to do so.  If you
 *  do not wish to do so, delete this exception statement from your
 *  version.
 *  */
#ifndef ECPP_TARGET_ATSAM4S_EXCEPTIONS_HPP_
#define ECPP_TARGET_ATSAM4S_EXCEPTIONS_HPP_

#include "sam4s.h"

/* Avoid C++ Name Mangling for exception handlers */
#ifdef __cplusplus
extern "C" {
#endif

void DefaultReset_Handler (void) __attribute__((noreturn));
void DefaultDummy_Handler (void);
void DefaultFault_Handler (void);

void Reset_Handler(void);
void Dummy_Handler(void);
void Fault_Handler(void);


/* Cortex-M4 core handlers */
void NMI_Handler        ( void );
void HardFault_Handler  ( void );
void MemManage_Handler  ( void );
void BusFault_Handler   ( void );
void UsageFault_Handler ( void );
void SVC_Handler        ( void );
void DebugMon_Handler   ( void );
void PendSV_Handler     ( void );
void SysTick_Handler    ( void );

/* ATSAM4S Peripherals handlers */
void SUPC_Handler   ( void );
void RSTC_Handler   ( void );
void RTC_Handler    ( void );
void RTT_Handler    ( void );
void WDT_Handler    ( void );
void PMC_Handler    ( void );
void EFC0_Handler   ( void );
#ifdef _SAM4S_EFC1_INSTANCE_
void EFC1_Handler   ( void );
#endif /* _SAM4S_EFC1_INSTANCE_ */
void UART0_Handler  ( void );
void UART1_Handler  ( void );
void PIOA_Handler   ( void );
void PIOB_Handler   ( void );
#ifdef _SAM4S_PIOC_INSTANCE_
void PIOC_Handler   ( void );
#endif /* _SAM4S_PIOC_INSTANCE_ */
void USART0_Handler ( void );
#ifdef _SAM4S_USART1_INSTANCE_
void USART1_Handler ( void );
#endif /* _SAM4S_USART1_INSTANCE_ */
#ifdef _SAM4S_HSMCI_INSTANCE_
void HSMCI_Handler  ( void );
#endif /* _SAM4S_HSMCI_INSTANCE_ */
void TWI0_Handler   ( void );
void TWI1_Handler   ( void );
void SPI_Handler    ( void );
void SSC_Handler    ( void );
void TC0_Handler    ( void );
void TC1_Handler    ( void );
void TC2_Handler    ( void );
#ifdef _SAM4S_TC1_INSTANCE_
void TC3_Handler    ( void );
#endif /* _SAM4S_TC1_INSTANCE_ */
#ifdef _SAM4S_TC1_INSTANCE_
void TC4_Handler    ( void );
#endif /* _SAM4S_TC1_INSTANCE_ */
#ifdef _SAM4S_TC1_INSTANCE_
void TC5_Handler    ( void );
#endif /* _SAM4S_TC1_INSTANCE_ */
void ADC_Handler    ( void );
#ifdef _SAM4S_DACC_INSTANCE_
void DACC_Handler   ( void );
#endif /* _SAM4S_DACC_INSTANCE_ */
void PWM_Handler    ( void );
void CRCCU_Handler  ( void );
void ACC_Handler    ( void );
void UDP_Handler    ( void );

extern const DeviceVectors exception_table __attribute__ ((section(".vectors")));

#ifdef __cplusplus
}
#endif



#endif