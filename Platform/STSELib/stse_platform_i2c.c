/******************************************************************************
 * \file    stse_platform_i2c.c
 * \brief   STSecureElement I2C platform for Linux (STM32MP1)
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
#include <errno.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define I2C_BUS_MAX 16

/* Open file descriptors, one per bus, indexed by busID */
static int i2c_fds[I2C_BUS_MAX];

/* Internal frame assembly buffer (A120 max input + 2 response length + 1 header) */
static PLAT_UI8  I2c_buffer[755U];
static PLAT_UI16 i2c_frame_size;
static PLAT_UI16 i2c_frame_offset;

stse_ReturnCode_t stse_platform_i2c_init(PLAT_UI8 busID) {
    char dev_path[32];

    if (busID >= I2C_BUS_MAX) {
        return STSE_PLATFORM_BUS_ACK_ERROR;
    }

    /* Close any previously opened descriptor on this bus */
    if (i2c_fds[busID] > 0) {
        close(i2c_fds[busID]);
    }

    snprintf(dev_path, sizeof(dev_path), "/dev/i2c-%u", (unsigned int)busID);
    i2c_fds[busID] = open(dev_path, O_RDWR);
    if (i2c_fds[busID] < 0) {
        fprintf(stderr, "stse_platform_i2c_init: failed to open %s: %s\n",
                dev_path, strerror(errno));
        return STSE_PLATFORM_BUS_ACK_ERROR;
    }

    return STSE_OK;
}

stse_ReturnCode_t stse_platform_i2c_wake(PLAT_UI8 busID,
                                         PLAT_UI8 devAddr,
                                         PLAT_UI16 speed) {
    struct i2c_msg           msg  = {0};
    struct i2c_rdwr_ioctl_data xfer = {0};

    (void)speed;

    if (busID >= I2C_BUS_MAX || i2c_fds[busID] < 0) {
        return STSE_PLATFORM_BUS_ACK_ERROR;
    }

    /* Zero-length write to generate a start/stop – NACK expected, so ignore result */
    msg.addr  = devAddr;
    msg.flags = 0;
    msg.len   = 0;
    msg.buf   = NULL;

    xfer.msgs  = &msg;
    xfer.nmsgs = 1;
    ioctl(i2c_fds[busID], I2C_RDWR, &xfer);

    return STSE_OK;
}

stse_ReturnCode_t stse_platform_i2c_send_start(
    PLAT_UI8  busID,
    PLAT_UI8  devAddr,
    PLAT_UI16 speed,
    PLAT_UI16 FrameLength) {
    (void)busID;
    (void)devAddr;
    (void)speed;

    if (FrameLength > sizeof(I2c_buffer)) {
        return STSE_PLATFORM_BUFFER_ERR;
    }

    i2c_frame_size   = FrameLength;
    i2c_frame_offset = 0;

    return STSE_OK;
}

stse_ReturnCode_t stse_platform_i2c_send_continue(
    PLAT_UI8  busID,
    PLAT_UI8  devAddr,
    PLAT_UI16 speed,
    PLAT_UI8 *pData,
    PLAT_UI16 data_size) {
    (void)busID;
    (void)devAddr;
    (void)speed;

    if (data_size != 0) {
        if (pData == NULL) {
            memset(I2c_buffer + i2c_frame_offset, 0x00, data_size);
        } else {
            memcpy(I2c_buffer + i2c_frame_offset, pData, data_size);
        }
        i2c_frame_offset += data_size;
    }

    return STSE_OK;
}

stse_ReturnCode_t stse_platform_i2c_send_stop(
    PLAT_UI8  busID,
    PLAT_UI8  devAddr,
    PLAT_UI16 speed,
    PLAT_UI8 *pData,
    PLAT_UI16 data_size) {
    stse_ReturnCode_t        ret  = STSE_OK;
    struct i2c_msg           msg  = {0};
    struct i2c_rdwr_ioctl_data xfer = {0};

    ret = stse_platform_i2c_send_continue(busID, devAddr, speed, pData, data_size);
    if (ret != STSE_OK) {
        return STSE_PLATFORM_BUS_ACK_ERROR;
    }

    if (busID >= I2C_BUS_MAX || i2c_fds[busID] < 0) {
        return STSE_PLATFORM_BUS_ACK_ERROR;
    }

    msg.addr  = devAddr;
    msg.flags = 0;          /* write */
    msg.len   = i2c_frame_size;
    msg.buf   = I2c_buffer;

    xfer.msgs  = &msg;
    xfer.nmsgs = 1;

    if (ioctl(i2c_fds[busID], I2C_RDWR, &xfer) < 0) {
        return STSE_PLATFORM_BUS_ACK_ERROR;
    }

    return STSE_OK;
}

stse_ReturnCode_t stse_platform_i2c_receive_start(
    PLAT_UI8  busID,
    PLAT_UI8  devAddr,
    PLAT_UI16 speed,
    PLAT_UI16 frameLength) {
    struct i2c_msg           msg  = {0};
    struct i2c_rdwr_ioctl_data xfer = {0};

    (void)speed;

    if (busID >= I2C_BUS_MAX || i2c_fds[busID] < 0) {
        return STSE_PLATFORM_BUS_ACK_ERROR;
    }

    i2c_frame_size = frameLength;

    msg.addr  = devAddr;
    msg.flags = I2C_M_RD;  /* read */
    msg.len   = frameLength;
    msg.buf   = I2c_buffer;

    xfer.msgs  = &msg;
    xfer.nmsgs = 1;

    if (ioctl(i2c_fds[busID], I2C_RDWR, &xfer) < 0) {
        return STSE_PLATFORM_BUS_ACK_ERROR;
    }

    i2c_frame_offset = 0;

    return STSE_OK;
}

stse_ReturnCode_t stse_platform_i2c_receive_continue(
    PLAT_UI8  busID,
    PLAT_UI8  devAddr,
    PLAT_UI16 speed,
    PLAT_UI8 *pData,
    PLAT_UI16 data_size) {
    (void)busID;
    (void)devAddr;
    (void)speed;

    if (pData != NULL) {
        if ((i2c_frame_size - i2c_frame_offset) < data_size) {
            return STSE_PLATFORM_BUFFER_ERR;
        }
        memcpy(pData, I2c_buffer + i2c_frame_offset, data_size);
    }

    i2c_frame_offset += data_size;

    return STSE_OK;
}

stse_ReturnCode_t stse_platform_i2c_receive_stop(
    PLAT_UI8  busID,
    PLAT_UI8  devAddr,
    PLAT_UI16 speed,
    PLAT_UI8 *pData,
    PLAT_UI16 data_size) {
    stse_ReturnCode_t ret;

    ret              = stse_platform_i2c_receive_continue(busID, devAddr, speed, pData, data_size);
    i2c_frame_offset = 0;

    return ret;
}

