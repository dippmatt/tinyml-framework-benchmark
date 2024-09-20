/**
  ******************************************************************************
  * @file    stm32l4xx_hal_rcc.c
  * @author  MCD Application Team
  * @brief   RCC HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Reset and Clock Control (RCC) peripheral:
  *           + Initialization and de-initialization functions
  *           + Peripheral Control functions
  *
  @verbatim
  ==============================================================================
                      ##### RCC specific features #####
  ==============================================================================
    [..]
      After reset the device is running from Multiple Speed Internal oscillator
      (4 MHz) with Flash 0 wait state. Flash prefetch buffer, D-Cache
      and I-Cache are disabled, and all peripherals are off except internal
      SRAM, Flash and JTAG.

      (+) There is no prescaler on High speed (AHBs) and Low speed (APBs) busses:
          all peripherals mapped on these busses are running at MSI speed.
      (+) The clock for all peripherals is switched off, except the SRAM and FLASH.
      (+) All GPIOs are in analog mode, except the JTAG pins which
          are assigned to be used for debug purpose.

    [..]
      Once the device started from reset, the user application has to:
      (+) Configure the clock source to be used to drive the System clock
          (if the application needs higher frequency/performance)
      (+) Configure the System clock frequency and Flash settings
      (+) Configure the AHB and APB busses prescalers
      (+) Enable the clock for the peripheral(s) to be used
      (+) Configure the clock source(s) for peripherals which clocks are not
          derived from the System clock (SAIx, RTC, ADC, USB OTG FS/SDMMC1/RNG)

  @endverbatim
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

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/** @addtogroup STM32L4xx_HAL_Driver
  * @{
  */

/** @defgroup RCC RCC
  * @brief RCC HAL module driver
  * @{
  */

#ifdef HAL_RCC_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup RCC_Private_Constants RCC Private Constants
 * @{
 */
#define HSE_TIMEOUT_VALUE          HSE_STARTUP_TIMEOUT
#define HSI_TIMEOUT_VALUE          2U    /* 2 ms (minimum Tick + 1) */
#define MSI_TIMEOUT_VALUE          2U    /* 2 ms (minimum Tick + 1) */
#if defined(RCC_CSR_LSIPREDIV)
#define LSI_TIMEOUT_VALUE          17U   /* 17 ms (16 ms starting time + 1) */
#else
#define LSI_TIMEOUT_VALUE          2U    /* 2 ms (minimum Tick + 1) */
#endif /* RCC_CSR_LSIPREDIV */
#define HSI48_TIMEOUT_VALUE        2U    /* 2 ms (minimum Tick + 1) */
#define PLL_TIMEOUT_VALUE          2U    /* 2 ms (minimum Tick + 1) */
#define CLOCKSWITCH_TIMEOUT_VALUE  5000U /* 5 s    */
/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/** @defgroup RCC_Private_Macros RCC Private Macros
  * @{
  */
#define __MCO1_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define MCO1_GPIO_PORT        GPIOA
#define MCO1_PIN              GPIO_PIN_8

#define RCC_PLL_OSCSOURCE_CONFIG(__HAL_RCC_PLLSOURCE__) \
            (MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC, (__HAL_RCC_PLLSOURCE__)))
/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/** @defgroup RCC_Private_Functions RCC Private Functions
  * @{
  */
static HAL_StatusTypeDef RCC_SetFlashLatencyFromMSIRange(uint32_t msirange);
#if defined(STM32L4P5xx) || defined(STM32L4Q5xx) || \
    defined(STM32L4R5xx) || defined(STM32L4R7xx) || defined(STM32L4R9xx) || defined(STM32L4S5xx) || defined(STM32L4S7xx) || defined(STM32L4S9xx)
static uint32_t          RCC_GetSysClockFreqFromPLLSource(void);
#endif
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/** @defgroup RCC_Exported_Functions RCC Exported Functions
  * @{
  */

/** @defgroup RCC_Exported_Functions_Group1 Initialization and de-initialization functions
  *  @brief    Initialization and Configuration functions
  *
  @verbatim
 ===============================================================================
           ##### Initialization and de-initialization functions #####
 ===============================================================================
    [..]
      This section provides functions allowing to configure the internal and external oscillators
      (HSE, HSI, LSE, MSI, LSI, PLL, CSS and MCO) and the System busses clocks (SYSCLK, AHB, APB1
       and APB2).

    [..] Internal/external clock and PLL configuration
         (+) HSI (high-speed internal): 16 MHz factory-trimmed RC used directly or through
             the PLL as System clock source.

         (+) MSI (Multiple Speed Internal): Its frequency is software trimmable from 100KHZ to 48MHZ.
             It can be used to generate the clock for the USB OTG FS (48 MHz).
             The number of flash wait states is automatically adjusted when MSI range is updated with
             HAL_RCC_OscConfig() and the MSI is used as System clock source.

         (+) LSI (low-speed internal): 32 KHz low consumption RC used as IWDG and/or RTC
             clock source.

         (+) HSE (high-speed external): 4 to 48 MHz crystal oscillator used directly or
             through the PLL as System clock source. Can be used also optionally as RTC clock source.

         (+) LSE (low-speed external): 32.768 KHz oscillator used optionally as RTC clock source.

         (+) PLL (clocked by HSI, HSE or MSI) providing up to three independent output clocks:
           (++) The first output is used to generate the high speed system clock (up to 80MHz).
           (++) The second output is used to generate the clock for the USB OTG FS (48 MHz),
                the random analog generator (<=48 MHz) and the SDMMC1 (<= 48 MHz).
           (++) The third output is used to generate an accurate clock to achieve
                high-quality audio performance on SAI interface.

         (+) PLLSAI1 (clocked by HSI, HSE or MSI) providing up to three independent output clocks:
           (++) The first output is used to generate SAR ADC1 clock.
           (++) The second output is used to generate the clock for the USB OTG FS (48 MHz),
                the random analog generator (<=48 MHz) and the SDMMC1 (<= 48 MHz).
           (++) The third output is used to generate an accurate clock to achieve
                high-quality audio performance on SAI interface.

         (+) PLLSAI2 (clocked by HSI, HSE or MSI) providing up to three independent output clocks:
           (++) The first output is used to generate an accurate clock to achieve
                high-quality audio performance on SAI interface.
           (++) The second output is used to generate either SAR ADC2 clock if ADC2 is present
                or LCD clock if LTDC is present.
           (++) The third output is used to generate DSI clock if DSI is present.

         (+) CSS (Clock security system): once enabled, if a HSE clock failure occurs
            (HSE used directly or through PLL as System clock source), the System clock
             is automatically switched to HSI and an interrupt is generated if enabled.
             The interrupt is linked to the Cortex-M4 NMI (Non-Maskable Interrupt)
             exception vector.

         (+) MCO (microcontroller clock output): used to output MSI, LSI, HSI, LSE, HSE or
             main PLL clock (through a configurable prescaler) on PA8 pin.

    [..] System, AHB and APB busses clocks configuration
         (+) Several clock sources can be used to drive the System clock (SYSCLK): MSI, HSI,
             HSE and main PLL.
             The AHB clock (HCLK) is derived from System clock through configurable
             prescaler and used to clock the CPU, memory and peripherals mapped
             on AHB bus (DMA, GPIO...). APB1 (PCLK1) and APB2 (PCLK2) clocks are derived
             from AHB clock through configurable prescalers and used to clock
             the peripherals mapped on these busses. You can use
             "HAL_RCC_GetSysClockFreq()" function to retrieve the frequencies of these clocks.

         -@- All the peripheral clocks are derived from the System clock (SYSCLK) except:

           (+@) SAI: the SAI clock can be derived either from a specific PLL (PLLSAI1) or (PLLSAI2) or
                from an external clock mapped on the SAI_CKIN pin.
                You have to use HAL_RCCEx_PeriphCLKConfig() function to configure this clock.
           (+@) RTC: the RTC clock can be derived either from the LSI, LSE or HSE clock
                divided by 2 to 31.
                You have to use __HAL_RCC_RTC_ENABLE() and HAL_RCCEx_PeriphCLKConfig() function
                to configure this clock.
           (+@) USB OTG FS, SDMMC1 and RNG: USB OTG FS requires a frequency equal to 48 MHz
                to work correctly, while the SDMMC1 and RNG peripherals require a frequency
                equal or lower than to 48 MHz. This clock is derived of the main PLL or PLLSAI1
                through PLLQ divider. You have to enable the peripheral clock and use
                HAL_RCCEx_PeriphCLKConfig() function to configure this clock.
           (+@) IWDG clock which is always the LSI clock.


         (+) The maximum frequency of the SYSCLK, HCLK, PCLK1 and PCLK2 is 80 MHz.
             The clock source frequency should be adapted depending on the device voltage range
             as listed in the Reference Manual "Clock source frequency versus voltage scaling" chapter.

  @endverbatim

           Table 1. HCLK clock frequency for other STM32L4 devices
           +-------------------------------------------------------+
           | Latency         |    HCLK clock frequency (MHz)       |
           |                 |-------------------------------------|
           |                 | voltage range 1  | voltage range 2  |
           |                 |      1.2 V       |     1.0 V        |
           |-----------------|------------------|------------------|
           |0WS(1 CPU cycles)|  0 < HCLK <= 16  |  0 < HCLK <= 6   |
           |-----------------|------------------|------------------|
           |1WS(2 CPU cycles)| 16 < HCLK <= 32  |  6 < HCLK <= 12  |
           |-----------------|------------------|------------------|
           |2WS(3 CPU cycles)| 32 < HCLK <= 48  | 12 < HCLK <= 18  |
           |-----------------|------------------|------------------|
           |3WS(4 CPU cycles)| 48 < HCLK <= 64  | 18 < HCLK <= 26  |
           |-----------------|------------------|------------------|
           |4WS(5 CPU cycles)| 64 < HCLK <= 80  | 18 < HCLK <= 26  |
           +-------------------------------------------------------+

           Table 2. HCLK clock frequency for STM32L4+ devices
           +--------------------------------------------------------+
           | Latency         |     HCLK clock frequency (MHz)       |
           |                 |--------------------------------------|
           |                 |  voltage range 1  | voltage range 2  |
           |                 |       1.2 V       |     1.0 V        |
           |-----------------|-------------------|------------------|
           |0WS(1 CPU cycles)|   0 < HCLK <= 20  |  0 < HCLK <= 8   |
           |-----------------|-------------------|------------------|
           |1WS(2 CPU cycles)|  20 < HCLK <= 40  |  8 < HCLK <= 16  |
           |-----------------|-------------------|------------------|
           |2WS(3 CPU cycles)|  40 < HCLK <= 60  | 16 < HCLK <= 26  |
           |-----------------|-------------------|------------------|
           |3WS(4 CPU cycles)|  60 < HCLK <= 80  | 16 < HCLK <= 26  |
           |-----------------|-------------------|------------------|
           |4WS(5 CPU cycles)|  80 < HCLK <= 100 | 16 < HCLK <= 26  |
           |-----------------|-------------------|------------------|
           |5WS(6 CPU cycles)| 100 < HCLK <= 120 | 16 < HCLK <= 26  |
           +--------------------------------------------------------+
  * @{
  */

/**
  * @brief  Reset the RCC clock configuration to the default reset state.
  * @note   The default reset state of the clock configuration is given below:
  *            - MSI ON and used as system clock source
  *            - HSE, HSI, PLL, PLLSAI1 and PLLSAI2 OFF
  *            - AHB, APB1 and APB2 prescalers set to 1.
  *            - CSS, MCO1 OFF
  *            - All interrupts disabled
  *            - All interrupt and reset flags cleared
  * @note   This function does not modify the configuration of the
  *            - Peripheral clock sources
  *            - LSI, LSE and RTC clocks (Backup domain)
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCC_DeInit(void)
{
  uint32_t tickstart;

  /* Reset to default System clock */
  /* Set MSION bit */
  SET_BIT(RCC->CR, RCC_CR_MSION);

  /* Insure MSIRDY bit is set before writing default MSIRANGE value */
  /* Get start tick */
  tickstart = HAL_GetTick();

  /* Wait till MSI is ready */
  while(READ_BIT(RCC->CR, RCC_CR_MSIRDY) == 0U)
  {
    if((HAL_GetTick() - tickstart) > MSI_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  /* Set MSIRANGE default value */
  MODIFY_REG(RCC->CR, RCC_CR_MSIRANGE, RCC_MSIRANGE_6);

  /* Reset CFGR register (MSI is selected as system clock source) */
  CLEAR_REG(RCC->CFGR);

  /* Update the SystemCoreClock global variable for MSI as system clock source */
  SystemCoreClock = MSI_VALUE;

  /* Configure the source of time base considering new system clock settings  */
  if(HAL_InitTick(uwTickPrio) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Insure MSI selected as system clock source */
  /* Get start tick */
  tickstart = HAL_GetTick();

  /* Wait till system clock source is ready */
  while(READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RCC_CFGR_SWS_MSI)
  {
    if((HAL_GetTick() - tickstart) > CLOCKSWITCH_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  /* Reset HSION, HSIKERON, HSIASFS, HSEON, HSECSSON, PLLON, PLLSAIxON bits */
#if defined(RCC_PLLSAI2_SUPPORT)

  CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_HSION | RCC_CR_HSIKERON| RCC_CR_HSIASFS | RCC_CR_PLLON | RCC_CR_PLLSAI1ON | RCC_CR_PLLSAI2ON);

#elif defined(RCC_PLLSAI1_SUPPORT)

  CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_HSION | RCC_CR_HSIKERON| RCC_CR_HSIASFS | RCC_CR_PLLON | RCC_CR_PLLSAI1ON);

#else

  CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_HSION | RCC_CR_HSIKERON| RCC_CR_HSIASFS | RCC_CR_PLLON);

#endif /* RCC_PLLSAI2_SUPPORT */

  /* Insure PLLRDY, PLLSAI1RDY and PLLSAI2RDY (if present) are reset */
  /* Get start tick */
  tickstart = HAL_GetTick();

#if defined(RCC_PLLSAI2_SUPPORT)

  while(READ_BIT(RCC->CR, RCC_CR_PLLRDY | RCC_CR_PLLSAI1RDY | RCC_CR_PLLSAI2RDY) != 0U)

#elif defined(RCC_PLLSAI1_SUPPORT)

  while(READ_BIT(RCC->CR, RCC_CR_PLLRDY | RCC_CR_PLLSAI1RDY) != 0U)

#else

  while(READ_BIT(RCC->CR, RCC_CR_PLLRDY) != 0U)

#endif
  {
    if((HAL_GetTick() - tickstart) > PLL_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  /* Reset PLLCFGR register */
  CLEAR_REG(RCC->PLLCFGR);
  SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLN_4 );

#if defined(RCC_PLLSAI1_SUPPORT)

  /* Reset PLLSAI1CFGR register */
  CLEAR_REG(RCC->PLLSAI1CFGR);
  SET_BIT(RCC->PLLSAI1CFGR,  RCC_PLLSAI1CFGR_PLLSAI1N_4 );

#endif /* RCC_PLLSAI1_SUPPORT */

#if defined(RCC_PLLSAI2_SUPPORT)

  /* Reset PLLSAI2CFGR register */
  CLEAR_REG(RCC->PLLSAI2CFGR);
  SET_BIT(RCC->PLLSAI2CFGR,  RCC_PLLSAI2CFGR_PLLSAI2N_4 );

#endif /* RCC_PLLSAI2_SUPPORT */

  /* Reset HSEBYP bit */
  CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);

  /* Disable all interrupts */
  CLEAR_REG(RCC->CIER);

  /* Clear all interrupt flags */
  WRITE_REG(RCC->CICR, 0xFFFFFFFFU);

  /* Clear all reset flags */
  SET_BIT(RCC->CSR, RCC_CSR_RMVF);

  return HAL_OK;
}

/**
  * @brief  Initialize the RCC Oscillators according to the specified parameters in the
  *         RCC_OscInitTypeDef.
  * @param  RCC_OscInitStruct  pointer to an RCC_OscInitTypeDef structure that
  *         contains the configuration information for the RCC Oscillators.
  * @note   The PLL is not disabled when used as system clock.
  * @note   The PLL source is not updated when used as PLLSAI(s) clock source.
  * @note   Transitions LSE Bypass to LSE On and LSE On to LSE Bypass are not
  *         supported by this macro. User should request a transition to LSE Off
  *         first and then LSE On or LSE Bypass.
  * @note   Transition HSE Bypass to HSE On and HSE On to HSE Bypass are not
  *         supported by this macro. User should request a transition to HSE Off
  *         first and then HSE On or HSE Bypass.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RCC_OscConfig(RCC_OscInitTypeDef  *RCC_OscInitStruct)
{
  uint32_t tickstart;
  HAL_StatusTypeDef status;
  uint32_t sysclk_source, pll_config;

  /* Check Null pointer */
  if(RCC_OscInitStruct == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_RCC_OSCILLATORTYPE(RCC_OscInitStruct->OscillatorType));

  sysclk_source = __HAL_RCC_GET_SYSCLK_SOURCE();
  pll_config = __HAL_RCC_GET_PLL_OSCSOURCE();

  /*----------------------------- MSI Configuration --------------------------*/
  if(((RCC_OscInitStruct->OscillatorType) & RCC_OSCILLATORTYPE_MSI) == RCC_OSCILLATORTYPE_MSI)
  {
    /* Check the parameters */
    assert_param(IS_RCC_MSI(RCC_OscInitStruct->MSIState));
    assert_param(IS_RCC_MSICALIBRATION_VALUE(RCC_OscInitStruct->MSICalibrationValue));
    assert_param(IS_RCC_MSI_CLOCK_RANGE(RCC_OscInitStruct->MSIClockRange));

    /* Check if MSI is used as system clock or as PLL source when PLL is selected as system clock */
    if((sysclk_source == RCC_CFGR_SWS_MSI) ||
       ((sysclk_source == RCC_CFGR_SWS_PLL) && (pll_config == RCC_PLLSOURCE_MSI)))
    {
      if((READ_BIT(RCC->CR, RCC_CR_MSIRDY) != 0U) && (RCC_OscInitStruct->MSIState == RCC_MSI_OFF))
      {
        return HAL_ERROR;
      }

       /* Otherwise, just the calibration and MSI range change are allowed */
      else
      {
        /* To correctly read data from FLASH memory, the number of wait states (LATENCY)
           must be correctly programmed according to the frequency of the CPU clock
           (HCLK) and the supply voltage of the device. */
        if(RCC_OscInitStruct->MSIClockRange > __HAL_RCC_GET_MSI_RANGE())
        {
          /* First increase number of wait states update if necessary */
          if(RCC_SetFlashLatencyFromMSIRange(RCC_OscInitStruct->MSIClockRange) != HAL_OK)
          {
            return HAL_ERROR;
          }

          /* Selects the Multiple Speed oscillator (MSI) clock range .*/
          __HAL_RCC_MSI_RANGE_CONFIG(RCC_OscInitStruct->MSIClockRange);
          /* Adjusts the Multiple Speed oscillator (MSI) calibration value.*/
          __HAL_RCC_MSI_CALIBRATIONVALUE_ADJUST(RCC_OscInitStruct->MSICalibrationValue);
        }
        else
        {
          /* Else, keep current flash latency while decreasing applies */
          /* Selects the Multiple Speed oscillator (MSI) clock range .*/
          __HAL_RCC_MSI_RANGE_CONFIG(RCC_OscInitStruct->MSIClockRange);
          /* Adjusts the Multiple Speed oscillator (MSI) calibration value.*/
          __HAL_RCC_MSI