/******************************************************************************
 * \file	stse_platform_crypto.c
 * \brief   STSecureElement cryptographic platform file
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

#include "Drivers/delay_ms/delay_ms.h"
#include "Drivers/delay_us/delay_us.h"
#include "stse_conf.h"
#include "stselib.h"

stse_ReturnCode_t stse_platform_delay_init(void) {
    /* Initialize platform Drivers used by PAL */
    delay_ms_init();

    return STSE_OK;
}

void stse_platform_Delay_ms(PLAT_UI16 delay_val) {
    delay_ms(delay_val);
}

void stse_platform_timeout_ms_start(PLAT_UI16 timeout_val) {
    timeout_ms_start(timeout_val);
}

PLAT_UI8 stse_platform_timeout_ms_get_status(void) {
    return timeout_ms_get_status();
}
