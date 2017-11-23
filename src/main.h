/*
 * main.h
 *
 *  Created on: 20 nov. 2017
 *      Author: Larraitz
 *
 */


#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo.h"

void imprimir(char* msg);
void Error_Handler(void);
static uint32_t TimeoutCalculation(uint32_t timevalue);
