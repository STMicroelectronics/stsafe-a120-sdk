/******************************************************************************
 * \file    stse_platform_crypto_init.c
 * \brief   STSecureElement cryptographic platform init for Linux (STM32MP1)
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
#include <openssl/crypto.h>

stse_ReturnCode_t stse_platform_crypto_init(void) {
    /*
     * OpenSSL is self-initializing since version 1.1.0.
     * No explicit initialization call is required.
     */
    return STSE_OK;
}
