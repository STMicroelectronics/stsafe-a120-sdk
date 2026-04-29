/**
 ******************************************************************************
 * @file    main.c
 * @author  CS application team
 * @brief   STSAFE-A120 Random number generation example
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
 * @brief  Main program entry point - STSAFE-A120 Random number generation example
 * @details Demonstrates random number generation:
 *          - Initializes STSAFE-A120 device
 *          - Generates 64 bytes of random data using STSAFE-A120 TRNG
 *          - Displays the generated random bytes in hexadecimal format
 * @retval Not applicable (infinite loop on success or error)
 */
int main(void) {
    stse_ReturnCode_t stse_ret = STSE_API_INVALID_PARAMETER;
    stse_Handler_t stse_handler;

    PLAT_UI16 random_size = 64;
    PLAT_UI8 pRandom[random_size];

    /* Initialize Terminal */
    apps_terminal_init(115200);

    /* - Print Example instruction on terminal */
    printf("----------------------------------------------------------------------------------------------------------------");
    printf("\n\r-                                  STSAFE-A120 Random number generation example                                -");
    printf("\n\r----------------------------------------------------------------------------------------------------------------");
    printf("\n\r-                                                                                                              -");
    printf("\n\r- description :                                                                                                -");
    printf("\n\r- This examples illustrates how to use generate random commands of a target STSAFE-A120 device                 -");
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

    /* Call STSE "Generate random" API */
    stse_ret = stse_generate_random(&stse_handler, pRandom, random_size);

    if (stse_ret != STSE_OK) {
        printf("\n\n\r - stse_generate_random ERROR : 0x%04X", stse_ret);
    } else {
        printf("\n\n\r - stse_generate_random (%d bytes) : \n\r", random_size);
        apps_print_hex_buffer(pRandom, random_size);
    }

    while (1) {
        /* - Infinite loop */
    }
}
