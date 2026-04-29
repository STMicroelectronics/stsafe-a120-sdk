/******************************************************************************
 * \file    stse_platform_power.c
 * \brief   STSecureElement power platform for Linux (STM32MP1)
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

stse_ReturnCode_t stse_platform_power_init(void) {
    /*
     * On Linux (STM32MP1), power management of the STSAFE device is handled
     * by the hardware and device tree configuration. No GPIO manipulation is
     * needed from userspace for the standard X-NUCLEO-ESE01A1 configuration.
     */
    return STSE_OK;
}

stse_ReturnCode_t stse_platform_power_on(PLAT_UI8 bus, PLAT_UI8 devAddr) {
    (void)bus;
    (void)devAddr;
    /* Power is managed by hardware on STM32MP1 */
    return STSE_OK;
}

stse_ReturnCode_t stse_platform_power_off(PLAT_UI8 bus, PLAT_UI8 devAddr) {
    (void)bus;
    (void)devAddr;
    /* Power is managed by hardware on STM32MP1 */
    return STSE_OK;
}
