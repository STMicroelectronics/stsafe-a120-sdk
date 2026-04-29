/**
 ******************************************************************************
 * @file    main.c
 * @author  CS application team
 * @brief   STSAFE-A120 Echo loop example
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

/**
 * @brief  Main program entry point - STSAFE-A120 Echo loop example
 * @details Demonstrates the echo command functionality:
 *          - Generates random message with random length (1-500 bytes)
 *          - Sends message to STSAFE-A120 device via echo command
 *          - Receives and verifies echoed message
 *          - Repeats continuously with 1 second delay
 * @retval Not applicable (infinite loop)
 */
int main(void) {
    stse_ReturnCode_t stse_ret = STSE_API_INVALID_PARAMETER;
    stse_Handler_t stse_handler;
    uint16_t message_length = 0;

    /* Initialize Terminal */
    apps_terminal_init(115200);

    /* Print Example instruction on terminal */
    printf(PRINT_CLEAR_SCREEN);
    printf("----------------------------------------------------------------------------------------------------------------");
    printf("\n\r-                                    STSAFE-A Echo loop example                                                -");
    printf("\n\r----------------------------------------------------------------------------------------------------------------");

    /* Initialize STSAFE-A1xx device handler */
    stse_ret = stse_set_default_handler_value(&stse_handler);
    if (stse_ret != STSE_OK) {
        printf("\n\r ## stse_set_default_handler_value ERROR : 0x%04X\n\r", stse_ret);
        apps_process_error(stse_ret);
    }

    stse_handler.device_type = STSAFE_A120;
    stse_handler.io.busID = 1;
    stse_handler.io.BusSpeed = 400;

    printf("\n\r - Initialize target STSAFE-A120");
    stse_ret = stse_init(&stse_handler);
    if (stse_ret != STSE_OK) {
        printf("\n\r ## stse_init ERROR : 0x%04X\n\r", stse_ret);
        apps_process_error(stse_ret);
    }

    while (1) {
        /* Generate random message length (1..500) */
        message_length = (uint16_t)(apps_generate_random_number() & 0x1FF);
        if ((message_length > 500) || (message_length == 0)) {
            message_length = 1;
        }

        /* Create message and echo buffers (max 500 bytes) */
        uint8_t message[500] = {0};
        uint8_t echoed_message[500] = {0};

        /* Fill message with random content */
        apps_randomize_buffer(message, message_length);

        /* Print message */
        printf("\n\r ## Message :\n\r");
        apps_print_hex_buffer(message, message_length);

        /* Perform echo operation */
        stse_ret = stse_device_echo(&stse_handler, message, echoed_message, message_length);
        if (stse_ret != STSE_OK) {
            printf("\n\r## stse_device_echo ERROR : 0x%04X\n\r", stse_ret);
            apps_process_error(stse_ret);
        }

        /* Compare message and echoed message */
        if (apps_compare_buffers(message, echoed_message, message_length)) {
            printf("\n\n \r ## ECHO MESSAGES COMPARE ERROR (%d)", message_length);
            printf("\n\r\t Echoed Message :\n\r");
            apps_print_hex_buffer(echoed_message, message_length);
        }
        printf("\n\n \r ## Echoed Message :\n\r");
        apps_print_hex_buffer(echoed_message, message_length);

        printf("\n\r\n\r*#*# STMICROELECTRONICS #*#*\n\r");

        /* Wait for 1s */
        apps_delay_ms(1000);
    }
}
