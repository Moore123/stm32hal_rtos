/**
  ******************************************************************************
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifndef STM32F103xB
    #define  STM32F103xB
#endif

#ifdef __cplusplus
 extern "C" {
#endif
//extern void _Error_Handler(char *, int);

#define EEPROM_CS_Pin GPIO_PIN_0
#define EEPROM_CS_GPIO_Port GPIOB
#define EEPROM_WP_Pin GPIO_PIN_1
#define EEPROM_WP_GPIO_Port GPIOB
#define EEPROM_HOLD_Pin GPIO_PIN_10
#define EEPROM_HOLD_GPIO_Port GPIOB
#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

#include "stm32f1xx_hal_conf.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_rtc.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_dma.h"
#include "stm32f1xx_ll_dma.h"

#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_hal_i2c.h"

//extern uint32_t SystemCoreClock;

//= 72000000U;

//#endif

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
