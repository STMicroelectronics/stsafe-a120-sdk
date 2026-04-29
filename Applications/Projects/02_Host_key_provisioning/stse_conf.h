/******************************************************************************
 * \file	stse_conf.h
 * \brief   STSecureElement library configuration file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STSE_CONF_H
#define STSE_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stse_platform_generic.h"

/************************************************************
 *                STSELIB DEVICE SUPPORT
 ************************************************************/
#define STSE_CONF_STSAFE_A_SUPPORT
//#define STSE_CONF_STSAFE_L_SUPPORT

/************************************************************
 *                STSAFE-A API/SERVICE SETTINGS
 ************************************************************/
#ifdef STSE_CONF_STSAFE_A_SUPPORT

/* STSAFE-A ECC services configuration */
//#define STSE_CONF_ECC_NIST_P_256
//#define STSE_CONF_ECC_NIST_P_384
//#define STSE_CONF_ECC_NIST_P_521
//#define STSE_CONF_ECC_BRAINPOOL_P_256
//#define STSE_CONF_ECC_BRAINPOOL_P_384
//#define STSE_CONF_ECC_BRAINPOOL_P_512
//#define STSE_CONF_ECC_CURVE_25519
//#define STSE_CONF_ECC_EDWARD_25519

/* STSAFE-A HASH services configuration */
//#define STSE_CONF_HASH_SHA_1
//#define STSE_CONF_HASH_SHA_224
//#define STSE_CONF_HASH_SHA_256
//#define STSE_CONF_HASH_SHA_384
//#define STSE_CONF_HASH_SHA_512
//#define STSE_CONF_HASH_SHA_3_256
//#define STSE_CONF_HASH_SHA_3_384
//#define STSE_CONF_HASH_SHA_3_512

/* STSAFE-A STATIC PERSONALIZATION INFORMATIONS */
//#define STSE_CONF_USE_STATIC_PERSONALIZATION_INFORMATIONS

/* STSAFE-A HOST KEY MANAGEMENT (DEVICE PAIRING) */
//#define STSE_CONF_USE_HOST_SESSION
//#define STSE_CONF_USE_HOST_KEY_ESTABLISHMENT
//#define STSE_CONF_USE_HOST_KEY_PROVISIONING_WRAPPED
//#define STSE_CONF_USE_HOST_KEY_PROVISIONING_WRAPPED_AUTHENTICATED

/* STSAFE-A SYMMETRIC KEY MANAGEMENT (DEVICE PAIRING) */
//#define STSE_CONF_USE_SYMMETRIC_KEY_ESTABLISHMENT
//#define STSE_CONF_USE_SYMMETRIC_KEY_ESTABLISHMENT_AUTHENTICATED
//#define STSE_CONF_USE_SYMMETRIC_KEY_PROVISIONING_WRAPPED
//#define STSE_CONF_USE_SYMMETRIC_KEY_PROVISIONING_WRAPPED_AUTHENTICATED

#endif /* STSE_CONF_STSAFE_A_SUPPORT */

/************************************************************
 *                STSAFE-L API/SERVICE SETTINGS
 ************************************************************/
#ifdef STSE_CONF_STSAFE_L_SUPPORT

#ifndef STSE_CONF_HASH_SHA_256
#define STSE_CONF_HASH_SHA_256
#endif /*STSE_CONF_HASH_SHA_256*/

#ifndef STSE_CONF_ECC_CURVE_25519
#define STSE_CONF_ECC_CURVE_25519
#endif /*STSE_CONF_ECC_CURVE_25519*/

#ifndef STSE_CONF_ECC_EDWARD_25519
#define STSE_CONF_ECC_EDWARD_25519
#endif /*STSE_CONF_ECC_EDWARD_25519*/

#define STSE_CONF_USE_I2C
#define STSE_CONF_USE_ST1WIRE

#endif /* STSE_CONF_STSAFE_L_SUPPORT */

/*********************************************************
 *                COMMUNICATION SETTINGS
 *********************************************************/

#define STSE_USE_RSP_POLLING
#define STSE_MAX_POLLING_RETRY 100
#define STSE_FIRST_POLLING_INTERVAL 10
#define STSE_POLLING_RETRY_INTERVAL 10
//#define STSE_FRAME_DEBUG_LOG

#ifdef __cplusplus
}
#endif

#endif /* STSE_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
