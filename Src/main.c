/*
 * main.c
 *
 *  Created on: 14 nov. 2017
 *      Author: Larraitz
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "init.h"
#include "gps.h"
//#include "stm32f4xx_lp_modes.h"
#include <string.h>
/* Private variables ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);


/* Private function prototypes -----------------------------------------------*/

extern WWDG_HandleTypeDef hwwdg;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart6;
extern SPI_HandleTypeDef hspi1;
/*Variable used for Erase procedure*/
FLASH_EraseInitTypeDef EraseInitStruct;

uint32_t SECTORError = 0;
char pars[80];
uint8_t datoFlash[16];
uint8_t flash = 888;
char *flash1 = "ESTO ES UN MENSAJE";
char parsingGPS[5] = "HOLII";

__attribute__((__section__(".user_data"))) const char userConfig[64];

int main(void) {

	/* MCU Configuration----------------------------------------------------------*/
	uint32_t delay;

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

//	strcpy(datoFlash, userConfig[0]);
//
//	Write_Flash(flash1);
//	/* Initialize all configured peripherals */
//	strcpy(datoFlash, userConfig[0]);
	GPIO_Init();
	BSP_LED_Init(LED2);

	UART2_Init();
	DMA_Init();
//	UART6_Init();
	UART1_Init();

	imprimir("\r\nCOMIENZO PROGRAMA\r\n");

//	inicializar_gps();

//	LP_Init();
//
//	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
//	HAL_Init();
//
//	/* Initialize all configured peripherals */
//	GPIO_Init();
//	UART2_Init();
//	DMA_Init();
//	UART6_Init();
//
//	inicializar_gps();

//  I2C1_Init();
//
	/*##-1- Check if the system has resumed from WWDG reset ####################*/
//	if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST) != RESET) {
//		/* WWDGRST flag set: Turn LED2 on */
//		BSP_LED_On(LED2);
//
//		/* Insert 4s delay */
//		HAL_Delay(4000);
//
//		/* Prior to clear WWDGRST flag: Turn LED2 off */
//		BSP_LED_Off(LED2);
//	}
//
//	/* Clear reset flags in any case */
//	__HAL_RCC_CLEAR_RESET_FLAGS();
//
//	BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);
//
//	WWDG_Init();
//
//	/* calculate delay to enter window. Add 1ms to secure round number to upper number  */
//	delay = TimeoutCalculation((hwwdg.Init.Counter - hwwdg.Init.Window) + 1) +  1;
	read_buffer();

	send_ATCommand_DMA("AT\r\n");
	HAL_Delay(1000);
//	leerBuffer();
//	send_ATCommand_DMA("AT+CMGF=1\r\n");
//	HAL_Delay(1000);
//	leerBuffer();
//	send_ATCommand_DMA("AT+CMGS=+34649103025\r\n");
//	HAL_Delay(1000);
//	leerBuffer();
//	send_ATCommand_DMA("ieeeeeee\r\n");
//	HAL_Delay(1000);
//	leerBuffer();
//	send_ATCommand_DMA("\x1A");
//	HAL_Delay(1000);
//	leerBuffer();

	while (1) {
		/* Toggle LED2 */
		BSP_LED_Toggle(LED2);
		//HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)"HOLAAAA", (uint8_t *)pars, 7, 5000);
//		imprimir("main loop");
		/* Insert calculated delay */
//		HAL_Delay(delay);

		HAL_Delay(3000);

//		if (HAL_WWDG_Refresh(&hwwdg) != HAL_OK) {
//			Error_Handler();
//		}
	}
}

uint32_t FlashAddress = 0x08040000;

void Write_Flash(char *data)
{
	HAL_FLASH_Unlock();
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR);
	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Sector = FLASH_SECTOR_6;
	EraseInitStruct.NbSectors = 1;
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK) {
		Error_Handler();
	}
	HAL_FLASH_Program(TYPEPROGRAM_WORD, FlashAddress, (uint64_t)data);
	HAL_FLASH_Lock();
}

void imprimir(char* msg){
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 0xFFFF);

}
/**
  * @brief  Timeout calculation function.
  *         This function calculates any timeout related to
  *         WWDG with given prescaler and system clock.
  * @param  timevalue: period in term of WWDG counter cycle.
  * @retval None
  */
uint32_t TimeoutCalculation(uint32_t timevalue)
{
  uint32_t timeoutvalue = 0;
  uint32_t pclk1 = 0;
  uint32_t wdgtb = 0;

  /* considering APB divider is still 1, use HCLK value */
  pclk1 = HAL_RCC_GetPCLK1Freq();

  /* get prescaler */
  wdgtb = (1 << ((hwwdg.Init.Prescaler) >> 7)); /* 2^WDGTB[1:0] */

  /* calculate timeout */
  timeoutvalue = ((4096 * wdgtb * timevalue) / (pclk1 / 1000));

  return timeoutvalue;
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 100000000
  *            HCLK(Hz)                       = 100000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            PLL_M                          = 16
  *            PLL_N                          = 400
  *            PLL_P                          = 4
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale2 mode
  *            Flash Latency(WS)              = 3
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /* Enable HSI Oscillator and activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 0x10;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }

//  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
//
//  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
//
//  /* SysTick_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED2);

  while(1)
  {
  }
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

