/**
  ******************************************************************************
  * @file    stm32l4xx_hal_exti.h
  * @author  MCD Application Team
  * @brief   Header file of EXTI HAL module.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32L4xx_HAL_EXTI_H
#define STM32L4xx_HAL_EXTI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal_def.h"

/** @addtogroup STM32L4xx_HAL_Driver
  * @{
  */

/** @defgroup EXTI EXTI
  * @brief EXTI HAL module driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup EXTI_Exported_Types EXTI Exported Types
  * @{
  */
typedef enum
{
  HAL_EXTI_COMMON_CB_ID          = 0x00U,
  HAL_EXTI_RISING_CB_ID          = 0x01U,
  HAL_EXTI_FALLING_CB_ID         = 0x02U,
} EXTI_CallbackIDTypeDef;


/**
  * @brief  EXTI Handle structure definition
  */
typedef struct
{
  uint32_t Line;                    /*!<  Exti line number */
  void (* PendingCallback)(void);   /*!<  Exti pending callback */
} EXTI_HandleTypeDef;

/**
  * @brief  EXTI Configuration structure definition
  */
typedef struct
{
  uint32_t Line;      /*!< The Exti line to be configured. This parameter
                           can be a value of @ref EXTI_Line */
  uint32_t Mode;      /*!< The Exit Mode to be configured for a core.
                           This parameter can be a combination of @ref EXTI_Mode */
  uint32_t Trigger;   /*!< The Exti Trigger to be configured. This parameter
                           can be a value of @ref EXTI_Trigger */
  uint32_t GPIOSel;   /*!< The Exti GPIO multiplexer selection to be configured.
                           This parameter is only possible for line 0 to 15. It
                           can be a value of @ref EXTI_GPIOSel */
} EXTI_ConfigTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup EXTI_Exported_Constants EXTI Exported Constants
  * @{
  */

/** @defgroup EXTI_Line  EXTI Line
  * @{
  */
#if defined(STM32L412xx) || defined(STM32L422xx)

#define EXTI_LINE_0                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x00u)
#define EXTI_LINE_1                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x01u)
#define EXTI_LINE_2                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x02u)
#define EXTI_LINE_3                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x03u)
#define EXTI_LINE_4                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x04u)
#define EXTI_LINE_5                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x05u)
#define EXTI_LINE_6                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x06u)
#define EXTI_LINE_7                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x07u)
#define EXTI_LINE_8                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x08u)
#define EXTI_LINE_9                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x09u)
#define EXTI_LINE_10                        (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x0Au)
#define EXTI_LINE_11                        (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x0Bu)
#define EXTI_LINE_12                        (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x0Cu)
#define EXTI_LINE_13                        (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x0Du)
#define EXTI_LINE_14                        (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x0Eu)
#define EXTI_LINE_15                        (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x0Fu)
#define EXTI_LINE_16                        (EXTI_CONFIG   | EXTI_REG1 | EXTI_EVENT | 0x10u)
#define EXTI_LINE_17                        (EXTI_DIRECT   | EXTI_REG1 | EXTI_EVENT | 0x11u)
#define EXTI_LINE_18                        (EXTI_CONFIG   | EXTI_REG1 | EXTI_EVENT | 0x12u)
#define EXTI_LINE_19                        (EXTI_CONFIG   | EXTI_REG1 | EXTI_EVENT | 0x13u)
#define EXTI_LINE_20                        (EXTI_CONFIG   | EXTI_REG1 | EXTI_EVENT | 0x14u)
#define EXTI_LINE_21                        (EXTI_CONFIG   | EXTI_REG1 | EXTI_EVENT | 0x15u)
#define EXTI_LINE_22                        (EXTI_RESERVED | EXTI_REG1              | 0x16u)
#define EXTI_LINE_23                        (EXTI_DIRECT   | EXTI_REG1 | EXTI_EVENT | 0x17u)
#define EXTI_LINE_24                        (EXTI_DIRECT   | EXTI_REG1 | EXTI_EVENT | 0x18u)
#define EXTI_LINE_25                        (EXTI_DIRECT   | EXTI_REG1 | EXTI_EVENT | 0x19u)
#define EXTI_LINE_26                        (EXTI_DIRECT   | EXTI_REG1 | EXTI_EVENT | 0x1Au)
#define EXTI_LINE_27                        (EXTI_DIRECT   | EXTI_REG1 | EXTI_EVENT | 0x1Bu)
#define EXTI_LINE_28                        (EXTI_DIRECT   | EXTI_REG1 | EXTI_EVENT | 0x1Cu)
#define EXTI_LINE_29                        (EXTI_RESERVED | EXTI_REG1              | 0x1Du)
#define EXTI_LINE_30                        (EXTI_RESERVED | EXTI_REG1              | 0x1Eu)
#define EXTI_LINE_31                        (EXTI_DIRECT   | EXTI_REG1 | EXTI_EVENT | 0x1Fu)
#define EXTI_LINE_32                        (EXTI_DIRECT   | EXTI_REG2 | EXTI_EVENT | 0x00u)
#define EXTI_LINE_33                        (EXTI_DIRECT   | EXTI_REG2 | EXTI_EVENT | 0x01u)
#define EXTI_LINE_34                        (EXTI_RESERVED | EXTI_REG2              | 0x02u)
#define EXTI_LINE_35                        (EXTI_CONFIG   | EXTI_REG2 | EXTI_EVENT | 0x03u)
#define EXTI_LINE_36                        (EXTI_RESERVED | EXTI_REG2              | 0x04u)
#define EXTI_LINE_37                        (EXTI_CONFIG   | EXTI_REG2 | EXTI_EVENT | 0x05u)
#define EXTI_LINE_38                        (EXTI_CONFIG   | EXTI_REG2 | EXTI_EVENT | 0x06u)
#define EXTI_LINE_39                        (EXTI_RESERVED | EXTI_REG2              | 0x07u)
#define EXTI_LINE_40                        (EXTI_RESERVED | EXTI_REG2              | 0x08u)

#endif /* STM32L412xx || STM32L422xx */

#if defined(STM32L431xx)

#define EXTI_LINE_0                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x00u)
#define EXTI_LINE_1                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x01u)
#define EXTI_LINE_2                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x02u)
#define EXTI_LINE_3                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x03u)
#define EXTI_LINE_4                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x04u)
#define EXTI_LINE_5                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x05u)
#define EXTI_LINE_6                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x06u)
#define EXTI_LINE_7                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x07u)
#define EXTI_LINE_8                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x08u)
#define EXTI_LINE_9                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x09u)
#define EXTI_LINE_10                        (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x0Au)
#define EXTI_LINE_11                        (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x0Bu)
#define EXTI_LINE_12                        (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x0Cu)
#define EXTI_LINE_13                        (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x0Du)
#define EXTI_LINE_14                        (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x0Eu)
#define EXTI_LINE_15                        (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x0Fu)
#define EXTI_LINE_16                        (EXTI_CONFIG   | EXTI_REG1 | EXTI_EVENT | 0x10u)
#define EXTI_LINE_17                        (EXTI_RESERVED | EXTI_REG1              | 0x11u)
#define EXTI_LINE_18                        (EXTI_CONFIG   | EXTI_REG1 | EXTI_EVENT | 0x12u)
#define EXTI_LINE_19                        (EXTI_CONFIG   | EXTI_REG1 | EXTI_EVENT | 0x13u)
#define EXTI_LINE_20                        (EXTI_CONFIG   | EXTI_REG1 | EXTI_EVENT | 0x14u)
#define EXTI_LINE_21                        (EXTI_CONFIG   | EXTI_REG1 | EXTI_EVENT | 0x15u)
#define EXTI_LINE_22                        (EXTI_CONFIG   | EXTI_REG1 | EXTI_EVENT | 0x16u)
#define EXTI_LINE_23                        (EXTI_DIRECT   | EXTI_REG1 | EXTI_EVENT | 0x17u)
#define EXTI_LINE_24                        (EXTI_DIRECT   | EXTI_REG1 | EXTI_EVENT | 0x18u)
#define EXTI_LINE_25                        (EXTI_DIRECT   | EXTI_REG1 | EXTI_EVENT | 0x19u)
#define EXTI_LINE_26                        (EXTI_DIRECT   | EXTI_REG1 | EXTI_EVENT | 0x1Au)
#define EXTI_LINE_27                        (EXTI_DIRECT   | EXTI_REG1 | EXTI_EVENT | 0x1Bu)
#define EXTI_LINE_28                        (EXTI_DIRECT   | EXTI_REG1 | EXTI_EVENT | 0x1Cu)
#define EXTI_LINE_29                        (EXTI_RESERVED | EXTI_REG1              | 0x1Du)
#define EXTI_LINE_30                        (EXTI_RESERVED | EXTI_REG1              | 0x1Eu)
#define EXTI_LINE_31                        (EXTI_DIRECT   | EXTI_REG1 | EXTI_EVENT | 0x1Fu)
#define EXTI_LINE_32                        (EXTI_DIRECT   | EXTI_REG2 | EXTI_EVENT | 0x00u)
#define EXTI_LINE_33                        (EXTI_DIRECT   | EXTI_REG2 | EXTI_EVENT | 0x01u)
#define EXTI_LINE_34                        (EXTI_DIRECT   | EXTI_REG2 | EXTI_EVENT | 0x02u)
#define EXTI_LINE_35                        (EXTI_RESERVED | EXTI_REG2              | 0x03u)
#define EXTI_LINE_36                        (EXTI_RESERVED | EXTI_REG2              | 0x04u)
#define EXTI_LINE_37                        (EXTI_CONFIG   | EXTI_REG2 | EXTI_EVENT | 0x05u)
#define EXTI_LINE_38                        (EXTI_CONFIG   | EXTI_REG2 | EXTI_EVENT | 0x06u)
#define EXTI_LINE_39                        (EXTI_RESERVED | EXTI_REG2              | 0x07u)
#define EXTI_LINE_40                        (EXTI_RESERVED | EXTI_REG2              | 0x08u)

#endif /* STM32L431xx */

#if defined(STM32L432xx) || defined(STM32L442xx)

#define EXTI_LINE_0                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x00u)
#define EXTI_LINE_1                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x01u)
#define EXTI_LINE_2                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x02u)
#define EXTI_LINE_3                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x03u)
#define EXTI_LINE_4                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x04u)
#define EXTI_LINE_5                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x05u)
#define EXTI_LINE_6                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x06u)
#define EXTI_LINE_7                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x07u)
#define EXTI_LINE_8                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x08u)
#define EXTI_LINE_9                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x09u)
#define EXTI_LINE_10                        (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x0Au)
#define EXTI_LINE_11                        (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x0Bu)
#define EXTI_LINE_12                        (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x0Cu)
#define EXTI_LINE_13                        (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x0Du)
#define EXTI_LINE_14                        (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x0Eu)
#define EXTI_LINE_15                        (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x0Fu)
#define EXTI_LINE_16                        (EXTI_CONFIG   | EXTI_REG1 | EXTI_EVENT | 0x10u)
#define EXTI_LINE_17                        (EXTI_DIRECT   | EXTI_REG1 | EXTI_EVENT | 0x11u)
#define EXTI_LINE_18                        (EXTI_CONFIG   | EXTI_REG1 | EXTI_EVENT | 0x12u)
#define EXTI_LINE_19                        (EXTI_CONFIG   | EXTI_REG1 | EXTI_EVENT | 0x13u)
#define EXTI_LINE_20                        (EXTI_CONFIG   | EXTI_REG1 | EXTI_EVENT | 0x14u)
#define EXTI_LINE_21                        (EXTI_CONFIG   | EXTI_REG1 | EXTI_EVENT | 0x15u)
#define EXTI_LINE_22                        (EXTI_CONFIG   | EXTI_REG1 | EXTI_EVENT | 0x16u)
#define EXTI_LINE_23                        (EXTI_DIRECT   | EXTI_REG1 | EXTI_EVENT | 0x17u)
#define EXTI_LINE_24                        (EXTI_RESERVED | EXTI_REG1              | 0x18u)
#define EXTI_LINE_25                        (EXTI_DIRECT   | EXTI_REG1 | EXTI_EVENT | 0x19u)
#define EXTI_LINE_26                        (EXTI_DIRECT   | EXTI_REG1 | EXTI_EVENT | 0x1Au)
#define EXTI_LINE_27                        (EXTI_DIRECT   | EXTI_REG1 | EXTI_EVENT | 0x1Bu)
#define EXTI_LINE_28                        (EXTI_RESERVED | EXTI_REG1              | 0x1Cu)
#define EXTI_LINE_29                        (EXTI_RESERVED | EXTI_REG1              | 0x1Du)
#define EXTI_LINE_30                        (EXTI_RESERVED | EXTI_REG1              | 0x1Eu)
#define EXTI_LINE_31                        (EXTI_DIRECT   | EXTI_REG1 | EXTI_EVENT | 0x1Fu)
#define EXTI_LINE_32                        (EXTI_DIRECT   | EXTI_REG2 | EXTI_EVENT | 0x00u)
#define EXTI_LINE_33                        (EXTI_DIRECT   | EXTI_REG2 | EXTI_EVENT | 0x01u)
#define EXTI_LINE_34                        (EXTI_DIRECT   | EXTI_REG2 | EXTI_EVENT | 0x02u)
#define EXTI_LINE_35                        (EXTI_CONFIG   | EXTI_REG2 | EXTI_EVENT | 0x03u)
#define EXTI_LINE_36                        (EXTI_RESERVED | EXTI_REG2              | 0x04u)
#define EXTI_LINE_37                        (EXTI_CONFIG   | EXTI_REG2 | EXTI_EVENT | 0x05u)
#define EXTI_LINE_38                        (EXTI_CONFIG   | EXTI_REG2 | EXTI_EVENT | 0x06u)
#define EXTI_LINE_39                        (EXTI_DIRECT   | EXTI_REG2 | EXTI_EVENT | 0x07u)
#define EXTI_LINE_40                        (EXTI_RESERVED | EXTI_REG2              | 0x08u)

#endif /* STM32L432xx || STM32L442xx */

#if defined(STM32L433xx) || defined(STM32L443xx)

#define EXTI_LINE_0                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x00u)
#define EXTI_LINE_1                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x01u)
#define EXTI_LINE_2                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x02u)
#define EXTI_LINE_3                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x03u)
#define EXTI_LINE_4                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x04u)
#define EXTI_LINE_5                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x05u)
#define EXTI_LINE_6                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x06u)
#define EXTI_LINE_7                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x07u)
#define EXTI_LINE_8                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x08u)
#define EXTI_LINE_9                         (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x09u)
#define EXTI_LINE_10                        (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x0Au)
#define EXTI_LINE_11                        (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x0Bu)
#define EXTI_LINE_12                        (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x0Cu)
#define EXTI_LINE_13                        (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x0Du)
#define EXTI_LINE_14                        (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x0Eu)
#define EXTI_LINE_15                        (EXTI_GPIO     | EXTI_REG1 | EXTI_EVENT | 0x0Fu)
#define EXTI_LINE_16                        (EXTI_CONFIG   | EXTI_REG1 | EXTI_EVENT | 0x10u)
#define EXTI_LINE_17                        (EXTI_DIRECT   | EXTI_REG1 | EXTI_EVENT | 0x11u)
#define EXTI_LINE_18                        (EXTI_CONFIG   | EXTI_REG1 | EXTI_EVENT | 0x12u)
#define EXTI_LINE_19                      