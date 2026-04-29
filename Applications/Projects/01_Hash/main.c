/**
 ******************************************************************************
 * @file    main.c
 * @author  CS application team
 * @brief   STSAFE-A120 Hash example
 ******************************************************************************
 *           			COPYRIGHT 2022 STMicroelectronics
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include "Apps_utils.h"

/* Defines -------------------------------------------------------------------*/

/**
 * @brief  Main program entry point - STSAFE-A120 Hash example
 * @details Demonstrates hash computation functionality:
 *          - Generates a random 128-byte message
 *          - Computes SHA-256 hash using platform crypto library
 *          - Computes SHA-256 hash using STSAFE-A120 device
 *          - Compares both hash values to verify correctness
 * @retval Not applicable (infinite loop on success or error)
 */
int main(void) {
    stse_ReturnCode_t stse_ret = STSE_API_INVALID_PARAMETER;
    stse_Handler_t stse_handler;

    PLAT_UI16 message_length = 128;
    PLAT_UI8 message_to_hash[message_length];

    stse_hash_algorithm_t hash_algo = STSE_SHA_256;
    PLAT_UI16 hash_length = stsafea_hash_info_table[hash_algo].length;
    PLAT_UI8 hash_platform[hash_length];
    PLAT_UI8 hash_stsafea[hash_length];

    /* Initialize Terminal */
    apps_terminal_init(115200);

    /* - Print Example instruction on terminal */
    printf("----------------------------------------------------------------------------------------------------------------");
    printf("\n\r-                                       STSAFE-A120 hash commands example                                      -");
    printf("\n\r----------------------------------------------------------------------------------------------------------------");
    printf("\n\r-                                                                                                              -");
    printf("\n\r- description :                                                                                                -");
    printf("\n\r- This examples illustrates how to use hash commands of a target STSAFE-A120 device                            -");
    printf("\n\r-                                                                                                              -");
    printf("\n\r----------------------------------------------------------------------------------------------------------------");

    /* ## Initialize STSAFE-A1xx device handler */
    stse_ret = stse_set_default_handler_value(&stse_handler);
    if (stse_ret != STSE_OK) {
        printf(PRINT_RED "\n\r ## stse_set_default_handler_value ERROR : 0x%04X\n\r", stse_ret);
        apps_process_error(stse_ret);
    }

    stse_handler.device_type = STSAFE_A120;
    stse_handler.io.busID = 1;
    stse_handler.io.BusSpeed = 400;

    printf("\n\r - Initialize target STSAFE-A120");
    stse_ret = stse_init(&stse_handler);
    if (stse_ret != STSE_OK) {
        printf(PRINT_RED "\n\r ## stse_init ERROR : 0x%04X\n\r", stse_ret);
        apps_process_error(stse_ret);
    }

    /* - Generate & Display random message */
    printf("\n\n\r - Message buffer to hash : \n\r");
    apps_randomize_buffer(message_to_hash, message_length);
    apps_print_hex_buffer(message_to_hash, message_length);

    /* - Hash message using platform */
    stse_ret = stse_platform_hash_compute(
        hash_algo,
        message_to_hash,
        message_length,
        hash_platform,
        &hash_length);

    if (stse_ret != STSE_OK) {
        printf(PRINT_RED "\n\n\r - stse_platform_hash_compute ERROR : 0x%04X", stse_ret);
    } else {
        printf("\n\n\r - stse_platform_hash_compute  : \n\r");
        apps_print_hex_buffer(hash_platform, hash_length);
    }

    /* - Hash message using STSAFE-A120 */
    stse_ret = stse_compute_hash(
        &stse_handler,
        hash_algo,
        message_to_hash,
        message_length,
        hash_stsafea,
        &hash_length);

    if (stse_ret != STSE_OK) {
        printf(PRINT_RED "\n\n\r - stse_compute_hash ERROR : 0x%04X", stse_ret);
    } else {
        printf("\n\n\r - stse_compute_hash  : \n\r");
        apps_print_hex_buffer(hash_stsafea, hash_length);
    }

    /* - Check STSAFE-A hash against platform hash */
    if (memcmp(hash_platform, hash_stsafea, hash_length)) {
        printf(PRINT_RED "\n\n\r - memcmp ERROR");
    } else {
        printf(PRINT_GREEN "\n\n\r - HASH SUCCESS\n\r");
    }

    while (1) {
        /* - Infinite loop */
    }
}
