/**
 ******************************************************************************
 * @file    Apps_utils.h
 * @author  CS application team
 * @brief   Application utilities header file providing common helper functions
 *          and definitions for STSAFE-A120 examples
 ******************************************************************************
 *                      COPYRIGHT 2022 STMicroelectronics
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include "Drivers/delay_ms/delay_ms.h"
#include "Drivers/rng/rng.h"
#include "Drivers/uart/uart.h"
#include "stselib.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef APPS_UTILS
#define APPS_UTILS

/* Terminal control definitions -------------------------------------------*/

/** @defgroup Terminal_Control_Sequences Terminal Control Sequences
 * @brief ANSI escape sequences for terminal formatting
 * @{
 */
#define PRINT_CLEAR_SCREEN "\x1B[1;1H\x1B[2J"  /**< Clear screen and move cursor to home */
#define PRINT_BLINK        "\x1B[5m"            /**< Enable blinking text */
#define PRINT_UNDERLINE    "\x1B[4m"            /**< Enable underlined text */
#define PRINT_BELL         "\a"                 /**< Sound terminal bell */
#define PRINT_BOLD         "\x1B[1m"            /**< Enable bold text */
#define PRINT_ITALIC       "\x1B[3m"            /**< Enable italic text */
#define PRINT_RESET        "\x1B[0m"            /**< Reset all text attributes */
/** @defgroup Terminal_Colors Terminal Color Codes
 * @brief ANSI color codes for terminal text
 * @{
 */
#ifdef DISABLE_PRINT_COLOR
#define PRINT_BLACK   ""   /**< Black text (disabled) */
#define PRINT_RED     ""   /**< Red text (disabled) */
#define PRINT_GREEN   ""   /**< Green text (disabled) */
#define PRINT_YELLOW  ""   /**< Yellow text (disabled) */
#define PRINT_BLUE    ""   /**< Blue text (disabled) */
#define PRINT_MAGENTA ""   /**< Magenta text (disabled) */
#define PRINT_CYAN    ""   /**< Cyan text (disabled) */
#define PRINT_WHITE   ""   /**< White text (disabled) */
#else
#define PRINT_BLACK   "\x1B[30m"   /**< Black text */
#define PRINT_RED     "\x1B[31m"   /**< Red text */
#define PRINT_GREEN   "\x1B[32m"   /**< Green text */
#define PRINT_YELLOW  "\x1B[33m"   /**< Yellow text */
#define PRINT_BLUE    "\x1B[34m"   /**< Blue text */
#define PRINT_MAGENTA "\x1B[35m"   /**< Magenta text */
#define PRINT_CYAN    "\x1B[36m"   /**< Cyan text */
#define PRINT_WHITE   "\x1B[37m"   /**< White text */
#endif
/**
 * @}
 */

/* Function declarations --------------------------------------------------*/

/** @defgroup Terminal_Functions Terminal Functions
 * @brief Functions for terminal I/O operations
 * @{
 */

/**
 * @brief  Initialize terminal for application output
 * @param  baudrate Baudrate for UART communication
 */
void apps_terminal_init(uint32_t baudrate);

/**
 * @brief  Read string from terminal
 * @param  string Pointer to buffer to store the read string
 * @param  length Pointer to maximum/actual length
 * @return 0 on success, 1 on error
 */
uint8_t apps_terminal_read_string(char *string, uint8_t *length);

/**
 * @brief  Read unsigned integer from terminal
 * @param  integer Pointer to store the read integer value
 * @return 0 on success, 1 on error
 */
uint8_t apps_terminal_read_unsigned_integer(uint16_t *integer);

/**
 * @}
 */

/** @defgroup Print_Functions Print Functions
 * @brief Functions for printing formatted STSAFE-A120 information
 * @{
 */

/**
 * @brief  Print data partition record table information
 * @param  pSTSE Pointer to STSE handler structure
 */
void apps_print_data_partition_record_table(stse_Handler_t *pSTSE);

/**
 * @brief  Print formatted symmetric key table information
 * @param  pSTSE Pointer to STSE handler structure
 */
void apps_print_symmetric_key_table_info(stse_Handler_t *pSTSE);

/**
 * @brief  Print symmetric key table provisioning control fields
 * @param  pSTSE Pointer to STSE handler structure
 */
void apps_print_symmetric_key_table_provisioning_control_fields(stse_Handler_t *pSTSE);

/**
 * @brief  Print formatted asymmetric (ECC) key table information
 * @param  pSTSE Pointer to STSE handler structure
 */
void apps_print_asymmetric_key_table_info(stse_Handler_t *pSTSE);

/**
 * @brief  Print host key provisioning control fields
 * @param  pSTSE Pointer to STSE handler structure
 */
void apps_print_host_key_provisioning_control_fields(stse_Handler_t *pSTSE);

/**
 * @brief  Print generic public key slot configuration flags
 * @param  pSTSE Pointer to STSE handler structure
 * @param  slot_number Slot number to query
 */
void apps_print_generic_public_key_slot_configuration_flags(stse_Handler_t *pSTSE, PLAT_UI8 slot_number);

/**
 * @brief  Print buffer content in hexadecimal format
 * @param  buffer Pointer to buffer to display
 * @param  buffer_size Size of the buffer in bytes
 */
void apps_print_hex_buffer(uint8_t *buffer, uint16_t buffer_size);

/**
 * @brief  Print command authorization record table
 * @param  command_ac_record_table Pointer to array of command authorization records
 * @param  total_command_count Number of commands in the table
 */
void apps_print_command_ac_record_table(stse_cmd_authorization_record_t *command_ac_record_table,
                                        uint8_t total_command_count);

/**
 * @brief  Print human-readable life cycle state
 * @param  life_cycle_state The life cycle state enumeration value
 */
void apps_print_life_cycle_state(stsafea_life_cycle_state_t life_cycle_state);

/**
 * @}
 */

/** @defgroup Utility_Functions Utility Functions
 * @brief Miscellaneous utility functions
 * @{
 */

/**
 * @brief  Generate a random 32-bit number
 * @return Random 32-bit unsigned integer
 */
uint32_t apps_generate_random_number(void);

/**
 * @brief  Fill buffer with random data
 * @param  pBuffer Pointer to buffer to fill
 * @param  buffer_length Length of the buffer in bytes
 */
void apps_randomize_buffer(uint8_t *pBuffer, uint16_t buffer_length);

/**
 * @brief  Compare two buffers byte by byte
 * @param  pBuffer1 Pointer to first buffer
 * @param  pBuffer2 Pointer to second buffer
 * @param  buffers_length Length of buffers to compare in bytes
 * @return 0 if buffers are identical, 1 if any byte differs
 */
uint8_t apps_compare_buffers(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t buffers_length);

/**
 * @brief  Delay execution for specified milliseconds
 * @param  ms Number of milliseconds to delay
 */
void apps_delay_ms(uint16_t ms);

/**
 * @brief  Process error by entering infinite loop
 * @param  err Error code
 */
void apps_process_error(uint32_t err);

/**
 * @}
 */

#endif /*APPS_UTILS*/
