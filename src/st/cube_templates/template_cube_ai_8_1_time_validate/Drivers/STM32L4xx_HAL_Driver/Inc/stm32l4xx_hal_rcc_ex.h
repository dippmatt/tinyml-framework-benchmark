/**
  ******************************************************************************
  * @file    stm32l4xx_hal_rcc_ex.h
  * @author  MCD Application Team
  * @brief   Header file of RCC HAL Extended module.
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
#ifndef STM32L4xx_HAL_RCC_EX_H
#define STM32L4xx_HAL_RCC_EX_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal_def.h"

/** @addtogroup STM32L4xx_HAL_Driver
  * @{
  */

/** @addtogroup RCCEx
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup RCCEx_Exported_Types RCCEx Exported Types
  * @{
  */

#if defined(RCC_PLLSAI1_SUPPORT)
/**
  * @brief  PLLSAI1 Clock structure definition
  */
typedef struct
{

  uint32_t PLLSAI1Source;    /*!< PLLSAI1Source: PLLSAI1 entry clock source.
                                  This parameter must be a value of @ref RCC_PLL_Clock_Source */

#if defined(RCC_PLLSAI1M_DIV_1_16_SUPPORT)
  uint32_t PLLSAI1M;         /*!< PLLSAI1M: specifies the division factor for PLLSAI1 input clock.
                                  This parameter must be a number between Min_Data = 1 and Max_Data = 16 */
#else
  uint32_t PLLSAI1M;         /*!< PLLSAI1M: specifies the division factor for PLLSAI1 input clock.
                                  This parameter must be a number between Min_Data = 1 and Max_Data = 8 */
#endif

  uint32_t PLLSAI1N;         /*!< PLLSAI1N: specifies the multiplication factor for PLLSAI1 VCO output clock.
                                  This parameter must be a number between 8 and 86 or 127 depending on devices. */

  uint32_t PLLSAI1P;         /*!< PLLSAI1P: specifies the division factor for SAI clock.
                                  This parameter must be a value of @ref RCC_PLLP_Clock_Divider */

  uint32_t PLLSAI1Q;         /*!< PLLSAI1Q: specifies the division factor for USB/RNG/SDMMC1 clock.
                                  This parameter must be a value of @ref RCC_PLLQ_Clock_Divider */

  uint32_t PLLSAI1R;         /*!< PLLSAI1R: specifies the division factor for ADC clock.
                                  This parameter must be a value of @ref RCC_PLLR_Clock_Divider */

  uint32_t PLLSAI1ClockOut;  /*!< PLLSAIClockOut: specifies PLLSAI1 output clock to be enabled.
                                  This parameter must be a value of @ref RCC_PLLSAI1_Clock_Output */
}RCC_PLLSAI1InitTypeDef;
#endif /* RCC_PLLSAI1_SUPPORT */

#if defined(RCC_PLLSAI2_SUPPORT)
/**
  * @brief  PLLSAI2 Clock structure definition
  */
typedef struct
{

  uint32_t PLLSAI2Source;    /*!< PLLSAI2Source: PLLSAI2 entry clock source.
                                  This parameter must be a value of @ref RCC_PLL_Clock_Source */

#if defined(RCC_PLLSAI2M_DIV_1_16_SUPPORT)
  uint32_t PLLSAI2M;         /*!< PLLSAI2M: specifies the division factor for PLLSAI2 input clock.
                                  This parameter must be a number between Min_Data = 1 and Max_Data = 16 */
#else
  uint32_t PLLSAI2M;         /*!< PLLSAI2M: specifies the division factor for PLLSAI2 input clock.
                                  This parameter must be a number between Min_Data = 1 and Max_Data = 8 */
#endif

  uint32_t PLLSAI2N;         /*!< PLLSAI2N: specifies the multiplication factor for PLLSAI2 VCO output clock.
                                  This parameter must be a number between 8 and 86 or 127 depending on devices. */

  