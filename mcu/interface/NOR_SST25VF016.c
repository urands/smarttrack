/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Drivers:NOR
 * Copyright (c) 2004-2013 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    NOR_AT45DB642D.c
 * Purpose: NOR Flash Device Driver for Atmel Data Flash AT45DB642D (SPI)
 * Rev.:    V5.04
 *----------------------------------------------------------------------------*/

#include "Driver_NOR.h"
#include "Driver_SPI.h"
#include "NOR_SST25VF016.h"

#define ARM_NOR_DRV_VERSION ARM_DRV_VERSION_MAJOR_MINOR(5,04)    /* driver version */


/* SPI Data Flash definitions */
#define BUS_SPEED       20000000        /* Bus speed   */
#define PAGE_SIZE       4096            /* Page size   */
#define BLOCK_SIZE      ( 8* PAGE_SIZE) /* Block size) */
#define SECTOR_SIZE     (16*BLOCK_SIZE) /* Sector size */

/* SPI Data Flash Commands */
#define CMD_READ_DATA           0x03
#define CMD_READ_STATUS         0x05
#define CMD_BLOCK_ERASE         0x20
#define CMD_BUF_WRITE           0x02
//#define CMD_PAGE_PROGRAM        0x83
//#define CMD_PAGE_READ           0x53


/* SPI Driver */
extern ARM_DRIVER_SPI *ARM_NOR_SPI_Driver_(NOR_FLASH_NAME);
#define ptrSPI        (ARM_NOR_SPI_Driver_(NOR_FLASH_NAME))


/* Driver Version */
static const ARM_DRV_VERSION DriverVersion = {
  ARM_NOR_API_VERSION,
  ARM_NOR_DRV_VERSION
};

/* Sector Information */
static ARM_NOR_SECTOR ARM_NOR_Sector_(NOR_FLASH_NAME)[NOR_FLASH_SECTOR_NUM] = {
  NOR_FLASH_SECTORS
};

/* Device Information */
ARM_NOR_DEVICE ARM_NOR_Device_(NOR_FLASH_NAME) = {
  NOR_FLASH_SIZE,
  NOR_FLASH_SECTOR_NUM,
  ARM_NOR_Sector_(NOR_FLASH_NAME),
  NOR_FLASH_EVAL
};


/* Send Command to Data Flash */
static bool SendCommand (uint8_t cmd, uint32_t addr, const uint8_t *data, uint32_t size) {
  ARM_SPI_STATUS spi_status;
  uint32_t       page_addr;
  uint32_t       page_offs;
  uint8_t        sr;
  uint8_t        buf[4];

  page_addr = addr / PAGE_SIZE;
  page_offs = addr % PAGE_SIZE;

 // addr = (page_addr << 11) | page_offs;

  /* Prepare Command with Address */
  buf[0] = cmd;
  buf[1] = (uint8_t)(addr >> 16);
  buf[2] = (uint8_t)(addr >>  8);
  buf[3] = (uint8_t)(addr >>  0);
  
  /* Select Slave */
  spi_status = ptrSPI->SlaveSelect(ARM_SPI_SS_ACTIVE);
  if (spi_status != ARM_SPI_OK) return ARM_NOR_ERROR;

  /* Send Command with Address */
  spi_status = ptrSPI->SendData(buf, 1);
  if (spi_status != ARM_SPI_OK) goto data_error;

  /* Send Data */
  if ((data != NULL) && (size != 0)) {
    spi_status = ptrSPI->SendData(data, size);
    if (spi_status != ARM_SPI_OK) goto data_error;
  }

  /* Deselect Slave */
  spi_status = ptrSPI->SlaveSelect(ARM_SPI_SS_INACTIVE);
  if (spi_status != ARM_SPI_OK) return ARM_NOR_ERROR;
	return true;
  /* Select Slave */
  spi_status = ptrSPI->SlaveSelect(ARM_SPI_SS_ACTIVE);
  if (spi_status != ARM_SPI_OK) return ARM_NOR_ERROR;

  ptrSPI->TransferByte(CMD_READ_STATUS);
  ptrSPI->TransferByte(0xFF);           /* Dummy byte */

  /* Wait while device is busy */
  do {
    sr = ptrSPI->TransferByte(0xFF);    /* Status register */
  } while ((sr & 0x80) == 0);

  /* Deselect Slave */
  spi_status = ptrSPI->SlaveSelect(ARM_SPI_SS_INACTIVE);
  if (spi_status != ARM_SPI_OK) return ARM_NOR_ERROR;

  return true;

data_error:
  ptrSPI->SlaveSelect(ARM_SPI_SS_INACTIVE);
  return false;
}


/**
  \fn          ARM_DRV_VERSION NOR_GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRV_VERSION
*/
static ARM_DRV_VERSION NOR_GetVersion (void) {
  return DriverVersion;
}


/**
  \fn          ARM_NOR_STATUS NOR_Initialize (uint32_t base_addr)
  \brief       Initialize NOR Flash Interface.
  \param[in]   base_addr specifies device base address
  \return      \ref ARM_NOR_STATUS
*/
static ARM_NOR_STATUS NOR_Initialize (uint32_t base_addr) {
  ARM_SPI_STATUS spi_status;

  spi_status = ptrSPI->Initialize(NULL);
  if (spi_status != ARM_SPI_OK) return ARM_NOR_ERROR;

  spi_status = ptrSPI->PowerControl(ARM_POWER_FULL);
  if (spi_status != ARM_SPI_OK) return ARM_NOR_ERROR;

  spi_status = ptrSPI->Configure(ARM_SPI_CPOL0_CPHA0, ARM_SPI_LSB_MSB);
  if (spi_status != ARM_SPI_OK) return ARM_NOR_ERROR;

  ptrSPI->BusSpeed(BUS_SPEED);
	
	SendCommand(0x06, 0,0,0); //Write enable
	
	
	SendCommand(0x60, 0,0,0); //Chip erase

  return ARM_NOR_OK;
}

/**
  \fn          ARM_NOR_STATUS NOR_Uninitialize (void)
  \brief       De-initialize NOR Flash Interface.
  \return      \ref ARM_NOR_STATUS
*/
static ARM_NOR_STATUS NOR_Uninitialize (void) {
  ARM_SPI_STATUS spi_status;

  spi_status = ptrSPI->Uninitialize();
  if (spi_status != ARM_SPI_OK) return ARM_NOR_ERROR;

  return ARM_NOR_OK;
} 

/**
  \fn          ARM_NOR_STATUS NOR_PowerControl (ARM_POWER_STATE state)
  \brief       Control NOR Flash Interface Power.
  \param[in]   state specifies Power state
  \return      \ref NOR_STATUS
*/
static ARM_NOR_STATUS NOR_PowerControl (ARM_POWER_STATE state) {
  /* ... */
  return ARM_NOR_OK;
}

/**
  \fn          ARM_NOR_STATUS NOR_ReadData (uint32_t addr, uint8_t *data, uint32_t size)
  \brief       Read data from NOR Flash.
               Optional function used for non-memory mapped devices.
  \param[in]   addr specifies data address
  \param[out]  data specifies pointer to buffer for data read from NOR Flash
  \param[in]   size specifies number of bytes to read
  \return      \ref ARM_NOR_STATUS
*/
static ARM_NOR_STATUS NOR_ReadData (uint32_t addr, uint8_t *data, uint32_t size) {
  ARM_SPI_STATUS spi_status;
  //uint32_t       page_addr;
  //uint32_t       page_offs;
  uint8_t        buf[8];

  //page_addr = addr / PAGE_SIZE;
  //page_offs = addr % PAGE_SIZE;

  //addr = (page_addr << 11) | page_offs;

  /* Prepare Command with Address */
  buf[0] = CMD_READ_DATA;
  buf[1] = (uint8_t)(addr >> 16);
  buf[2] = (uint8_t)(addr >>  8);
  buf[3] = (uint8_t)(addr >>  0);
  /* buf[4..7]: don't care */
  
  /* Select Slave */
  spi_status = ptrSPI->SlaveSelect(ARM_SPI_SS_ACTIVE);
  if (spi_status != ARM_SPI_OK) return ARM_NOR_ERROR;

  /* Send Command with Address */
  spi_status = ptrSPI->SendData(buf, 4);
  if (spi_status != ARM_SPI_OK) goto data_error;

  /* Receive Data */
  spi_status = ptrSPI->ReceiveData(data, size, 0xFF);
  if (spi_status != ARM_SPI_OK) goto data_error;

  /* Deselect Slave */
  spi_status = ptrSPI->SlaveSelect(ARM_SPI_SS_INACTIVE);
  if (spi_status != ARM_SPI_OK) return ARM_NOR_ERROR;

  return ARM_NOR_OK;

data_error:
  ptrSPI->SlaveSelect(ARM_SPI_SS_INACTIVE);
  return ARM_NOR_ERROR;
}

/**
  \fn          ARM_NOR_STATUS NOR_WriteData (uint32_t addr, const uint8_t *data, uint32_t size)
  \brief       Write data to NOR Flash.
  \param[in]   addr specifies data address
  \param[in]   data specifies pointer to buffer with data to write to NOR Flash
  \param[in]   size specifies number of bytes to write
  \return      \ref ARM_NOR_STATUS
*/
static ARM_NOR_STATUS NOR_WriteData (uint32_t addr, const uint8_t *data, uint32_t size) {
  uint32_t cnt;

  while (size) {
//    if (!SendCommand(CMD_PAGE_READ,    addr, NULL, 0))   return ARM_NOR_ERROR;

    cnt = PAGE_SIZE - (addr % PAGE_SIZE);
    if (cnt > size) cnt = size;
    if (!SendCommand(CMD_BUF_WRITE,    addr, data, cnt)) return ARM_NOR_ERROR;
//    if (!SendCommand(CMD_PAGE_PROGRAM, addr, NULL, 0))   return ARM_NOR_ERROR;

    addr += cnt;
    data += cnt;
    size -= cnt;
  }

  return ARM_NOR_OK;
}

/**
  \fn          ARM_NOR_STATUS NOR_EraseSector (uint32_t addr)
  \brief       Erase NOR Flash Sector.
  \param[in]   addr specifies sector address
  \return      \ref ARM_NOR_STATUS
*/
static ARM_NOR_STATUS NOR_EraseSector (uint32_t addr) {
  uint32_t n;

  for (n = 0; n < (SECTOR_SIZE/BLOCK_SIZE); n++) {
    if (!SendCommand(CMD_BLOCK_ERASE, addr, NULL, 0)) return ARM_NOR_ERROR;
    addr += BLOCK_SIZE;
  }

  return ARM_NOR_OK;
}


/* NOR_Flash Device Driver Control Block */
ARM_DRIVER_NOR ARM_Driver_NOR_(NOR_FLASH_NAME) = {
  NOR_GetVersion,
  NOR_Initialize,
  NOR_Uninitialize,
  NOR_PowerControl,
  NOR_ReadData,
  NOR_WriteData,
  NOR_EraseSector,
  NULL
};

