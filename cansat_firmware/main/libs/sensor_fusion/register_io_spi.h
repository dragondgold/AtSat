/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file register_io_spi.h
 * @brief The register_io_spi.h file declares low-level interface functions for reading
 * and writing sensor registers using CMSIS APIs.
 */

#ifndef __REGISTER_IO_SPI_H__
#define __REGISTER_IO_SPI_H__

#include "sensor_drv.h"

#define SPI_SS_ACTIVE_LOW (ARM_SPI_SS_INACTIVE)
#define SPI_SS_ACTIVE_HIGH (ARM_SPI_SS_ACTIVE)

/*! Function pointer for the slave read information*/
typedef void (*fpSpiReadPreprocessFn_t)(void *pCmdOut, uint32_t offset, uint32_t size);
/*! Function pointer for the slave write information*/
typedef void (*fpSpiWritePreprocessFn_t)(void *pCmdOut, uint32_t offset, uint32_t size, void *pWritebuffer);

/*! @brief The SPI Slave Control Command Params SDK2.0 Driver. */
typedef struct
{
    void *pTargetSlavePinID;
    uint8_t activeValue;
    uint8_t cmdCode;
} spiControlParams_t;

/*! @brief The SPI Slave Transfer Command Params SDK2.0 Driver. */
typedef struct spi_mater_SlaveCmd
{
    uint32_t size;         /*!< The tranfer size.*/
    uint8_t *pReadBuffer;  /*!< The handle the readbuffer.*/
    uint8_t *pWriteBuffer; /*!< The handle the writecommand.*/
} spiCmdParams_t;

/*! @brief This structure defines the spi slave command format. */
typedef struct
{
    fpSpiReadPreprocessFn_t pReadPreprocessFN;
    fpSpiWritePreprocessFn_t pWritePreprocessFN;
    void *pTargetSlavePinID;
    uint8_t spiCmdLen;
    uint8_t ssActiveValue;
} spiSlaveSpecificParams_t;

#if defined(SPI0)
/*! @brief The SPI0 device index. */
#define SPI0_INDEX 0

/*! @brief       Defines the SPI signal event handler.
 *  @details     The Signal Event Handler function required by register_io_i2c.c.
 *  @param[in]   event The SPI event that occured.
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      void
 */
void SPI0_SignalEvent_t(uint32_t event);
#endif

#if defined(SPI1)
/*! @brief The SPI1 device index. */
#define SPI1_INDEX 1

/*! @brief       Defines the SPI signal event handler.
 *  @details     The Signal Event Handler function required by register_io_i2c.c.
 *  @param[in]   event The SPI event that occured.
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      void
 */
void SPI1_SignalEvent_t(uint32_t event);
#endif

#if defined(SPI2)
/*! @brief The SPI2 device index. */
#define SPI2_INDEX 2

/*! @brief       Defines the SPI signal event handler.
 *  @details     The Signal Event Handler function required by register_io_i2c.c.
 *  @param[in]   event The SPI event that occured.
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      void
 */
void SPI2_SignalEvent_t(uint32_t event);
#endif

#if defined(SPI3)
/*! @brief The SPI3 device index. */
#define SPI3_INDEX 3

/*! @brief       Defines the SPI signal event handler.
 *  @details     The Signal Event Handler function required by register_io_i2c.c.
 *  @param[in]   event The SPI event that occured.
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      void
 */
void SPI3_SignalEvent_t(uint32_t event);
#endif

#if defined(SPI4)
/*! @brief The SPI4 device index. */
#define SPI4_INDEX 4

/*! @brief       Defines the SPI signal event handler.
 *  @details     The Signal Event Handler function required by register_io_i2c.c.
 *  @param[in]   event The SPI event that occured.
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      void
 */
void SPI4_SignalEvent_t(uint32_t event);
#endif

#if defined(SPI5)
/*! @brief The SPI5 device index. */
#define SPI5_INDEX 5

/*! @brief       Defines the SPI signal event handler.
 *  @details     The Signal Event Handler function required by register_io_i2c.c.
 *  @param[in]   event The SPI event that occured.
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      void
 */
void SPI5_SignalEvent_t(uint32_t event);
#endif

#if defined(SPI6)
/*! @brief The SPI6 device index. */
#define SPI6_INDEX 6

/*! @brief       Defines the SPI signal event handler.
 *  @details     The Signal Event Handler function required by register_io_i2c.c.
 *  @param[in]   event The SPI event that occured.
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      void
 */
void SPI6_SignalEvent_t(uint32_t event);
#endif

#if defined(SPI7)
/*! @brief The SPI7 device index. */
#define SPI7_INDEX 7

/*! @brief       Defines the SPI signal event handler.
 *  @details     The Signal Event Handler function required by register_io_i2c.c.
 *  @param[in]   event The SPI event that occured.
 *  @Constraints None
 *  @Reentrant   Yes
 *  @return      void
 */
void SPI7_SignalEvent_t(uint32_t event);
#endif

#endif // __REGISTER_IO_SPI_H__
