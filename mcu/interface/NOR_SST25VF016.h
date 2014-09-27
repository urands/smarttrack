/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Drivers:NOR
 * Copyright (c) 2004-2013 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    NOR_AT45DB642D.h
 * Purpose: NOR Flash Device Description for Atmel Data Flash AT45DB642D (SPI))
 * Rev.:    V5.04
 *----------------------------------------------------------------------------*/

#define NOR_FLASH_NAME  SST25VF016          /* Flash Device Name */

#define NOR_FLASH_SIZE  0x200000            /* Flash Device Size in bytes */
#define NOR_FLASH_EVAL  0xFF                /* Contents of Erased Memory */

#define NOR_FLASH_SECTORS                                           \
  ARM_NOR_SECTOR_INFO(0x000000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x021000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x042000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x063000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x084000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x0A5000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x0C6000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x0E7000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x108000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x129000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x14A000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x16B000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x18C000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x1AD000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x1CE000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x1EF000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x210000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x231000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x252000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x273000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x294000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x2B5000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x2D6000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x2F7000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x318000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x339000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x35A000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x37B000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x39C000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x3BD000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x3DE000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x3FF000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x420000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x441000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x462000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x483000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x4A4000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x4C5000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x4E6000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x507000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x528000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x549000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x56A000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x58B000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x5AC000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x5CD000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x5EE000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x60F000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x630000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x651000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x672000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x693000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x6B4000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x6D5000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x6F6000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x717000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x738000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x759000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x77A000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x79B000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x7BC000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x7DD000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x7FE000, 0x21000),   /* Sector Size 132kB */ \
  ARM_NOR_SECTOR_INFO(0x81F000, 0x21000)    /* Sector Size 132kB */

#define NOR_FLASH_SECTOR_NUM    64          /* Number of Sectors */

