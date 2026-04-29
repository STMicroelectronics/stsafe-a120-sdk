/******************************************************************************
 * \file    delay_ms.h
 * \brief   Millisecond delay inline helpers for Linux (STM32MP1)
 *          Uses POSIX nanosleep() / clock_gettime() directly — no driver
 *          compilation unit required.
 * \author  STMicroelectronics - CS application team
 *
 ******************************************************************************
 * \attention
 *
 * <h2><center>&copy; COPYRIGHT 2022 STMicroelectronics</center></h2>
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#ifndef DELAY_MS_H
#define DELAY_MS_H

#include <stdint.h>
#include <time.h>

static inline void delay_ms(uint16_t ms) {
    struct timespec ts;
    ts.tv_sec  = ms / 1000U;
    ts.tv_nsec = (long)(ms % 1000U) * 1000000L;
    nanosleep(&ts, NULL);
}

#endif /* DELAY_MS_H */
