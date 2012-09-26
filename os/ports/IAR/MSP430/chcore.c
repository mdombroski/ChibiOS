/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    MSP430/chcore.c
 * @brief   MSP430 architecture port code.
 *
 * @note    Use this .c implementation or the .s43 assembly version, but not
 *          both.
 *
 * @addtogroup MSP430_CORE
 * @{
 */

#include "ch.h"
#include <intrinsics.h>


/*
 * EW430 Calling Convention:
 *   R0 (PC)    program counter
 *   R1 (SP)    stack pointer
 *   R2 (SR)    status register
 *   R3 (CG)    constant generator
 *   R4-R11     preserved by called function
 *   R12-R15    scratch
 *
 * Register Usage:
 *   R0 (PC)    indirectly modified
 *   R1 (SP)    modified
 *   R2 (SR)    not directly modified
 *   R3 (CG)    not used
 *   R4-R11     saved & restored
 *   R12-R14    all used
 *   R15        not used
 *
 *
 * Note that RAM in the MSP430 and MSP430X is located below 64K.
 * Therefore 16-bit stack pointer always suffices.
 */


static istate_t s;

/**
 * @brief   Kernel-lock action.
 * @details Usually this function just disables interrupts but may perform more
 *          actions.
 * @note    Implemented as global interrupt disable.
 */
void port_lock(void)
{
  istate_t t = __get_interrupt_state();
  __disable_interrupt();
  s = t;
}

/**
 * @brief   Kernel-unlock action.
 * @details Usually this function just enables interrupts but may perform more
 *          actions.
 * @note    Implemented as global interrupt enable.
 */
void port_unlock(void)
{
  __set_interrupt_state(s);
}

/**
 * @brief   Performs a context switch between two threads.
 * @details This is the most critical code in any port, this function
 *          is responsible for the context switch between 2 threads.
 * @note    The implementation of this code affects <b>directly</b> the context
 *          switch performance so optimize here as much as you can.
 *
 * @param[in] ntp       the thread to be switched in
 * @param[in] otp       the thread to be switched out
 */
void port_switch(Thread *ntp, Thread *otp) {
  // save outgoing execution state
#if __CORE__ == __430X__
  asm( "pushm.a #8, R11" );
#else
  asm( "push    r11   \n" \
       "push    r10   \n" \
       "push    r9    \n" \
       "push    r8    \n" \
       "push    r7    \n" \
       "push    r6    \n" \
       "push    r5    \n" \
       "push    r4" );
#endif

  // save outgoing stack pointer
  otp->p_ctx.sp = (struct intctx*) __get_SP_register();

  // restore incoming stack pointer
  __set_SP_register( (unsigned short) ntp->p_ctx.sp );

#if __CORE__ == __430X__
  asm( "popm.a #8, R11" );
#else
  // restore incoming execution state
  asm( "pop     r4    \n" \
       "pop     r5    \n" \
       "pop     r6    \n" \
       "pop     r7    \n" \
       "pop     r8    \n" \
       "pop     r9    \n" \
       "pop     r10   \n" \
       "pop     r11" );
#endif
}

/**
 * @brief   Halts the system.
 * @details This function is invoked by the operating system when an
 *          unrecoverable error is detected (for example because a programming
 *          error in the application code that triggers an assertion while in
 *          debug mode).
 */
void port_halt(void) {

  port_disable();
  while (TRUE) {
  }
}

/**
 * @brief   Start a thread by invoking its work function.
 * @details If the work function returns @p chThdExit() is automatically
 *          invoked.
 */
void _port_thread_start(void) {

  chSysUnlock();

  // copy argument pointer into r12
  // call function pointer

#if __CORE__ == __430X__
  asm( "mova    r11, r12 \n"
       "calla   r10" );
#else
  asm( "mov     r11, r12 \n"
       "call    r10" );
#endif

  chThdExit( 0 );
}

/** @} */
