/**
 ******************************************************************************
 * @file    main.c
 * @author  CS application team
 * @brief   STSAFE-A120 Key wrapping example
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
#define BUFFER_SIZE 16

const stsafea_aes_128_host_keys_t host_keys = {

    .host_mac_key = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                     0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF},

    .host_cipher_key = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
                        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF}};

/**
 * @brief  Main program entry point - STSAFE-A120 Key wrapping example
 * @details Demonstrates local envelope wrapping/unwrapping:
 *          - Checks host key presence and session requirements
 *          - Establishes host session if required for secure transfer
 *          - Generates random plaintext secret (16 bytes)
 *          - Wraps the secret using STSAFE-A120 local envelope
 *          - Unwraps the secret using STSAFE-A120
 *          - Compares original and unwrapped secrets to verify operation
 * @note   For production use, enable host session to prevent secret
 *         leakage over the communication bus
 * @retval Not applicable (infinite loop on success or error)
 */
int main(void) {
    stse_ReturnCode_t stse_ret = STSE_API_INVALID_PARAMETER;
    stse_Handler_t stse_handler;
    stse_session_t host_session_handler;

    uint8_t plain_text_secret[BUFFER_SIZE];
    uint8_t wrapped_secret[BUFFER_SIZE + 8] = {0};
    uint8_t unwrapped_secret[BUFFER_SIZE] = {0};
    stse_cmd_access_conditions_t wrap_protection = STSE_CMD_AC_NEVER;
    stse_cmd_access_conditions_t unwrap_protection = STSE_CMD_AC_NEVER;
    PLAT_UI8 wrap_cmd_encryption_flag = 0;
    PLAT_UI8 unwrap_rsp_encryption_flag = 0;

    /* Initialize Terminal */
    apps_terminal_init(115200);

    /* - Print Example instruction on terminal */
    printf("----------------------------------------------------------------------------------------------------------------");
    printf("\n\r-                                    STSAFE-A120 key wrapping Example                                          -");
    printf("\n\r----------------------------------------------------------------------------------------------------------------");
    printf("\n\r- This example illustrates STSAFE-A120 key wrapping process.                                                   -");
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

    /* ## Command config through perso */
    stsafea_perso_info_get_cmd_AC(&stse_handler.perso_info, STSAFEA_CMD_WRAP_LOCAL_ENVELOPE, &wrap_protection);
    stsafea_perso_info_get_cmd_AC(&stse_handler.perso_info, STSAFEA_CMD_UNWRAP_LOCAL_ENVELOPE, &unwrap_protection);
    stsafea_perso_info_get_cmd_encrypt_flag(&stse_handler.perso_info, STSAFEA_CMD_WRAP_LOCAL_ENVELOPE, &wrap_cmd_encryption_flag);
    stsafea_perso_info_get_rsp_encrypt_flag(&stse_handler.perso_info, STSAFEA_CMD_UNWRAP_LOCAL_ENVELOPE, &unwrap_rsp_encryption_flag);

    if (wrap_protection == 0 || unwrap_protection == 0 || wrap_cmd_encryption_flag == 0 || unwrap_rsp_encryption_flag == 0) {
        printf(PRINT_CYAN "\n\n\n\n\n\n\r----------------------------------------------------------------------------------------------------------------");
        printf("\n\r-                                          !!!!! WARNING !!!!!                                                 -");
        printf("\n\r----------------------------------------------------------------------------------------------------------------");
        printf("\n\r-           To avoid any leakage of secret through communication bus during wrap or unwrap process,            -");
        printf("\n\r-                        it's recommended to provision command access condition                                -");
        printf("\n\r----------------------------------------------------------------------------------------------------------------\n\n\r" PRINT_RESET);
    }

    /* ## Provision Wrap/Unwrap keys (for STSAFE-A120 SPL05 only) */
    stse_ret = stsafea_generate_wrap_unwrap_key(&stse_handler, 00, STSE_AES_128_KT);
    if (stse_ret != STSE_OK && stse_ret != STSE_ACCESS_CONDITION_NOT_SATISFIED) {
        printf(PRINT_RED "\n\r - stsafea_generate_wrap_unwrap_key ERROR : 0x%04X", stse_ret);
        apps_process_error(stse_ret);
    } else if (stse_ret != STSE_ACCESS_CONDITION_NOT_SATISFIED) {
        printf("\n\r - new AES-128 wrap/unwrap key generated in slot 0");
    }

    /* ## Provision plain text buffer */
    for (uint16_t k = 0; k < sizeof(plain_text_secret) / sizeof(plain_text_secret[0]); k++) {
        plain_text_secret[k] = (uint8_t)k;
    }

    /* ## Display plain text */
    printf("\n\r - Plain-text buffer to be wrapped  : ");
    apps_print_hex_buffer(plain_text_secret, BUFFER_SIZE);
    printf("\n\r");

    /* ## Open Host Session (required for protecting sensitive exchanges over I2C between host and STSAFE-A) */
	stsafea_session_clear_context(&host_session_handler);

	stse_ret = stsafea_open_host_session(
		&stse_handler,                         /* Target STSafe Handler */
		&host_session_handler,                 /* pointer to session handler */
		(uint8_t *)host_keys.host_mac_key,     /* pointer to Host MAC  key to be used by the session */
		(uint8_t *)host_keys.host_cipher_key); /* pointer to Host cipher key to be used by the session */
	if (stse_ret != STSE_OK) {
		printf(PRINT_RED "\n\r - stse_open_host_session ERROR : 0x%04X", stse_ret);
		if (stse_ret == STSE_SERVICE_SESSION_ERROR) {
			printf(PRINT_RED "\n\r - Host keys not populated through STSE");
		}
		apps_process_error(stse_ret);
	}

    /* ## Wrap payload */
    stse_ret = stsafea_wrap_payload(
        &stse_handler,     /* Target STSafe Handler */
        0,                 /* Wrap key slot : 0 */
        plain_text_secret, /* Pointer to the buffer to be wrapped */
        BUFFER_SIZE,       /* Size of the buffer to be wrapped */
        wrapped_secret,    /* receive pointer for the wrapped result*/
        BUFFER_SIZE + 8);  /* Size of the expected wrapped result */

    if (stse_ret != STSE_OK) {
        printf(PRINT_RED "\n\n\r - stsafea_wrap_payload ERROR : 0x%04X", stse_ret);
        apps_process_error(stse_ret);
    } else {
        printf("\n\n\r - Wrapped buffer  : \n\r");
        apps_print_hex_buffer(wrapped_secret, BUFFER_SIZE + 8);
    }

    /* ## Un-wrap payload */
    stse_ret = stsafea_unwrap_payload(
        &stse_handler,    /* Target STSafe Handler */
        0,                /* Wrap key slot : 0 */
        wrapped_secret,   /* Pointer to wrapped buffer to be unwrapped */
        BUFFER_SIZE + 8,  /* Size of the wrapped buffer */
        unwrapped_secret, /* Receive pointer for the un-wrapped result*/
        BUFFER_SIZE);     /* Size of the expected un-wrapped result */
    if (stse_ret != STSE_OK) {
        printf(PRINT_RED "\n\n\r - stsafea_unwrap_payload ERROR : 0x%04X", stse_ret);
    } else {
        printf("\n\n\r - Un-wrapped buffer  : \n\r");
        apps_print_hex_buffer(unwrapped_secret, BUFFER_SIZE);
    }

    /* ## Close Host Session */
    stsafea_close_host_session(&host_session_handler);

    /* Compare message and echoed message */
    if (apps_compare_buffers(plain_text_secret, unwrapped_secret, BUFFER_SIZE)) {
        printf(PRINT_RED "\n\n \r ## WRAP UNWRAP MESSAGES COMPARE ERROR (%d)", BUFFER_SIZE);
        printf("\n\r\t Unwrapped secret :\n\r");
        apps_print_hex_buffer(unwrapped_secret, BUFFER_SIZE);
        apps_process_error(stse_ret);
    }

    printf(PRINT_RESET "\n\r\n\r*#*# STMICROELECTRONICS #*#*\n\r");

    while (1) {
        // infinite loop
    }

    return 0;
}
