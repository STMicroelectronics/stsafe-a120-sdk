/**
 ******************************************************************************
 * @file    main.c
 * @author  CS application team
 * @brief   STSAFE-A120 Ed25519 Key pair & signature generation example
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
#define HASH_SIZE(x) (x - x % 16)  /**< Macro to align hash size to 16-byte boundary */

/**
 * @brief  Main program entry point - STSAFE-A120 Edwards-25519 key pair generation
 * @details Demonstrates EdDSA key pair generation and signature:
 *          - Queries asymmetric key table to find available slot
 *          - Generates Edwards-25519 (Ed25519) key pair in selected slot
 *          - Retrieves the generated public key
 *          - Creates a random message and computes SHA-512 hash
 *          - Generates EdDSA signature using the private key
 *          - Verifies the signature using platform crypto library
 * @note   Ed25519 is a modern signature scheme offering high security
 *         and performance with small key and signature sizes
 * @retval Not applicable (infinite loop on success or error)
 */
int main(void) {
    stse_ReturnCode_t stse_ret = STSE_API_INVALID_PARAMETER;
    stse_Handler_t stse_handler;

    /* Initialize Terminal */
    apps_terminal_init(115200);

    /* - Print Example instruction on terminal */
    printf("----------------------------------------------------------------------------------------------------------------");
    printf("\n\r-                              STSAFE-A120 EDWARDS-22519 key pair generation                                       -");
    printf("\n\r----------------------------------------------------------------------------------------------------------------");
    printf("\n\r- This example demonstrates how to generate and use a EDWARDS-25519 key pair on a target STSAFE-A120 device         -");
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

    /* ## Generate key pair in slot 1 */
    uint8_t slot_1_public_key[stse_ecc_info_table[STSE_ECC_KT_ED25519].public_key_size];
    stse_ret = stse_generate_ecc_key_pair(
        &stse_handler,
        1,
        STSE_ECC_KT_ED25519,
        255,
        slot_1_public_key);

    if (stse_ret != STSE_OK) {
        printf(PRINT_RED "\n\n\r ## stse_generate_ecc_key_pair : 0x%04X", stse_ret);
        apps_process_error(stse_ret);
    }

    printf("\n\n\r - STSAFE-A120 generate ECC key pair : success");
    printf("\n\r\t o public key : ");
    apps_print_hex_buffer(slot_1_public_key, sizeof(slot_1_public_key));

    /* ## Generate hash to be signed */
    uint8_t hash[HASH_SIZE(stse_ecc_info_table[STSE_ECC_KT_ED25519].min_signature_message_size)];
    printf("\n\n\r - Plain-text hash :");
    apps_randomize_buffer(hash, sizeof(hash));
    apps_print_hex_buffer(hash, sizeof(hash));

    /* ## Generate signature over hash using target STSAFE-A120 */
    uint8_t signature[stse_ecc_info_table[STSE_ECC_KT_ED25519].signature_size];
    stse_ret = stse_ecc_generate_signature(
        &stse_handler,
        1,
        STSE_ECC_KT_ED25519,
        hash,
        sizeof(hash),
        signature);

    if (stse_ret != STSE_OK) {
        printf(PRINT_RED "\n\n\r ## stse_ecc_generate_signature : 0x%04X", stse_ret);
        apps_process_error(stse_ret);
    }

    printf("\n\n\r - STSAFE-A120 generate signature : success");
    printf("\n\r\t o signature : ");
    apps_print_hex_buffer(signature, sizeof(signature));

    /* ## Verify signature using target STSAFE-A120 */
    uint8_t verification_status;
    stse_ret = stse_ecc_verify_signature(
        &stse_handler,
        STSE_ECC_KT_ED25519,
        slot_1_public_key,
        signature,
        hash,
        sizeof(hash),
        0,
        &verification_status);

    if (stse_ret != STSE_OK) {
        printf(PRINT_RED "\n\n\r ## stse_ecc_verify_signature : 0x%04X", stse_ret);
        apps_process_error(stse_ret);
    }
    if (verification_status != 1) {
        printf(PRINT_RED "\n\n\r ## STSAFE-A120 verify signature : failed");
    } else {
        printf(PRINT_GREEN "\n\n\r - STSAFE-A120 verify signature : success");
    }

    while (1) {
        // infinite loop
    }

    return 0;
}
