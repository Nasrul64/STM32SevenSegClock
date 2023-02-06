/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "TM1637.h"
#include "DS3231.h"
#include <string.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
  TIME timeNow;
  float fTemp;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void printRem(char*);
void debugTrace();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  tm1637_Init();

  tm1637_SetBrightness(1); // Optionally set brightness. 0 is off. By default, initialized to full brightness.

  tm1637_DisplayDecimal(1234, 1); // Display the value "1234" and turn on the `:` that is between digits 2 and 3.
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  int nHour, nMin;
  uint8_t bColon;

  enum StateType
  {
	  stInit = 0,
	  stDisplayTime,
	  stUpdateTime,
	  stSetTime,
	  stDisplayTemp,
	  stErr
  };

  int nStateTime = 0;
  enum StateType nCurState = 0;
  enum StateType nNextState = 0;
  uint8_t bFirst;

  int nDebounceBtn1 = 0;
  int nDebounceBtn2 = 0;

  while (1)
  {
	  bFirst = nStateTime==0;

	  switch(nCurState)
	  {
	  case stInit:
		  {
			  nNextState = stUpdateTime;
			  bColon = 0;
		  }
		  break;
	  case stUpdateTime:
		  {
			  ds3231_GetTime(&timeNow);
			  nHour = timeNow.hour%12;
			  nHour = (nHour==0)?12:nHour;
			  nMin = timeNow.minutes;
			  bColon =! bColon;
			  nNextState = stDisplayTime;
		  }
		  break;

	  case stDisplayTime:
		  if(bFirst)
		  {
			  tm1637_DisplayClock(nHour, nMin, bColon);
			  //debugTrace();
		  }
		  if(nStateTime==50)
		  {
			  nNextState = stUpdateTime;
		  }

		  if(HAL_GPIO_ReadPin(BTN1_GPIO_Port, BTN1_Pin)==GPIO_PIN_RESET){if(nDebounceBtn1++==5){timeNow.seconds=0;timeNow.minutes++;if(timeNow.minutes>59)timeNow.minutes=0;ds3231_SetTimeByTimeStruct(timeNow);}}else nDebounceBtn1 = 0;
		  if(HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin)==GPIO_PIN_RESET){if(nDebounceBtn2++==5){timeNow.hour++;if(timeNow.hour>12)timeNow.hour=1;ds3231_SetTimeByTimeStruct(timeNow);}}else nDebounceBtn2 = 0;

		  if(nDebounceBtn1==0 && nDebounceBtn2==0 && timeNow.seconds==30)
		  {
			  nNextState = stDisplayTemp;
		  }

		  break;

	  case stDisplayTemp:
		  if(bFirst)
		  {
			  ds3231_ForceTempConv();
			  fTemp = ds3231_GetTemp();
			  tm1637_DisplayTempDegC((int)(fTemp+0.5));
		  }

		  if(nStateTime==300)
		  {
			  nNextState = stUpdateTime;
		  }

		  break;
	  case stErr:
		  tm1637_DisplayDecimal(9999, 0);
	  default:
		  break;
	  }

	  HAL_Delay(10);

	  if(nCurState==nNextState)
	  {
		  nStateTime++;
	  }
	  else
	  {
		  nStateTime = 0;
		  nCurState = nNextState;
	  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void printRem(char* str)
{
	HAL_UART_Transmit(&huart1, (uint8_t*) str, strlen(str), 100);
}

void debugTrace()
{
	  char szBuf[20];

	  sprintf (szBuf, "%02d:%02d:%02d\r\n", timeNow.hour, timeNow.minutes, timeNow.seconds);
	  printRem(szBuf);


	  sprintf (szBuf, "%02d-%02d-20%02d\r\n", timeNow.dayofmonth, timeNow.month, timeNow.year);
	  printRem(szBuf);

	  ds3231_ForceTempConv();
	  fTemp = ds3231_GetTemp();

	  sprintf (szBuf, "%d.%02d °C\r\n", (int)fTemp, (int)((fTemp-(int)fTemp)*100.0));
	  printRem(szBuf);

}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
