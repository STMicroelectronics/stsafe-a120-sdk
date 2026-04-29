/******************************************************************************
 * \file    stse_platform_delay.c
 * \brief   STSecureElement delay platform for Linux (STM32MP1)
 * \author  STMicroelectronics - CS application team
 *
 ******************************************************************************
 * \attention
 *
 * <h2><center>&copy; COPYRIGHT 2022 STMicroelectronics</center></h2>
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include "stse_conf.h"
#include "stselib.h"
#include <time.h>

static struct timespec timeout_end;

stse_ReturnCode_t stse_platform_delay_init(void) {
    /* Nothing to initialize on Linux */
    return STSE_OK;
}

void stse_platform_Delay_ms(PLAT_UI32 delay_val) {
    struct timespec ts;
    ts.tv_sec  = delay_val / 1000U;
    ts.tv_nsec = (long)(delay_val % 1000U) * 1000000L;
    nanosleep(&ts, NULL);
}

void stse_platform_timeout_ms_start(PLAT_UI16 timeout_val) {
    clock_gettime(CLOCK_MONOTONIC, &timeout_end);
    timeout_end.tv_sec  += timeout_val / 1000;
    timeout_end.tv_nsec += (long)(timeout_val % 1000) * 1000000L;
    if (timeout_end.tv_nsec >= 1000000000L) {
        timeout_end.tv_sec  += 1;
        timeout_end.tv_nsec -= 1000000000L;
    }
}

PLAT_UI8 stse_platform_timeout_ms_get_status(void) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    if (now.tv_sec > timeout_end.tv_sec ||
        (now.tv_sec == timeout_end.tv_sec && now.tv_nsec >= timeout_end.tv_nsec)) {
        return 1; /* timeout elapsed */
    }
    return 0;
}
