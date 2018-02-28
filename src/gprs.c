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

#define RXBUFFERSIZE 60
uint8_t aRxBuffer[RXBUFFERSIZE];
uint8_t OK[2] = "OK";
uint8_t PDP[7] = "#SGACT:";
uint8_t Post[3] = ">>>";
uint8_t ACT[15] = "context already";
uint8_t HTTPPring200[16] = "#HTTPRING: 0,200";
uint8_t HTTPPring400[16] = "#HTTPRING: 0,400";
uint8_t HTTPPring0[14] = "#HTTPRING:  0,0";





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
	while (HAL_UART_Receive_DMA(&huart1, (uint8_t *) aRxBuffer, RXBUFFERSIZE) != HAL_OK)
	{
		//imprimir("Error Read");
	}
//	while (Uart1Ready != SET) {}
//	Uart1Ready = RESET;
	HAL_Delay(5000);
//	imprimir(aRxBuffer);
}


void leerBuffer()
{
	imprimir(aRxBuffer);
}

void network_registration(){


	send_ATCommand("AT+CMEE=2\r\n", 5000);
	HAL_Delay(5000);
	leerBuffer();

	send_ATCommand("AT+COPS=4,2,\"42502\"\r\n", 5000);
	while(isOK()!=0){
		send_ATCommand("AT+COPS=4,2,\"42502\"\r\n", 5000);
		leerBuffer();
		HAL_Delay(2000);
	}
	HAL_Delay(2000);

	send_ATCommand("AT+CGDCONT=1,\"IP\",\"internetm2m.air.com\"\r\n", 5000);
	while(isOK()!=0){
		send_ATCommand("AT+CGDCONT=1,\"IP\",\"internetm2m.air.com\"\r\n", 5000);
		leerBuffer();
		HAL_Delay(2000);
	}
	HAL_Delay(2000);

	send_ATCommand("AT#SGACT=1,1\r\n", 5000);
	leerBuffer();
	while(PDPActivated() != 0) {
		send_ATCommand("AT#SGACT=1,1\r\n", 5000);
		leerBuffer();
		HAL_Delay(5000);
	}
	HAL_Delay(2000);

	send_ATCommand("AT#HTTPCFG=0,\"larraitz.myruns.com\",80,0,,,0,120,1\r\n",5000);
	while (isOK() != 0) {
//		send_ATCommand("AT#HTTPCFG=0,\"larraitz.myruns.com\",80,0,,,0,120,1\r\n",5000);
//		leerBuffer();
//		HAL_Delay(2000);
	}
}

void HTTP_post(){
	send_ATCommand("AT#HTTPSND=0,0,\"/pruebas_post.php\",46,\"application/x-www-form-urlencoded\"\r\n",1000);
	HAL_Delay(5000);
	enviar_coordenadas_gprs();
//	HAL_Delay(5000);
	while(response_OK() != 0){
		HAL_Delay(10000);
	}

}
int PDPActivated(){
	char* str = NULL;
	char* acti = NULL;
	str = strstr(aRxBuffer,PDP);
	acti = strstr(aRxBuffer,ACT);
	if(!(acti || str)){
		return -1;
	}
	//recoger la IP en la que esta registrado
	return 0;
}
int isOK() {
	char* str = NULL;
	str = strstr(aRxBuffer,OK);
	if(!str){
			return -1;
	}
	return 0;
}
int init_post(){
	char* str = NULL;
	str = strstr(aRxBuffer, Post);
	if (!str) {
		return -1;
	}
	return 0;
}

int response_OK(){
	char* str = NULL;
	char* mal = NULL;
	char* cero = NULL;
	str = strstr(aRxBuffer, HTTPPring200);
	mal = strstr(aRxBuffer, HTTPPring400);
	cero = strstr(aRxBuffer, HTTPPring0);

	while (str || mal || cero)
	{}
	if (str) {
		imprimir("Envio OK\r\n");
		return 0;
	} else if (mal) {
		imprimir("Bad Request\r\n");
		HAL_Delay(10000);
		send_ATCommand("AT#HTTPSND=0,0,\"/pruebas_post.php\",46,\"application/x-www-form-urlencoded\"\r\n",5000);
		HAL_Delay(5000);
		enviar_coordenadas_gprs();
		return -1;
	}
	else if (cero){
		imprimir("Envio 0\r\n");
		HAL_Delay(10000);
		send_ATCommand("AT#HTTPSND=0,0,\"/pruebas_post.php\",46,\"application/x-www-form-urlencoded\"\r\n",5000);
		HAL_Delay(5000);
		enviar_coordenadas_gprs();
		return -1;
	}
}

