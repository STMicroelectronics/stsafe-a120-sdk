/******************************************************************************
 * \file    uart.h
 * \brief   UART inline helpers for Linux (STM32MP1)
 *          Redirects to stdio putchar/getchar — no driver compilation unit
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

#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdio.h>

static inline void uart_init(uint32_t baudrate) {
    (void)baudrate;
    /* On Linux the terminal is already open; nothing to configure */
}

static inline void uart_putc(uint8_t ch) {
    putchar((int)ch);
}

static inline uint8_t uart_getc(void) {
    int c = getchar();
    return (c == EOF) ? 0u : (uint8_t)c;
}

#endif /* UART_H */
