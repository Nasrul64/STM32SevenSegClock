/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SS_CLK_Pin GPIO_PIN_0
#define SS_CLK_GPIO_Port GPIOF
#define SS_DIO_Pin GPIO_PIN_1
#define SS_DIO_GPIO_Port GPIOF
#define BTN1_Pin GPIO_PIN_0
#define BTN1_GPIO_Port GPIOA
#define BTN2_Pin GPIO_PIN_1
#define BTN2_GPIO_Port GPIOA
#define REM_TX_Pin GPIO_PIN_2
#define REM_TX_GPIO_Port GPIOA
#define REM_RX_Pin GPIO_PIN_3
#define REM_RX_GPIO_Port GPIOA
#define RTC_SCL_Pin GPIO_PIN_9
#define RTC_SCL_GPIO_Port GPIOA
#define RTC_SDA_Pin GPIO_PIN_10
#define RTC_SDA_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
