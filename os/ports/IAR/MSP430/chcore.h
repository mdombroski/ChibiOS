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
 * @file    IAR/MSP430/chcore.h
 * @brief   MSP430 architecture port macros and structures.
 *
 * @addtogroup MSP430_IAR_CORE
 * @{
 */

#ifndef _CHCORE_H_
#define _CHCORE_H_


#include <msp430.h>
#include <intrinsics.h>

#if !(defined __CORE__&& defined __430__ && defined __430X__ && \
      ((__CORE__==__430__)||(__CORE__==__430X__)))
#error Compiler has not defined __CORE__ (needed)
#endif

#define _PORT_PRAGMA(x) _Pragma(#x)
#define _PORT_JOIN(x,y) _PORT_JOIN2(x,y)
#define _PORT_JOIN2(x,y) _PORT_JOIN3(x,y)
#define _PORT_JOIN3(x,y) x##y

/**
 * @brief   Enables the use of a wait state in the idle thread loop.
 */
#ifndef ENABLE_WFI_IDLE
#define ENABLE_WFI_IDLE                 0
#endif

/**
 * @brief   Selects the low power idle mode (LPM0-LPM4)
 */
#ifndef IDLE_LPM_BITS
#define IDLE_LPM_BITS                   LPM1_bits
#endif

#if ENABLE_WFI_IDLE
#define PORT_SLEEP()      __bis_SR_register( IDLE_LPM_BITS )
#define PORT_SLEEP_IRQ()  __bis_SR_register_on_exit( IDLE_LPM_BITS )
#define PORT_WAKE()       __bic_SR_register( IDLE_LPM_BITS )
#define PORT_WAKE_IRQ()   __bic_SR_register_on_exit( IDLE_LPM_BITS )
#else
#define PORT_SLEEP()      __no_operation()
#define PORT_SLEEP_IRQ()
#define PORT_WAKE()
#define PORT_WAKE_IRQ()
#endif

/**
 * @brief   Macro defining the MSP430 architecture.
 */
#define CH_ARCHITECTURE_MSP430

/**
 * @brief   Name of the implemented architecture.
 */
#define CH_ARCHITECTURE_NAME            "MSP430"

/**
 * @brief   Name of the architecture variant (optional).
 */
#define CH_CORE_VARIANT_NAME            "MSP430"

/**
 * @brief   Name of the compiler supported by this port.
 */
#define CH_COMPILER_NAME                "IAR " __VER__

/**
 * @brief   Port-specific information string.
 */
#define CH_PORT_INFO                    "None"

/**
 * @brief   16 bits stack and memory alignment enforcement.
 */
typedef uint16_t stkalign_t;

/**
 * @brief   Generic MSP430 register.
 */
typedef void __data16* regmsp_t;

/**
 * @brief   Generic MSP430X register.
 */
#if __REGISTER_MODEL__ == __REGISTER_MODEL_REG20__
typedef void __data20* regmspx_t;
#else
typedef unsigned long regmspx_t;
#endif

#if __CORE__ == __430X__
typedef regmspx_t _regmsp_t;
#else
typedef regmsp_t _regmsp_t;
#endif


/**
 * @brief   Interrupt saved context.
 * @details This structure represents the stack frame saved during a
 *          preemption-capable interrupt handler.
 */
struct extctx {
  _regmsp_t      r12;
  _regmsp_t      r13;
  _regmsp_t      r14;
  _regmsp_t      r15;
  _regmsp_t      sr;
  _regmsp_t      pc;
};

/**
 * @brief   System saved context.
 * @details This structure represents the inner stack frame during a context
 *          switching.
 */
struct intctx {
  _regmsp_t      r4;
  _regmsp_t      r5;
  _regmsp_t      r6;
  _regmsp_t      r7;
  _regmsp_t      r8;
  _regmsp_t      r9;
  _regmsp_t      r10;
  _regmsp_t      r11;
  _regmsp_t      pc;
};

/**
 * @brief   Platform dependent part of the @p chThread structure.
 * @details This structure usually contains just the saved stack pointer
 *          defined as a pointer to a @p intctx structure.
 */
struct context {
  struct intctx __data16 *sp;
};

/**
 * @brief   Platform dependent part of the @p chThdCreateI() API.
 * @details This code usually setup the context switching frame represented
 *          by an @p intctx structure.
 */
#define SETUP_CONTEXT(workspace, wsize, pf, arg) {                          \
  tp->p_ctx.sp = (struct intctx __data16*)((uint8_t __data16*)workspace +   \
                                   wsize -                                  \
                                   sizeof(struct intctx));                  \
  tp->p_ctx.sp->r10	= (_regmsp_t) pf;                                       \
  tp->p_ctx.sp->r11	= (_regmsp_t) arg;                                      \
  tp->p_ctx.sp->pc	= (_regmsp_t) _port_thread_start;                       \
}

/**
 * @brief   Stack size for the system idle thread.
 * @details This size depends on the idle thread implementation, usually
 *          the idle thread should take no more space than those reserved
 *          by @p PORT_INT_REQUIRED_STACK.
 */
#ifndef PORT_IDLE_THREAD_STACK_SIZE
#define PORT_IDLE_THREAD_STACK_SIZE     0
#endif

/**
 * @brief   Per-thread stack overhead for interrupts servicing.
 * @details This constant is used in the calculation of the correct working
 *          area size.
 *          This value can be zero on those architecture where there is a
 *          separate interrupt stack and the stack space between @p intctx and
 *          @p extctx is known to be zero.
 * @note    In this port the default is 32 bytes per thread.
 */
#ifndef PORT_INT_REQUIRED_STACK
#define PORT_INT_REQUIRED_STACK         16
#endif

/**
 * @brief   Stack size reserved for interrupt processing
 * @details This constant is used in the calculation of the interrupt stack
 *          area.
 *          This value can be zero on systems not requiring interrupt stack.
 * @note    In this port the default is 64 bytes.
 */
#ifndef PORT_IRQ_STACK_SIZE
#define PORT_IRQ_STACK_SIZE             64
#endif

/**
 * @brief   Enforces a correct alignment for a stack area size value.
 */
#define STACK_ALIGN(n) ((((n) - 1) | (sizeof(stkalign_t) - 1)) + 1)

/**
 * @brief   Computes the thread working area global size.
 */
#define THD_WA_SIZE(n) STACK_ALIGN(sizeof(chThread) +                         \
                                   sizeof(struct intctx) +                  \
                                   sizeof(struct extctx) +                  \
                                   (n) + 4 + (PORT_INT_REQUIRED_STACK))

/**
 * @brief   Static working area allocation.
 * @details This macro is used to allocate a static thread working area
 *          aligned as both position and size.
 */
#define WORKING_AREA(s, n) stkalign_t s[THD_WA_SIZE(n) / sizeof(stkalign_t)]

/**
 * @brief   IRQ prologue code.
 * @details This macro must be inserted at the start of all IRQ handlers
 *          enabled to invoke system APIs.
 */
#define PORT_IRQ_PROLOGUE() port_irq_enter()

/**
 * @brief   IRQ epilogue code.
 * @details This macro must be inserted at the end of all IRQ handlers
 *          enabled to invoke system APIs.
 */
#define PORT_IRQ_EPILOGUE() {                                               \
  port_irq_exit();                                                          \
  dbg_check_lock();                                                         \
  if (chSchIsPreemptionRequired())                                          \
    chSchDoReschedule();                                                    \
  dbg_check_unlock();                                                       \
  PORT_WAKE_IRQ();                                                          \
}

/**
 * @brief   IRQ handler function declaration.
 * @note    @p id can be a function name or a vector number depending on the
 *          port implementation.
 */
#define PORT_IRQ_HANDLER(id)                                                \
  _PORT_PRAGMA(vector=_PORT_JOIN(id,_VECTOR))                               \
  __interrupt void _PORT_JOIN(port_isr_,id) (void)

/**
 * @brief   Port-related initialization code.
 * @note    This function is empty in this port.
 */
#define port_init()

/* interrupt enable/disable state */
extern istate_t _port_istate;

/**
 * @brief   Kernel-lock action.
 * @details Stores interrupt state and globally disables interrupts.
  */
#define port_lock() {                                                       \
  istate_t t = __get_interrupt_state();                                     \
  __disable_interrupt();                                                    \
  _port_istate = t;                                                         \
}

/**
 * @brief   Kernel-unlock action.
 * @details Restores previously saved interrupt state.
 */
#define port_unlock() {                                                     \
  __set_interrupt_state( _port_istate );                                    \
}

/**
 * @brief   Kernel-lock action from an interrupt handler.
 * @details This function is invoked before invoking I-class APIs from
 *          interrupt handlers. The implementation is architecture dependent,
 *          in its simplest form it is void.
 * @note    This function is empty in this port.
 */
#define port_lock_from_isr()

/**
 * @brief   Kernel-unlock action from an interrupt handler.
 * @details This function is invoked after invoking I-class APIs from interrupt
 *          handlers. The implementation is architecture dependent, in its
 *          simplest form it is void.
 * @note    This function is empty in this port.
 */
#define port_unlock_from_isr()

/**
 * @brief   Disables all the interrupt sources.
 * @note    Of course non-maskable interrupt sources are not included.
 * @note    Implemented as global interrupt disable.
 */
#define port_disable() __disable_interrupt()

/**
 * @brief   Disables the interrupt sources below kernel-level priority.
 * @note    Interrupt sources above kernel level remains enabled.
 * @note    Same as @p port_disable() in this port, there is no difference
 *          between the two states.
 */
#define port_suspend() __disable_interrupt()

/**
 * @brief   Enables all the interrupt sources.
 * @note    Implemented as global interrupt enable.
 */
#define port_enable() __enable_interrupt()

/**
 * @brief   Enters an architecture-dependent IRQ-waiting mode.
 * @details The function is meant to return when an interrupt becomes pending.
 *          The simplest implementation is an empty function or macro but this
 *          would not take advantage of architecture-specific power saving
 *          modes.
 * @note    This port function is implemented as inlined code for performance
 *          reasons.
 * @note    PORT_SLEEP enters a low power mode when @p ENABLE_WFI_IDLE and
 *          @p IDLE_LPM_BITS are enabled. port_wait_for_interrupt could also be
 *          defined externally.
 */
#ifndef port_wait_for_interrupt
#if ENABLE_WFI_IDLE
#define port_wait_for_interrupt() PORT_SLEEP()
#else
#define port_wait_for_interrupt()
#endif
#endif

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
#if !CH_DBG_ENABLE_STACK_CHECK || defined(__DOXYGEN__)
#define port_switch(ntp, otp) _port_switch(ntp, otp)
#else
#define port_switch(ntp, otp) {                                             \
  unsigned short limit = __get_SP_register() - sizeof(struct intctx);       \
  if ((stkalign_t*) limit < otp->p_stklimit)                                \
    chDbgPanic("stack overflow");                                           \
  _port_switch(ntp, otp);                                                   \
}
#endif

#if PORT_IRQ_STACK_SIZE > 0 || defined(__DOXYGEN__)
extern stkalign_t _port_irq_stack[ PORT_IRQ_STACK_SIZE / sizeof(stkalign_t) ];
#define irq_stack_top (((stkalign_t) _port_irq_stack ) + sizeof(_port_irq_stack))
#define port_irq_enter() {                                                  \
  currp->p_ctx.sp = (struct intctx __data16*) __get_SP_register();          \
  __set_SP_register( irq_stack_top );                                       \
}
#define port_irq_exit() __set_SP_register((unsigned short)currp->p_ctx.sp)
#else
#define port_irq_enter()
#define port_irq_exit()
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void port_halt(void);
  void _port_lock(void);
  void _port_unlock(void);
  void _port_switch(chThread *ntp, chThread *otp);
  void _port_thread_start(void);
#ifdef __cplusplus
}
#endif

#endif /* _CHCORE_H_ */

/** @} */
