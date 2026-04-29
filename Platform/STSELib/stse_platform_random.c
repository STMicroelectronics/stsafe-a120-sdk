/******************************************************************************
 * \file    stse_platform_random.c
 * \brief   STSecureElement random number generator platform for Linux (STM32MP1)
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
#include <fcntl.h>
#include <stdio.h>
#include <sys/random.h>
#include <unistd.h>

stse_ReturnCode_t stse_platform_generate_random_init(void) {
    /* Nothing to start on Linux - kernel RNG is always available */
    return STSE_OK;
}

PLAT_UI32 stse_platform_generate_random(void) {
    PLAT_UI32 value = 0;
    ssize_t   ret;

    ret = getrandom(&value, sizeof(value), 0);
    if (ret != (ssize_t)sizeof(value)) {
        /* Fallback: read from /dev/urandom */
        int fd = open("/dev/urandom", O_RDONLY);
        if (fd >= 0) {
            if (read(fd, &value, sizeof(value)) != (ssize_t)sizeof(value)) {
                value = 0;
                fprintf(stderr, "stse_platform_generate_random: /dev/urandom read failed\n");
            }
            close(fd);
        } else {
            fprintf(stderr, "stse_platform_generate_random: failed to open /dev/urandom\n");
        }
    }

    return value;
}
