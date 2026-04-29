/**
 ******************************************************************************
 * @file    main.c
 * @author  CS application team
 * @brief   STSAFE-A120 Secure data storage with counter access example
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
#define READ_BUFFER_SIZE 16  /**< Size of read buffer for zone data */
#define RANDOM_SIZE 16       /**< Size of random data to write */

/**
 * @brief  Main program entry point - STSAFE-A120 Secure data storage with counter
 * @details Demonstrates data partition with monotonic counter operations:
 *          - Queries partition configuration and displays zone information
 *          - Reads data from zone with counter (zone 0)
 *          - Reads and displays current counter value
 *          - Generates random data and updates zone
 *          - Verifies counter increment after update
 *          - Demonstrates decrement-only counter functionality
 * @note   Zone IDs are aligned with STSAFE-A120 SPL05 personalization.
 *         Counters provide tamper detection by tracking update operations.
 * @retval Not applicable (infinite loop on success or error)
 */
int main(void) {
    stse_ReturnCode_t stse_ret = STSE_API_INVALID_PARAMETER;
    stse_Handler_t stse_handler;

    uint8_t readBuffer[READ_BUFFER_SIZE];
    uint8_t random[RANDOM_SIZE];
    uint32_t counter_value;
    uint32_t new_counter_value;

    /* Initialize Terminal */
    apps_terminal_init(115200);

    /* - Print Example instruction on terminal */
    printf("----------------------------------------------------------------------------------------------------------------");
    printf("\n\r-                            STSAFE-A120 secure data storage counter access example                            -");
    printf("\n\r----------------------------------------------------------------------------------------------------------------");
    printf("\n\r-                                                                                                              -");
    printf("\n\r- description :                                                                                                -");
    printf("\n\r- This examples illustrates how to makes use of the STSAFE-A data storage APIs by performing following         -");
    printf("\n\r- accesses/commands to the target STSAFE device                                                                -");
    printf("\n\r-          o Query STSAFE-A total partition count                                                              -");
    printf("\n\r-          o Query STSAFE-A partitions information                                                             -");
    printf("\n\r-          o Read STSAFE-A zone 5 counter                                                                      -");
    printf("\n\r-          o decrement STSAFE-A zone 5 counter                                                                 -");
    printf("\n\r-          o Update STSAFE-A counter zone 5                                                                    -");
    printf("\n\r-                                                                                                              -");
    printf("\n\r- Note : zone IDs used in this example are aligned with STSAFE-A120 SPL05 personalization                      -");
    printf("\n\r-        Accesses parameters must be adapted for other device personalization                                  -");
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

    /* ## Get and print device Partition table */
    apps_print_data_partition_record_table(&stse_handler);

    /* ## Read zone 5 (counter zone) */
    stse_ret = stse_data_storage_read_counter_zone(
        &stse_handler,      /* SE handler		*/
        5,                  /* Zone index		*/
        0x0000,             /* Read Offset		*/
        readBuffer,         /* Read buffer		*/
        sizeof(readBuffer), /* Read length		*/
        04,                 /* Read chunk size	*/
        &counter_value,     /* Counter Value	*/
        STSE_NO_PROT);
    if (stse_ret != STSE_OK) {
        printf(PRINT_RED "\n\n\r ### stse_data_storage_read_counter_zone : ERROR 0x%04X", stse_ret);
        apps_process_error(stse_ret);
    } else {
        printf("\n\n\r - stse_data_storage_read_counter_zone ( zone : 05 - length : %d)", sizeof(readBuffer) / sizeof(readBuffer[0]));
        printf("\n\r\t o Associated Data : ");
        apps_print_hex_buffer(readBuffer, sizeof(readBuffer));
        printf("\n\r\t o Counter Value : %u", counter_value);
    }

    /* ## Decrement zone 5 counter and store Randomized Associated data */
    apps_randomize_buffer(random, sizeof(random));
    stse_ret = stse_data_storage_decrement_counter_zone(
        &stse_handler,      /* SE handler 			*/
        5,                  /* Zone index 			*/
        1,                  /* Amount				*/
        0x0000,             /* Update Offset 		*/
        random,             /* Update input buffer 	*/
        sizeof(random),     /* Update Length 		*/
        &new_counter_value, /* New counter value 	*/
        STSE_NO_PROT);
    if (stse_ret != STSE_OK) {
        printf(PRINT_RED "\n\n\r ### stse_data_storage_decrement_counter_zone : ERROR 0x%04X", stse_ret);
        apps_process_error(stse_ret);
    } else {
        printf("\n\n\r - stse_data_storage_decrement_counter_zone ( zone : 05 - length : %d)", sizeof(random) / sizeof(random[0]));
        printf("\n\r\t o New Associated Data : ");
        apps_print_hex_buffer(random, sizeof(random));
        printf("\n\r\t o New Counter Value : %u", new_counter_value);
    }

    /* ## Read zone 5 (counter zone) */
    stse_ret = stse_data_storage_read_counter_zone(
        &stse_handler,      /* SE handler		*/
        5,                  /* Zone index		*/
        0x0000,             /* Read Offset		*/
        readBuffer,         /* Read buffer		*/
        sizeof(readBuffer), /* Read length		*/
        04,                 /* Read chunk size	*/
        &counter_value,     /* Counter Value	*/
        STSE_NO_PROT);
    if (stse_ret != STSE_OK) {
        printf(PRINT_RED "\n\n\r ### stse_data_storage_read_counter_zone : ERROR 0x%04X", stse_ret);
        apps_process_error(stse_ret);
    } else {
        printf("\n\n\r - stse_data_storage_read_counter_zone ( zone : 05 - length : %d)", sizeof(readBuffer) / sizeof(readBuffer[0]));
        printf("\n\r\t o Associated Data : ");
        apps_print_hex_buffer(readBuffer, sizeof(readBuffer));
        printf("\n\r\t o Counter Value : %u", counter_value);
    }

    while (1) {
        // infinite loop
    }

    return 0;
}
