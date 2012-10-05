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
 * @file    chmsg.h
 * @brief   Messages macros and structures.
 *
 * @addtogroup messages
 * @{
 */

#ifndef _CHMSG_H_
#define _CHMSG_H_

#if CH_USE_MESSAGES || defined(__DOXYGEN__)

/**
 * @name    Macro Functions
 * @{
 */
/**
 * @brief   Evaluates to TRUE if the thread has pending messages.
 *
 * @iclass
 */
#define chMsgIsPendingI(tp) \
        ((tp)->p_msgqueue.p_next != (chThread *)&(tp)->p_msgqueue)
/** @} */

#ifdef __cplusplus
extern "C" {
#endif
  msg_t chMsgSend(chThread *tp, msg_t msg);
  msg_t chMsgSendTimeout(chThread *tp, msg_t msg, systime_t time);
  msg_t chMsgWait(void);
  msg_t chMsgWaitTimeout(systime_t time);
  void chMsgRelease(msg_t msg);
#ifdef __cplusplus
}
#endif

#endif /* CH_USE_MESSAGES */

#endif /* _CHMSG_H_ */

/** @} */
