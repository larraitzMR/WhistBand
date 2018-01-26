/*
 * init.c
 *
 *  Created on: 14 nov. 2017
 *      Author: Larraitz
 */

#include "init.h"
#include "main.h"
#include "lp_modes.h"


I2C_HandleTypeDef hi2c1;
SPI_HandleTypeDef hspi1;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart6;
WWDG_HandleTypeDef hwwdg;


/** Pinout Configuration
*/
void GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __GPIOA_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();

}

/* USART1 init function */
void UART1_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart1);

}

/* USART2 init function */
void UART2_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart2);

}

/* USART1 init function */
void UART6_Init(void)
{

	huart6.Instance = USART6;
	huart6.Init.BaudRate = 9600;
	huart6.Init.WordLength = UART_WORDLENGTH_8B;
	huart6.Init.StopBits = UART_STOPBITS_1;
	huart6.Init.Parity = UART_PARITY_NONE;
	huart6.Init.Mode = UART_MODE_TX_RX;
	huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart6.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart6);

}

/**
  * Enable DMA controller clock
  */
void DMA_Init(void)
{
  /* DMA controller clock enable */
  __DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
  HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);
  HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
  HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

}

/* SPI1 init function */
void SPI1_Init(void)
{

  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;

  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLED;

  HAL_SPI_Init(&hspi1);

}

/* I2C1 init function */
void I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
  HAL_I2C_Init(&hi2c1);

}

/* WWDG init function */
void WWDG_Init(void)
{

  hwwdg.Instance = WWDG;
  hwwdg.Init.Prescaler = WWDG_PRESCALER_8;
  hwwdg.Init.Window    = 80;
  hwwdg.Init.Counter   = 127;
  hwwdg.Init.EWIMode   = WWDG_EWI_DISABLE;
  HAL_WWDG_Init(&hwwdg);

}

void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef* hwwdg)
{
	imprimir("EarlyWakeUP\r\n");
}


__IO uint32_t uwCounter = 0;

void LP_Init(void)
{
	/* Enable Power Clock */
	__HAL_RCC_PWR_CLK_ENABLE();

	/* Check and handle if the system was resumed from Standby mode */
	if (__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET) {
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);

		/* Infinite loop */
		while (1) {
			/* Toggle LED2 */
			BSP_LED_Toggle(LED2);

			/* Insert a 100ms delay */
			HAL_Delay(100);
		}
	}

	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

	/* Wait until USER button is pressed to enter the Low Power mode */
	while (BSP_PB_GetState(BUTTON_KEY) != RESET) {
		/* Toggle LED2 */
		BSP_LED_Toggle(LED2);

		/* Insert 1s Delay */
		HAL_Delay(1000);
	}

	/* Loop while USER Button is maintained pressed */
	while (BSP_PB_GetState(BUTTON_KEY) == RESET) {
	}

#if defined (SLEEP_MODE)
	/* Sleep Mode Entry
	 - System Running at PLL (168MHz)
	 - Flash 5 wait state
	 - Instruction and Data caches ON
	 - Prefetch ON
	 - Code running from Internal FLASH
	 - All peripherals disabled.
	 - Wake-up using EXTI Line (User Button)
	 */
	SleepMode_Measure();
#elif defined (STOP_MODE)
  /* STOP Mode Entry
      - RTC Clocked by LSI
      - Regulator in LP mode
      - HSI, HSE OFF and LSI OFF if not used as RTC Clock source
      - No IWDG
      - FLASH in deep power down mode
      - Automatic Wake-up using RTC clocked by LSI (after ~20s)
   */
  StopMode_Measure();
#elif defined (STANDBY_MODE)
  /* STANDBY Mode Entry
      - Backup SRAM and RTC OFF
      - IWDG and LSI OFF
      - Wake-up using WakeUp Pin (PA.00)
   */
  StandbyMode_Measure();

#elif defined (STANDBY_RTC_MODE)
  /* STANDBY Mode with RTC on LSI Entry
      - RTC Clocked by LSI
      - IWDG OFF and LSI OFF if not used as RTC Clock source
      - Backup SRAM OFF
      - Automatic Wake-up using RTC clocked by LSI (after ~20s)
   */
  StandbyRTCMode_Measure();

#elif defined (STANDBY_RTC_BKPSRAM_MODE)
  /* STANDBY Mode with RTC on LSI Entry
      - RTC Clocked by LSI
      - Backup SRAM ON
      - IWDG OFF
      - Automatic Wake-up using RTC clocked by LSI (after ~20s)
  */
  StandbyRTCBKPSRAMMode_Measure();
#endif

	if (uwCounter != 0) {
		BSP_LED_Init(LED2);
	}

}

/**
  * @brief SYSTICK callback
  * @param None
  * @retval None
  */
void HAL_SYSTICK_Callback(void)
{
  HAL_IncTick();
}

/**
  * @brief  Wake Up Timer callback
  * @param  hrtc : hrtc handle
  * @retval None
  */
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
  /* NOTE : add the specific code to handle the RTC wake up interrupt */
  uwCounter = 1;
  imprimir("RTC callback");
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Configure LED2 */
  BSP_LED_Init(LED2);
  /* NOTE : add the specific code to handle the wake up button interrupt */
  if(GPIO_Pin == KEY_BUTTON_PIN)
  {
    uwCounter = 2;
    imprimir("GPIO callback");
  }
}


