/**
  ******************************************************************************
  * @file    stm32l4xx_hal_pwr_ex.c
  * @author  MCD Application Team
  * @brief   Extended PWR HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Power Controller (PWR) peripheral:
  *           + Extended Initialization and de-initialization functions
  *           + Extended Peripheral Control functions
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/** @addtogroup STM32L4xx_HAL_Driver
  * @{
  */

/** @defgroup PWREx PWREx
  * @brief PWR Extended HAL module driver
  * @{
  */

#ifdef HAL_PWR_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#if defined (STM32L412xx) || defined (STM32L422xx) || defined (STM32L431xx) || defined (STM32L432xx) || defined (STM32L433xx) || defined (STM32L442xx) || defined (STM32L443xx)
#define PWR_PORTH_AVAILABLE_PINS   ((uint32_t)0x0000000B) /* PH0/PH1/PH3 */
#elif defined (STM32L451xx) || defined (STM32L452xx) || defined (STM32L462xx)
#define PWR_PORTH_AVAILABLE_PINS   ((uint32_t)0x0000000B) /* PH0/PH1/PH3 */
#elif defined (STM32L471xx) || defined (STM32L475xx) || defined (STM32L476xx) || defined (STM32L485xx) || defined (STM32L486xx)
#define PWR_PORTH_AVAILABLE_PINS   ((uint32_t)0x00000003) /* PH0/PH1 */
#elif defined (STM32L496xx) || defined (STM32L4A6xx) || defined (STM32L4P5xx) || defined (STM32L4Q5xx) || defined (STM32L4R5xx) || defined (STM32L4R7xx) || defined (STM32L4R9xx) || defined (STM32L4S5xx) || defined (STM32L4S7xx) || defined (STM32L4S9xx)
#define PWR_PORTH_AVAILABLE_PINS   ((uint32_t)0x0000FFFF) /* PH0..PH15 */
#endif

#if defined (STM32L496xx) || defined (STM32L4A6xx) || defined (STM32L4P5xx) || defined (STM32L4Q5xx) || defined (STM32L4R5xx) || defined (STM32L4R7xx) || defined (STM32L4R9xx) || defined (STM32L4S5xx) || defined (STM32L4S7xx) || defined (STM32L4S9xx)
#define PWR_PORTI_AVAILABLE_PINS   ((uint32_t)0x00000FFF) /* PI0..PI11 */
#endif

/** @defgroup PWR_Extended_Private_Defines PWR Extended Private Defines
  * @{
  */

/** @defgroup PWREx_PVM_Mode_Mask PWR PVM Mode Mask
  * @{
  */
#define PVM_MODE_IT               ((uint32_t)0x00010000)  /*!< Mask for interruption yielded by PVM threshold crossing */
#define PVM_MODE_EVT              ((uint32_t)0x00020000)  /*!< Mask for event yielded by PVM threshold crossing        */
#define PVM_RISING_EDGE           ((uint32_t)0x00000001)  /*!< Mask for rising edge set as PVM trigger                 */
#define PVM_FALLING_EDGE          ((uint32_t)0x00000002)  /*!< Mask for falling edge set as PVM trigger                */
/**
  * @}
  */

/** @defgroup PWREx_TimeOut_Value PWR Extended Flag Setting Time Out Value
  * @{
  */
#define PWR_FLAG_SETTING_DELAY_US                      50UL   /*!< Time out value for REGLPF and VOSF flags setting */
/**
  * @}
  */



/**
  * @}
  */



/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup PWREx_Exported_Functions PWR Extended Exported Functions
  * @{
  */

/** @defgroup PWREx_Exported_Functions_Group1 Extended Peripheral Control functions
  *  @brief   Extended Peripheral Control functions
  *
@verbatim
 ===============================================================================
              ##### Extended Peripheral Initialization and de-initialization functions #####
 ===============================================================================
    [..]

@endverbatim
  * @{
  */


/**
  * @brief Return Voltage Scaling Range.
  * @retval VOS bit field (PWR_REGULATOR_VOLTAGE_SCALE1 or PWR_REGULATOR_VOLTAGE_SCALE2
  *         or PWR_REGULATOR_VOLTAGE_SCALE1_BOOST when applicable)
  */
uint32_t HAL_PWREx_GetVoltageRange(void)
{
#if defined(PWR_CR5_R1MODE)
    if (READ_BIT(PWR->CR1, PWR_CR1_VOS) == PWR_REGULATOR_VOLTAGE_SCALE2)
    {
      return PWR_REGULATOR_VOLTAGE_SCALE2;
    }
    else if (READ_BIT(PWR->CR5, PWR_CR5_R1MODE) == PWR_CR5_R1MODE)
    {
      /* PWR_CR5_R1MODE bit set means that Range 1 Boost is disabled */
      return PWR_REGULATOR_VOLTAGE_SCALE1;
    }
    else
    {
      return PWR_REGULATOR_VOLTAGE_SCALE1_BOOST;
    }
#else
  return  (PWR->CR1 & PWR_CR1_VOS);
#endif
}



/**
  * @brief Configure the main internal regulator output voltage.
  * @param  VoltageScaling specifies the regulator output voltage to achieve
  *         a tradeoff between performance and power consumption.
  *          This parameter can be one of the following values:
  @if STM32L4S9xx
  *            @arg @ref PWR_REGULATOR_VOLTAGE_SCALE1_BOOST when available, Regulator voltage output range 1 boost mode,
  *                                                typical output voltage at 1.2 V,
  *                                                system frequency up to 120 MHz.
  @endif
  *            @arg @ref PWR_REGULATOR_VOLTAGE_SCALE1 Regulator voltage output range 1 mode,
  *                                                typical output voltage at 1.2 V,
  *                                                system frequency up to 80 MHz.
  *            @arg @ref PWR_REGULATOR_VOLTAGE_SCALE2 Regulator voltage output range 2 mode,
  *                                                typical output voltage at 1.0 V,
  *                                                system frequency up to 26 MHz.
  * @note  When moving from Range 1 to Range 2, the system frequency must be decreased to
  *        a value below 26 MHz before calling HAL_PWREx_ControlVoltageScaling() API.
  *        When moving from Range 2 to Range 1, the system frequency can be increased to
  *        a value up to 80 MHz after calling HAL_PWREx_ControlVoltageScaling() API. For
  *        some devices, the system frequency can be increased up to 120 MHz.
  * @note  When moving from Range 2 to Range 1, the API waits for VOSF flag to be
  *        cleared before returning the status. If the flag is not cleared within
  *        50 microseconds, HAL_TIMEOUT status is reported.
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_PWREx_ControlVoltageScaling(uint32_t VoltageScaling)
{
  uint32_t wait_loop_index;

  assert_param(IS_PWR_VOLTAGE_SCALING_RANGE(VoltageScaling));

#if defined(PWR_CR5_R1MODE)
  if (VoltageScaling == PWR_REGULATOR_VOLTAGE_SCALE1_BOOST)
  {
    /* If current range is range 2 */
    if (READ_BIT(PWR->CR1, PWR_CR1_VOS) == PWR_REGULATOR_VOLTAGE_SCALE2)
    {
      /* Make sure Range 1 Boost is enabled */
      CLEAR_BIT(PWR->CR5, PWR_CR5_R1MODE);

      /* Set Range 1 */
      MODIFY_REG(PWR->CR1, PWR_CR1_VOS, PWR_REGULATOR_VOLTAGE_SCALE1);

      /* Wait until VOSF is cleared */
      wait_loop_index = ((PWR_FLAG_SETTING_DELAY_US * SystemCoreClock) / 1000000U) + 1;
      while ((HAL_IS_BIT_SET(PWR->SR2, PWR_SR2_VOSF)) && (wait_loop_index != 0U))
      {
        wait_loop_index--;
      }
      if (HAL_IS_BIT_SET(PWR->SR2, PWR_SR2_VOSF))
      {
        return HAL_TIMEOUT;
      }
    }
    /* If current range is range 1 normal or boost mode */
    else
    {
      /* Enable Range 1 Boost (no issue if bit already reset) */
      CLEAR_BIT(PWR->CR5, PWR_CR5_R1MODE);
    }
  }
  else if (VoltageScaling == PWR_REGULATOR_VOLTAGE_SCALE1)
  {
    /* If current range is range 2 */
    if (READ_BIT(PWR->CR1, PWR_CR1_VOS) == PWR_REGULATOR_VOLTAGE_SCALE2)
    {
      /* Make sure Range 1 Boost is disabled */
      SET_BIT(PWR->CR5, PWR_CR5_R1MODE);

      /* Set Range 1 */
      MODIFY_REG(PWR->CR1, PWR_CR1_VOS, PWR_REGULATOR_VOLTAGE_SCALE1);

      /* Wait until VOSF is cleared */
      wait_loop_index = ((PWR_FLAG_SETTING_DELAY_US * SystemCoreClock) / 1000000U) + 1;
      while ((HAL_IS_BIT_SET(PWR->SR2, PWR_SR2_VOSF)) && (wait_loop_index != 0U))
      {
        wait_loop_index--;
      }
      if (HAL_IS_BIT_SET(PWR->SR2, PWR_SR2_VOSF))
      {
        return HAL_TIMEOUT;
      }
    }
     /* If current range is range 1 normal or boost mode */
    else
    {
      /* Disable Range 1 Boost (no issue if bit already set) */
      SET_BIT(PWR->CR5, PWR_CR5_R1MODE);
    }
  }
  else
  {
    /* Set Range 2 */
    MODIFY_REG(PWR->CR1, PWR_CR1_VOS, PWR_REGULATOR_VOLTAGE_SCALE2);
    /* No need to wait for VOSF to be cleared for this transition */
    /* PWR_CR5_R1MODE bit setting has no effect in Range 2        */
  }

#else

  /* If Set Range 1 */
  if (VoltageScaling == PWR_REGULATOR_VOLTAGE_SCALE1)
  {
    if (READ_BIT(PWR->CR1, PWR_CR1_VOS) != PWR_REGULATOR_VOLTAGE_SCALE1)
    {
      /* Set Range 1 */
      MODIFY_REG(PWR->CR1, PWR_CR1_VOS, PWR_REGULATOR_VOLTAGE_SCALE1);

      /* Wait until VOSF is cleared */
      wait_loop_index = ((PWR_FLAG_SETTING_DELAY_US * SystemCoreClock) / 1000000U) + 1U;
      while ((HAL_IS_BIT_SET(PWR->SR2, PWR_SR2_VOSF)) && (wait_loop_index != 0U))
      {
        wait_loop_index--;
      }
      if (HAL_IS_BIT_SET(PWR->SR2, PWR_SR2_VOSF))
      {
        return HAL_TIMEOUT;
      }
    }
  }
  else
  {
    if (READ_BIT(PWR->CR1, PWR_CR1_VOS) != PWR_REGULATOR_VOLTAGE_SCALE2)
    {
      /* Set Range 2 */
      MODIFY_REG(PWR->CR1, PWR_CR1_VOS, PWR_REGULATOR_VOLTAGE_SCALE2);
      /* No need to wait for VOSF to be cleared for this transition */
    }
  }
#endif

  return HAL_OK;
}


/**
  * @brief Enable battery charging.
  *        When VDD is present, charge the external battery on VBAT thru an internal resistor.
  * @param  ResistorSelection specifies the resistor impedance.
  *          This parameter can be one of the following values:
  *            @arg @ref PWR_BATTERY_CHARGING_RESISTOR_5     5 kOhms resistor
  *            @arg @ref PWR_BATTERY_CHARGING_RESISTOR_1_5 1.5 kOhms resistor
  * @retval None
  */
void HAL_PWREx_EnableBatteryCharging(uint32_t ResistorSelection)
{
  assert_param(IS_PWR_BATTERY_RESISTOR_SELECT(ResistorSelection));

  /* Specify resistor selection */
  MODIFY_REG(PWR->CR4, PWR_CR4_VBRS, ResistorSelection);

  /* Enable battery charging */
  SET_BIT(PWR->CR4, PWR_CR4_VBE);
}


/**
  * @brief Disable battery charging.
  * @retval None
  */
void HAL_PWREx_DisableBatteryCharging(void)
{
  CLEAR_BIT(PWR->CR4, PWR_CR4_VBE);
}


#if defined(PWR_CR2_USV)
/**
  * @brief Enable VDDUSB supply.
  * @note  Remove VDDUSB electrical and logical isolation, once VDDUSB supply is present.
  * @retval None
  */
void HAL_PWREx_EnableVddUSB(void)
{
  SET_BIT(PWR->CR2, PWR_CR2_USV);
}


/**
  * @brief Disable VDDUSB supply.
  * @retval None
  */
void HAL_PWREx_DisableVddUSB(void)
{
  CLEAR_BIT(PWR->CR2, PWR_CR2_USV);
}
#endif /* PWR_CR2_USV */

#if defined(PWR_CR2_IOSV)
/**
  * @brief Enable VDDIO2 supply.
  * @note  Remove VDDIO2 electrical and logical isolation, once VDDIO2 supply is present.
  * @retval None
  */
void HAL_PWREx_EnableVddIO2(void)
{
  SET_BIT(PWR->CR2, PWR_CR2_IOSV);
}


/**
  * @brief Disable VDDIO2 supply.
  * @retval None
  */
void HAL_PWREx_DisableVddIO2(void)
{
  CLEAR_BIT(PWR->CR2, PWR_CR2_IOSV);
}
#endif /* PWR_CR2_IOSV */


/**
  * @brief Enable Internal Wake-up Line.
  * @retval None
  */
void HAL_PWREx_EnableInternalWakeUpLine(void)
{
  SET_BIT(PWR->CR3, PWR_CR3_EIWF);
}


/**
  * @brief Disable Internal Wake-up Line.
  * @retval None
  */
void HAL_PWREx_DisableInternalWakeUpLine(void)
{
  CLEAR_BIT(PWR->CR3, PWR_CR3_EIWF);
}



/**
  * @brief Enable GPIO pull-up state in Standby and Shutdown modes.
  * @note  Set the relevant PUy bits of PWR_PUCRx register to configure the I/O in
  *        pull-up state in Standby and Shutdown modes.
  * @note  This state is effective in Standby and Shutdown modes only if APC bit
  *        is set through HAL_PWREx_EnablePullUpPullDownConfig() API.
  * @note  The configuration is lost when exiting the Shutdown mode due to the
  *        power-on reset, maintained when exiting the Standby mode.
  * @note  To avoid any conflict at Standby and Shutdown modes exits, the corresponding
  *        PDy bit of PWR_PDCRx register is cleared unless it is reserved.
  * @note  Even if a PUy bit to set is reserved, the other PUy bits entered as input
  *        parameter at the same time are set.
  * @param  GPIO Specify the IO port. This parameter can be PWR_GPIO_A, ..., PWR_GPIO_H
  *         (or PWR_GPIO_I depending on the devices) to select the GPIO peripheral.
  * @param  GPIONumber Specify the I/O pins numbers.
  *         This parameter can be one of the following values:
  *         PWR_GPIO_BIT_0, ..., PWR_GPIO_BIT_15 (except for the port where less
  *         I/O pins are available) or the logical OR of several of them to set
  *         several bits for a given port in a single API call.
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_PWREx_EnableGPIOPullUp(uint32_t GPIO, uint32_t GPIONumber)
{
  HAL_StatusTypeDef status = HAL_OK;

  assert_param(IS_PWR_GPIO(GPIO));
  assert_param(IS_PWR_GPIO_BIT_NUMBER(GPIONumber));

  switch (GPIO)
  {
    case PWR_GPIO_A:
       SET_BIT(PWR->PUCRA, (GPIONumber & (~(PWR_GPIO_BIT_14))));
       CLEAR_BIT(PWR->PDCRA, (GPIONumber & (~(PWR_GPIO_BIT_13|PWR_GPIO_BIT_15))));
       break;
    case PWR_GPIO_B:
       SET_BIT(PWR->PUCRB, GPIONumber);
       CLEAR_BIT(PWR->PDCRB, (GPIONumber & (~(PWR_GPIO_BIT_4))));
       break;
    case PWR_GPIO_C:
       SET_BIT(PWR->PUCRC, GPIONumber);
       CLEAR_BIT(PWR->PDCRC, GPIONumber);
       break;
#if defined(GPIOD)
    case PWR_GPIO_D:
       SET_BIT(PWR->PUCRD, GPIONumber);
       CLEAR_BIT(PWR->PDCRD, GPIONumber);
       break;
#endif
#if defined(GPIOE)
    case PWR_GPIO_E:
       SET_BIT(PWR->PUCRE, GPIONumber);
       CLEAR_BIT(PWR->PDCRE, GPIONumber);
       break;
#endif
#if defined(GPIOF)
    case PWR_GPIO_F:
       SET_BIT(PWR->PUCRF, GPIONumber);
       CLEAR_BIT(PWR->PDCRF, GPIONumber);
       break;
#endif
#if defined(GPIOG)
    case PWR_GPIO_G:
       SET_BIT(PWR->PUCRG, GPIONumber);
       CLEAR_BIT(PWR->PDCRG, GPIONumber);
       break;
#endif
    case PWR_GPIO_H:
       SET_BIT(PWR->PUCRH, (GPIONumber & PWR_PORTH_AVAILABLE_PINS));
#if defined (STM32L496xx) || defined (STM32L4A6xx)
       CLEAR_BIT(PWR->PDCRH, ((GPIONumber & PWR_PORTH_AVAILABLE_PINS) & (~(PWR_GPIO_BIT_3))));
#else
       CLEAR_BIT(PWR->PDCRH, (GPIONumber & PWR_PORTH_AVAILABLE_PINS));
#endif
       break;
#if defined(GPIOI)
    case PWR_GPIO_I:
       SET_BIT(PWR->PUCRI, (GPIONumber & PWR_PORTI_AVAILABLE_PINS));
       CLEAR_BIT(PWR->PDCRI, (GPIONumber & PWR_PORTI_AVAILABLE_PINS));
       break;
#endif
    default:
      status = HAL_ERROR;
      break;
  }

  return status;
}


/**
  * @brief Disable GPIO pull-up state in Standby mode and Shutdown modes.
  * @note  Reset the relevant PUy bits of PWR_PUCRx register used to configure the I/O
  *        in pull-up state in Standby and Shutdown modes.
  * @note  Even if a PUy bit to reset is reserved, the other PUy bits entered as input
  *        parameter at the same time are reset.
  * @param  GPIO Specifies the IO port. This parameter can be PWR_GPIO_A, ..., PWR_GPIO_H
  *          (or PWR_GPIO_I depending on the devices) to select the GPIO peripheral.
  * @param  GPIONumber Specify the I/O pins numbers.
  *        