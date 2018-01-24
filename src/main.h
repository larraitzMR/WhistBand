/*
 * main.h
 *
 *  Created on: 20 nov. 2017
 *      Author: Larraitz
 *
 */


#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
  #define RTC_ASYNCH_PREDIV    0x7F
  #define RTC_SYNCH_PREDIV     0x0130

void imprimir(char* msg);
void Error_Handler(void);
uint32_t TimeoutCalculation(uint32_t timevalue);
void Write_Flash(char *data);
