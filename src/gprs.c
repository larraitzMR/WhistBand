/*
 * gprs.c
 *
 *  Created on: 11 dic. 2017
 *      Author: Larraitz
 */


#include "main.h"
#include "gprs.h"
#include <string.h>

extern UART_HandleTypeDef huart1;
extern I2C_HandleTypeDef hi2c1;
extern __IO ITStatus Uart1Ready;

uint8_t RXBUFFERSIZE = 5;
uint8_t aRxBuffer[50];
extern uint8_t OKMsg[2];

/**
 * @brief  Send an AT command string
 * @param  String command
 * @param  Timeout in ms
 * @retval None
 */
void send_ATCommand(char *cmd, uint32_t esp_timeout) {
	uint16_t cmd_len = strlen(cmd);

	if (HAL_UART_Transmit(&huart1, (uint8_t *) cmd, cmd_len, esp_timeout) != HAL_OK) {
//		imprimir("Error");
	}
}

int send_ATCommand_DMA(char *cmd) {
	uint16_t cmd_len = strlen(cmd);

	if (HAL_UART_Transmit_DMA(&huart1, (uint8_t *) cmd, cmd_len) != HAL_OK) {
		imprimir("Error Send");
		return -1;
	}
	while (Uart1Ready != SET) {}
	Uart1Ready = RESET;
	return 0;
}

void read_buffer() {
	while (HAL_UART_Receive(&huart1, (uint8_t *) aRxBuffer, RXBUFFERSIZE,1000) != HAL_OK)
	{
		//imprimir("Error Read");
	}
//	while (Uart1Ready != SET) {}
//	Uart1Ready = RESET;
	HAL_Delay(5000);
	imprimir(aRxBuffer);
}


void leerBuffer()
{
	imprimir(aRxBuffer);
}


int isOK(int a) {
	if (strncmp((const char*) aRxBuffer, (const char*) OKMsg, 2) != 0) {
		return -1;
	}
	return 0;
}
