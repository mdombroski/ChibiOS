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
 * @file    RC/STM8/chcore.c
 * @brief   STM8 (Raisonance) architecture port code.
 *
 * @addtogroup STM8_RAISONANCE_CORE
 * @{
 */
#pragma SRC("tmp.asm")

#include "ch.h"

page0 ReadyList rlist;

/**
 * @brief   Performs a context switch between two threads.
 *
 * @param otp   the thread to be switched out
 */
void _port_switch(chThread *otp) {

  (void)otp;
  /* Asm because unoptimal code would generated by using _getSP_().*/
#pragma ASM
        LDW     Y,SP                ; old context pointer
        LDW     (5,X),Y             ; SP saved in otp->p_ctx.sp
        LDW     X,rlist + 5         ; r_current (currp) field
        LDW     X,(5,X)             ; currp->p_ctx.sp
        LDW     SP,X                ; new context pointer
#pragma ENDASM
}

/**
 * @brief   Thread start code.
 */
void _port_thread_start(void) {

  chSysUnlock();
#pragma ASM
        POPW    X
#pragma ENDASM
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

/** @} */
