/******************************************************************************
 * \file    rng.h
 * \brief   Random number generator inline helpers for Linux (STM32MP1)
 *          Uses getrandom() syscall directly — no driver compilation unit
 *          required.
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

#ifndef RNG_H
#define RNG_H

#include <fcntl.h>
#include <stdint.h>
#include <sys/random.h>
#include <unistd.h>

static inline void rng_start(void) {
    /* Nothing to start on Linux — kernel RNG is always available */
}

static inline uint32_t rng_generate_random_number(void) {
    uint32_t value = 0;
    if (getrandom(&value, sizeof(value), 0) != (ssize_t)sizeof(value)) {
        int fd = open("/dev/urandom", O_RDONLY);
        if (fd >= 0) {
            if (read(fd, &value, sizeof(value)) != (ssize_t)sizeof(value)) {
                value = 0;
            }
            close(fd);
        }
    }
    return value;
}

static inline void rng_stop(void) {
    /* Nothing to stop on Linux */
}

#endif /* RNG_H */
