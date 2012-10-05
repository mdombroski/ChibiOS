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
 * @file    chinline.h
 * @brief   Kernel inlined functions.
 * @details In this file there are a set of inlined functions if the
 *          @p CH_OPTIMIZE_SPEED is enabled.
 */

#ifndef _CHINLINE_H_
#define _CHINLINE_H_

/* If the performance code path has been chosen then all the following
   functions are inlined into the various kernel modules.*/
#if CH_OPTIMIZE_SPEED
static INLINE void prio_insert(chThread *tp, ThreadsQueue *tqp) {

  chThread *cp = (chThread *)tqp;
  do {
    cp = cp->p_next;
  } while ((cp != (chThread *)tqp) && (cp->p_prio >= tp->p_prio));
  tp->p_next = cp;
  tp->p_prev = cp->p_prev;
  tp->p_prev->p_next = cp->p_prev = tp;
}

static INLINE void queue_insert(chThread *tp, ThreadsQueue *tqp) {

  tp->p_next = (chThread *)tqp;
  tp->p_prev = tqp->p_prev;
  tp->p_prev->p_next = tqp->p_prev = tp;
}

static INLINE chThread *fifo_peek(ThreadsQueue *tqp) {
  return tqp->p_next;
}

static INLINE chThread *fifo_remove(ThreadsQueue *tqp) {
  chThread *tp = tqp->p_next;

  (tqp->p_next = tp->p_next)->p_prev = (chThread *)tqp;
  return tp;
}

static INLINE chThread *lifo_remove(ThreadsQueue *tqp) {
  chThread *tp = tqp->p_prev;

  (tqp->p_prev = tp->p_prev)->p_next = (chThread *)tqp;
  return tp;
}

static INLINE chThread *dequeue(chThread *tp) {

  tp->p_prev->p_next = tp->p_next;
  tp->p_next->p_prev = tp->p_prev;
  return tp;
}

static INLINE void list_insert(chThread *tp, ThreadsList *tlp) {

  tp->p_next = tlp->p_next;
  tlp->p_next = tp;
}

static INLINE chThread *list_remove(ThreadsList *tlp) {

  chThread *tp = tlp->p_next;
  tlp->p_next = tp->p_next;
  return tp;
}
#endif /* CH_OPTIMIZE_SPEED */

#endif /* _CHINLINE_H_ */
