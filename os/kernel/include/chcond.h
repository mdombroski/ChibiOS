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
/*
   Concepts and parts of this file have been contributed by Leon Woestenberg.
 */

/**
 * @file    chcond.h
 * @brief   Condition Variables macros and structures.
 *
 * @addtogroup condvars
 * @{
 */

#ifndef _CHCOND_H_
#define _CHCOND_H_

#if CH_USE_CONDVARS || defined(__DOXYGEN__)

/*
 * Module dependencies check.
 */
#if !CH_USE_MUTEXES
#error "CH_USE_CONDVARS requires CH_USE_MUTEXES"
#endif

/**
 * @brief   chCondVar structure.
 */
typedef struct chCondVar {
  ThreadsQueue          c_queue;        /**< @brief chCondVar threads queue.*/
} chCondVar;

#ifdef __cplusplus
extern "C" {
#endif
  void chCondInit(chCondVar *cp);
  void chCondSignal(chCondVar *cp);
  void chCondSignalI(chCondVar *cp);
  void chCondBroadcast(chCondVar *cp);
  void chCondBroadcastI(chCondVar *cp);
  msg_t chCondWait(chCondVar *cp);
  msg_t chCondWaitS(chCondVar *cp);
#if CH_USE_CONDVARS_TIMEOUT
  msg_t chCondWaitTimeout(chCondVar *cp, systime_t time);
  msg_t chCondWaitTimeoutS(chCondVar *cp, systime_t time);
#endif
#ifdef __cplusplus
}
#endif

/**
 * @brief Data part of a static condition variable initializer.
 * @details This macro should be used when statically initializing a condition
 *          variable that is part of a bigger structure.
 *
 * @param[in] name      the name of the condition variable
 */
#define _CONDVAR_DATA(name) {_THREADSQUEUE_DATA(name.c_queue)}

/**
 * @brief Static condition variable initializer.
 * @details Statically initialized condition variables require no explicit
 *          initialization using @p chCondInit().
 *
 * @param[in] name      the name of the condition variable
 */
#define CONDVAR_DECL(name) chCondVar name = _CONDVAR_DATA(name)

#endif /* CH_USE_CONDVARS */

#endif /* _CHCOND_H_ */

/** @} */
