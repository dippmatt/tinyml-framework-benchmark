/**
  ******************************************************************************
  * @file    stm32l4xx_hal_flash.h
  * @author  MCD Application Team
  * @brief   Header file of FLASH HAL module.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file in
  * the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32L4xx_HAL_FLASH_H
#define STM32L4xx_HAL_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal_def.h"

/** @addtogroup STM32L4xx_HAL_Driver
  * @{
  */

/** @addtogroup FLASH
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup FLASH_Exported_Types FLASH Exported Types
  * @{
  */

/**
  * @brief  FLASH Erase structure definition
  */
typedef struct
{
  uint32_t TypeErase;   /*!< Mass erase or page erase.
                             This parameter can be a value of @ref FLASH_Type_Erase */
  uint32_t Banks;       /*!< Select bank to erase.
                             This parameter must be a value of @ref FLASH_Banks
                             (FLASH_BANK_BOTH should be used only for mass erase) */
  uint32_t Page;        /*!< Initial Flash page to erase when page erase is disabled
                             This parameter must be a value between 0 and (max number of pages in the bank - 1)
                             (eg : 255 for 1MB dual bank) */
  uint32_t NbPages;     /*!< Number of pages to be erased.
                             This parameter must be a value between 1 and (max number of pages in the bank - value of initial page)*/
} FLASH_EraseInitTypeDef;

/**
  * @brief  FLASH Option Bytes Program structure definition
  */
typedef struct
{
  uint32_t OptionType;     /*!< Option byte to be configured.
                                This parameter can be a combination of the values of @ref FLASH_OB_Type */
  uint32_t WRPArea;        /*!< Write protection area to be programmed (used for OPTIONBYTE_WRP).
                                Only one WRP area could be programmed at the same time.
                                This parameter can be value of @ref FLASH_OB_WRP_Area */
  uint32_t WRPStartOffset; /*!< Write protection start offset (used for OPTIONBYTE_WRP).
                                This parameter must be a value between 0 and (max number of pages in the bank - 1)
                                (eg : 25 for 1MB dual bank) */
  uint32_t WRPEndOffset;   /*!< Write protection end offset (used for OPTIONBYTE_WRP).
                                This parameter must be a value between WRPStartOffset and (max number of pages in the bank - 1) */
  uint32_t RDPLevel;       /*!< Set the read protection level.. (used for OPTIONBYTE_RDP).
                                This parameter can be a value of @ref FLASH_OB_Read_Protection */
  uint32_t USERType;       /*!< User option byte(s) to be configured (used for OPTIONBYTE_USER).
                                This parameter can be a combination of @ref FLASH_OB_USER_Type */
  uint32_t USERConfig;     /*!< Value of the user option byte (used for OPTIONBYTE_USER).
                                This parameter can be a combination of @ref FLASH_OB_USER_BOR_LEVEL,
                                @ref FLASH_OB_USER_nRST_STOP, @ref FLASH_OB_USER_nRST_STANDBY,
                                @ref FLASH_OB_USER_nRST_SHUTDOWN, @ref FLASH_OB_USER_IWDG_SW,
                                @ref FLASH_OB_USER_IWDG_STOP, @ref FLASH_OB_USER_IWDG_STANDBY,
                                @ref FLASH_OB_USER_WWDG_SW, @ref FLASH_OB_USER_BFB2,
                                @ref FLASH_OB_USER_DUALBANK, @ref FLASH_OB_USER_nBOOT1,
                                @ref FLASH_OB_USER_SRAM2_PE, @ref FLASH_OB_USER_SRAM2_RST,
                                @ref FLASH_OB_USER_nSWBOOT0 and @ref FLASH_OB_USER_nBOOT0 */
  uint32_t PCROPConfig;    /*!< Configuration of the PCROP (used for OPTIONBYTE_PCROP).
                                This parameter must be a combination of @ref FLASH_Banks (except FLASH_BANK_BOTH)
                                and @ref FLASH_OB_PCROP_RDP */
  uint32_t PCROPStartAddr; /*!< PCROP Start address (used for OPTIONBYTE_PCROP).
                                This parameter must be a value between begin and end of bank
                                => Be careful of the bank swapping for the address */
  uint32_t PCROPEndAddr;   /*!< PCROP End address (used for OPTIONBYTE_PCROP).
                                This parameter must be a value between PCROP Start address and end of bank */
} FLASH_OBProgramInitTypeDef;

/**
  * @brief  FLASH Procedure structure definition
  */
typedef enum
{
  FLASH_PROC_NONE = 0,
  FLASH_PROC_PAGE_ERASE,
  FLASH_PROC_MASS_ERASE,
  FLASH_PROC_PROGRAM,
  FLASH_PROC_PROGRAM_LAST
} FLASH_ProcedureTypeDef;

/**
  * @brief  FLASH Cache structure definition
  */
typedef enum
{
  FLASH_CACHE_DISABLED = 0,
  FLASH_CACHE_ICACHE_ENABLED,
  FLASH_CACHE_DCACHE_ENABLED,
  FLASH_CACHE_ICACHE_DCACHE_ENABLED
} FLASH_CacheTypeDef;

/**
  * @brief  FLASH handle Structure definition
  */
typedef struct
{
  HAL_LockTypeDef             Lock;              /* FLASH locking object */
  __IO uint32_t               ErrorCode;         /* FLASH error code */
  __IO FLASH_ProcedureTypeDef ProcedureOnGoing;  /* Internal variable to indicate which procedure is ongoing or not in IT context */
  __IO uint32_t               Address;           /* Internal variable to save address selected for program in IT context */
  __IO uint32_t               Bank;              /* Internal variable to save current bank selected during erase in IT context */
  __IO uint32_t               Page;              /* Internal variable to define the current page which is erasing in IT context */
  __IO uint32_t               NbPagesToErase;    /* Internal variable to save the remaining pages to erase in IT context */
  __IO FLASH_CacheTypeDef     CacheToReactivate; /* Internal variable to indicate which caches should be reactivated */
}FLASH_ProcessTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup FLASH_Exported_Constants FLASH Exported Constants
  * @{
  */

/** @defgroup FLASH_Error FLASH Error
  * @{
  */
#define HAL_FLASH_ERROR_NONE      0x00000000U
#define HAL_FLASH_ERROR_OP        FLASH_FLAG_OPERR
#define HAL_FLASH_ERROR_PROG      FLASH_FLAG_PROGERR
#define HAL_FLASH_ERROR_WRP       FLASH_FLAG_WRPERR
#define HAL_FLASH_ERROR_PGA       FLASH_FLAG_PGAERR
#define HAL_FLASH_ERROR_SIZ       FLASH_FLAG_SIZERR
#define HAL_FLASH_ERROR_PGS       FLASH_FLAG_PGSERR
#define HAL_FLASH_ERROR_MIS       FLASH_FLAG_MISERR
#define HAL_FLASH_ERROR_FAST      FLASH_FLAG_FASTERR
#define HAL_FLASH_ERROR_RD        FLASH_FLAG_RDERR
#define HAL_FLASH_ERROR_OPTV      FLASH_FLAG_OPTVERR
#define HAL_FLASH_ERROR_ECCC      FLASH_FLAG_ECCC
#define HAL_FLASH_ERROR_ECCD      FLASH_FLAG_ECCD
#if defined (STM32L412xx) || defined (STM32L422xx) || defined (STM32L431xx) || defined (STM32L432xx) || defined (STM32L433xx) || defined (STM32L442xx) || \
    defined (STM32L443xx) || defined (STM32L451xx) || defined (STM32L452xx) || defined (STM32L462xx) || defined (STM32L496xx) || defined (STM32L4A6xx) || \
    defined (STM32L4P5xx) || defined (STM32L4Q5xx) || defined (STM32L4R5xx) || defined (STM32L4R7xx) || defined (STM32L4R9xx) || defined (STM32L4S5xx) || \
    defined (STM32L4S7xx) || defined (STM32L4S9xx)
#define HAL_FLASH_ERROR_PEMPTY    FLASH_FLAG_PEMPTY
#endif
/**
  * @}
  */

/** @defgroup FLASH_Type_Erase FLASH Erase Type
  * @{
  */
#define FLASH_TYPEERASE_PAGES     ((uint32_t)0x00)  /*!<Pages erase only*/
#define FLASH_TYPEERASE_MASSERASE ((uint32_t)0x01)  /*!<Flash mass erase activation*/
/**
  * @}
  */

/** @defgroup FLASH_Banks FLASH Banks
  * @{
  */
#define FLASH_BANK_1              ((uint32_t)0x01)                          /*!< Bank 1   */
#if defined (STM32L471xx) || defined (STM32L475xx) || defined (STM32L476xx) || defined (STM32L485xx) || defined (STM32L486xx) || \
    defined (STM32L496xx) || defined (STM32L4A6xx) || defined (STM32L4P5xx) || defined (STM32L4Q5xx) || defined (STM32L4R5xx) || \
    defined (STM32L4R7xx) || defined (STM32L4R9xx) || defined (STM32L4S5xx) || defined (STM32L4S7xx) || defined (STM32L4S9xx)
#define FLASH_BANK_2              ((uint32_t)0x02)                          /*!< Bank 2   */
#define FLASH_BANK_BOTH           ((uint32_t)(FLASH_BANK_1 | FLASH_BANK_2)) /*!< Bank1 and Bank2  */
#else
#define FLASH_BANK_BOTH           ((uint32_t)(FLASH_BANK_1))                /*!< Bank 1   */
#endif
/**
  * @}
  */


/** @defgroup FLASH_Type_Program FLASH Program Type
  * @{
  */
#define FLASH_TYPEPROGRAM_DOUBLEWORD    ((uint32_t)0x00)  /*!<Program a double-word (64-bit) at a specified address.*/
#define FLASH_TYPEPROGRAM_FAST          ((uint32_t)0x01)  /*!<Fast program a 32 row double-word (64-bit) at a specified address.
                                                                 And another 32 row double-word (64-bit) will be programmed */
#define FLASH_TYPEPROGRAM_FAST_AND_LAST ((uint32_t)0x02)  /*!<Fast program a 32 row double-word (64-bit) at a specified address.
                                                                 And this is the last 32 row double-word (64-bit) programmed */
/**
  * @}
  */

/** @defgroup FLASH_OB_Type FLASH Option Bytes Type
  * @{
  */
#define OPTIONBYTE_WRP            ((uint32_t)0x01)  /*!< WRP option byte configuration */
#define OPTIONBYTE_RDP            ((uint32_t)0x02)  /*!< RDP option byte configuration */
#define OPTIONBYTE_USER           ((uint32_t)0x04)  /*!< USER option byte configuration */
#define OPTIONBYTE_PCROP          ((uint32_t)0x08)  /*!< PCROP option byte configuration */
/**
  * @}
  */

/** @defgroup FLASH_OB_WRP_Area FLASH WRP Area
  * @{
  */
#define OB_WRPAREA_BANK1_AREAA    ((uint32_t)0x00)  /*!< Flash Bank 1 Area A */
#define OB_WRPAREA_BANK1_AREAB    ((uint32_t)0x01)  /*!< Flash Bank 1 Area B */
#if defined (STM32L471xx) || defined (STM32L475xx) || defined (STM32L476xx) || defined (STM32L485xx) || defined (STM32L486xx) || \
    defined (STM32L496xx) || defined (STM32L4A6xx) || defined (STM32L4P5xx) || defined (STM32L4Q5xx) || defined (STM32L4R5xx) || \
    defined (STM32L4R7xx) || defined (STM32L4R9xx) || defined (STM32L4S5xx) || defined (STM32L4S7xx) || defined (STM32L4S9xx)
#define OB_WRPAREA_BANK2_AREAA    ((uint32_t)0x02)  /*!< Flash Bank 2 Area A */
#define OB_WRPAREA_BANK2_AREAB    ((uint32_t)0x04)  /*!< Flash Bank 2 Area B */
#endif
/**
  * @}
  */

/** @defgroup FLASH_OB_Read_Protection FLASH Option Bytes Read Protection
  * @{
  */
#define OB_RDP_LEVEL_0            ((uint32_t)0xAA)
#define OB_RDP_LEVEL_1            ((uint32_t)0xBB)
#define OB_RDP_LEVEL_2            ((uint32_t)0xCC) /*!< Warning: When enabling read protection level 2
                                                        it's no more possible to go back to level 1 or 0 */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_Type FLASH Option Bytes User Type
  * @{
  */
#define OB_USER_BOR_LEV           ((uint32_t)0x0001)                   /*!< BOR reset Level */
#define OB_USER_nRST_STOP         ((uint32_t)0x0002)                   /*!< Reset generated when entering the stop mode */
#define OB_USER_nRST_STDBY        ((uint32_t)0x0004)                   /*!< Reset generated when entering the standby mode */
#define OB_USER_IWDG_SW           ((uint32_t)0x0008)                   /*!< Independent watchdog selection */
#define OB_USER_IWDG_STOP         ((uint32_t)0x0010)                   /*!< Independent watchdog counter freeze in stop mode */
#define OB_USER_IWDG_STDBY        ((uint32_t)0x0020)                   /*!< Independent watchdog counter freeze in standby mode */
#define OB_USER_WWDG_SW           ((uint32_t)0x0040)                   /*!< Window watchdog selection */
#if defined (STM32L471xx) || defined (STM32L475xx) || defined (STM32L476xx) || defined (STM32L485xx) || defined (STM32L486xx) || \
    defined (STM32L496xx) || defined (STM32L4A6xx) || defined (STM32L4P5xx) || defined (STM32L4Q5xx) || defined (STM32L4R5xx) || \
    defined (STM32L4R7xx) || defined (STM32L4R9xx) || defined (STM32L4S5xx) || defined (STM32L4S7xx) || defined (STM32L4S9xx)
#define OB_USER_BFB2              ((uint32_t)0x0080)                   /*!< Dual-bank boot */
#if defined (STM32L4R5xx) || defined (STM32L4R7xx) || defined (STM32L4R9xx) || defined (STM32L4S5xx) || defined (STM32L4S7xx) || defined (STM32L4S9xx)
#define OB_USER_DUALBANK          ((uint32_t)0x0100)                   /*!< Dual-Bank on 1MB or 512kB Flash memory devices */
#else
#define OB_USER_DUALBANK          ((uint32_t)0x0100)                   /*!< Dual-Bank on 512KB or 256KB Flash memory devices */
#endif
#endif
#define OB_USER_nBOOT1            ((uint32_t)0x0200)                   /*!< Boot configuration */
#define OB_USER_SRAM2_PE          ((uint32_t)0x0400)                   /*!< SRAM2 parity check enable */
#define OB_USER_SRAM2_RST         ((uint32_t)0x0800)                   /*!< SRAM2 Erase when system reset */
#define OB_USER_nRST_SHDW         ((uint32_t)0x1000)                   /*!< Reset generated when entering the shutdown mode */
#if defined (STM32L412xx) || defined (STM32L422xx) || defined (STM32L431xx) || defined (STM32L432xx) || defined (STM32L433xx) || \
    defined (STM32L442xx) || defined (STM32L443xx) || defined (STM32L451xx) || defined (STM32L452xx) || defined (STM32L462xx) || \
    defined (STM32L496xx) || defined (STM32L4A6xx) || defined (STM32L4P5xx) || defined (STM32L4Q5xx) || defined (STM32L4R5xx) || \
    defined (STM32L4R7xx) || defined (STM32L4R9xx) || defined (STM32L4S5xx) || defined (STM32L4S7xx) || defined (STM32L4S9xx)
#define OB_USER_nSWBOOT0          ((uint32_t)0x2000)                   /*!< Software BOOT0 */
#define OB_USER_nBOOT0            ((uint32_t)0x4000)                   /*!< nBOOT0 option bit */
#endif
#if defined (STM32L4P5xx) || defined (STM32L4Q5xx) || defined (STM32L4R5xx) || defined (STM32L4R7xx) || defined (STM32L4R9xx) || defined (STM32L4S5xx) || defined (STM32L4S7xx) || defined (STM32L4S9xx)
#define OB_USER_DBANK             ((uint32_t)0x8000)                   /*!< Single bank with 128-bits data or two banks with 64-bits data */
#endif
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_BOR_LEVEL FLASH Option Bytes User BOR Level
  * @{
  */
#define OB_BOR_LEVEL_0            ((uint32_t)FLASH_OPTR_BOR_LEV_0)     /*!< Reset level threshold is around 1.7V */
#define OB_BOR_LEVEL_1            ((uint32_t)FLASH_OPTR_BOR_LEV_1)     /*!< Reset level threshold is around 2.0V */
#define OB_BOR_LEVEL_2            ((uint32_t)FLASH_OPTR_BOR_LEV_2)     /*!< Reset level threshold is around 2.2V */
#define OB_BOR_LEVEL_3            ((uint32_t)FLASH_OPTR_BOR_LEV_3)     /*!< Reset level threshold is around 2.5V */
#define OB_BOR_LEVEL_4            ((uint32_t)FLASH_OPTR_BOR_LEV_4)     /*!< Reset level threshold is around 2.8V */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_nRST_STOP FLASH Option Bytes User Reset On Stop
  * @{
  */
#define OB_STOP_RST               ((uint32_t)0x0000)                   /*!< Reset generated when entering the stop mode */
#define OB_STOP_NORST             ((uint32_t)FLASH_OPTR_nRST_STOP)     /*!< No reset generated when entering the stop mode */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_nRST_STANDBY FLASH Option Bytes User Reset On Standby
  * @{
  */
#define OB_STANDBY_RST            ((uint32_t)0x0000)                   /*!< Reset generated when entering the standby mode */
#define OB_STANDBY_NORST          ((uint32_t)FLASH_OPTR_nRST_STDBY)    /*!< No reset generated when entering the standby mode */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_nRST_SHUTDOWN FLASH Option Bytes User Reset On Shutdown
  * @{
  */
#define OB_SHUTDOWN_RST           ((uint32_t)0x0000)                   /*!< Reset generated when entering the shutdown mode */
#define OB_SHUTDOWN_NORST         ((uint32_t)FLASH_OPTR_nRST_SHDW)     /*!< No reset generated when entering the shutdown mode */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_IWDG_SW FLASH Option Bytes User IWDG Type
  * @{
  */
#define OB_IWDG_HW                ((uint32_t)0x00000)                  /*!< Hardware independent watchdog */
#define OB_IWDG_SW                ((uint32_t)FLASH_OPTR_IWDG_SW)       /*!< Software independent watchdog */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_IWDG_STOP FLASH Option Bytes User IWDG Mode On Stop
  * @{
  */
#define OB_IWDG_STOP_FREEZE       ((uint32_t)0x00000)                  /*!< Independent watchdog counter is frozen in Stop mode */
#define OB_IWDG_STOP_RUN          ((uint32_t)FLASH_OPTR_IWDG_STOP)     /*!< Independent watchdog counter is running in Stop mode */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_IWDG_STANDBY FLASH Option Bytes User IWDG Mode On Standby
  * @{
  */
#define OB_IWDG_STDBY_FREEZE      ((uint32_t)0x00000)                  /*!< Independent watchdog counter is frozen in Standby mode */
#define OB_IWDG_STDBY_RUN         ((uint32_t)FLASH_OPTR_IWDG_STDBY)    /*!< Independent watchdog counter is running in Standby mode */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_WWDG_SW FLASH Option Bytes User WWDG Type
  * @{
  */
#define OB_WWDG_HW                ((uint32_t)0x00000)                  /*!< Hardware window watchdog */
#define OB_WWDG_SW                ((uint32_t)FLASH_OPTR_WWDG_SW)       /*!< Software window watchdog */
/**
  * @}
  */

#if defined (STM32L471xx) || defined (STM32L475xx) || defined (STM32L476xx) || defined (STM32L485xx) || defined (STM32L486xx) || \
    defined (STM32L496xx) || defined (STM32L4A6xx) || defined (STM32L4P5xx) || defined (STM32L4Q5xx) || defined (STM32L4R5xx) || \
    defined (STM32L4R7xx) || defined (STM32L4R9xx) || defined (STM32L4S5xx) || defined (STM32L4S7xx) || defined (STM32L4S9xx)
/** @defgroup FLASH_OB_USER_BFB2 FLASH Option Bytes User BFB2 Mode
  * @{
  */
#define OB_BFB2_DISABLE           ((uint32_t)0x000000)                 /*!< Dual-bank boot disable */
#define OB_BFB2_ENABLE            ((uint32_t)FLASH_OPTR_BFB2)          /*!< Dual-bank boot enable */
/**
  * @}
  */
#if defined (STM32L4P5xx) || defined (STM32L4Q5xx) || defined (STM32L4R5xx) || defined (STM32L4R7xx) || defined (STM32L4R9xx) || defined (STM32L4S5xx) || defined (STM32L4S7xx) || defined (STM32L4S9xx)
/** @defgroup FLASH_OB_USER_DUALBANK FLASH Option Bytes User Dual-bank Type
  * @{
  */
#define OB_DUALBANK_SINGLE        ((uint32_t)0x000000)                 /*!< 1 MB/512 kB Single-bank Flash */
#define OB_DUALBANK_DUAL          ((uint32_t)FLASH_OPTR_DB1M)          /*!< 1 MB/512 kB Dual-bank Flash */
/**
  * @}
  */
#else
/** @defgroup FLASH_OB_USER_DUALBANK FLASH Option Bytes User Dual-bank Type
  * @{
  */
#define OB_DUALBANK_SINGLE        ((uint32_t)0x000000)                 /*!< 256 KB/512 KB Single-bank Flash */
#define OB_DUALBANK_DUAL          ((uint32_t)FLASH_OPTR_DUALBANK)      /*!< 256 KB/512 KB Dual-bank Flash */
/**
  * @}
  */
#endif
#endif

#if defined (STM32L4P5xx) || defined (STM32L4Q5xx) || defined (STM32L4R5xx) || defined (STM32L4R7xx) || defined (STM32L4R9xx) || defined (STM32L4S5xx) || defined (STM32L4S7xx) || defined (STM32L4S9xx)
/** @defgroup FLASH_OB_USER_DBANK FLASH Option Bytes User DBANK Type
  * @{
  */
#define OB_DBANK_128_BITS         ((uint32_t)0x000000)                 /*!< Single-bank with 128-bits data */
#define OB_DBANK_64_BITS          ((uint32_t)FLASH_OPTR_DBANK)         /*!< Dual-bank with 64-bits data */
#endif
/**
  * @}
  */
/** @defgroup FLASH_OB_USER_nBOOT1 FLASH Option Bytes User BOOT1 Type
  * @