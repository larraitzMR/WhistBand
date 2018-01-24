/*
 * gps.c
 *
 *  Created on: 20 nov. 2017
 *      Author: Larraitz
 */

#include "gps.h"
#include "main.h"
#include <string.h>

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart6;

char parsing[80];
struct datosBuffer mibuffer[200];
struct diaDatos GPS[1000];

char *nmea[15];
int i = 0;
int numGPS = 0;
int z = 0;
char *hora, *dia, *latitud, *latCoor, *longitud, *longCoor, *fecha;
char *tipo;
int c = 0;
int mb = 0;
int dat = 0;
int num;
int fechaOK = 0;

uint8_t init_nmea[] = "$PSRF100,0,9600,8,1,0*04\r\n";
uint8_t init_GGA[] = "$PSRF103,00,00,01,01*25\r\n";
uint8_t deinit_GGA[] = "$PSRF103,00,00,00,01*25\r\n";
uint8_t init_GLL[] = "$PSRF103,01,00,01,01*24\r\n";
uint8_t deinit_GLL[] = "$PSRF103,01,00,00,01*25\r\n";
uint8_t init_GSA[] = "$PSRF103,02,00,01,01*27\r\n";
uint8_t deinit_GSA[] = "$PSRF103,02,00,00,01*26\r\n";
uint8_t init_GSV[] = "$PSRF103,03,00,05,01*22\r\n";
uint8_t deinit_GSV[] = "$PSRF103,03,00,00,01*27\r\n";
uint8_t init_RMC[] = "$PSRF103,04,00,01,01*21\r\n";
uint8_t deinit_RMC[] = "$PSRF103,04,00,00,01*20\r\n";
uint8_t init_VTG[] = "$PSRF103,05,00,01,01*20\r\n";
uint8_t deinit_VTG[] = "$PSRF103,05,00,00,01*21\r\n";

__IO ITStatus UartReady = RESET;
__IO ITStatus Uart1Ready = RESET;

void inicializar_gps(void)
{
	if (HAL_UART_Receive_DMA(&huart6, (uint8_t *) parsing, 80) != HAL_OK) {
		Error_Handler();
	}
	while (UartReady != SET) {}
	UartReady = RESET;

	if (HAL_UART_Transmit_DMA(&huart6, (uint8_t *) deinit_GSV, sizeof(deinit_GSV)) != HAL_OK) {
		Error_Handler();
	}
	while (UartReady != SET) {}
	UartReady = RESET;

	if (HAL_UART_Transmit_DMA(&huart6, (uint8_t *) deinit_GSA, sizeof(deinit_GSA)) != HAL_OK) {
		Error_Handler();
	}
	while (UartReady != SET) {}
	UartReady = RESET;

	if (HAL_UART_Transmit_DMA(&huart6, (uint8_t *) deinit_GGA, sizeof(deinit_GGA)) != HAL_OK) {
		Error_Handler();
	}
	while (UartReady != SET) {}
	UartReady = RESET;

	if (HAL_UART_Transmit_DMA(&huart6, (uint8_t *) init_RMC, sizeof(init_RMC)) != HAL_OK) {
		Error_Handler();
	}
	while (UartReady != SET) {}
	UartReady = RESET;
}

/**
  * @brief  Las tramas obtenidas se guardan en un struct
  * @param  None
  * @retval None
  */
void guardar_coordenadas(char buffer[])
{
	strcpy(mibuffer[c].datos, buffer);
	c++;
	if (c == 199)
	{
		c = 0;
	}
//	obtener_coordenadas();
}

/**
  * @brief  Se obtienen las coordenadas del gps por la uart
  * @param  None
  * @retval None
  */
void obtener_coordenadas() {

	char* str = NULL;
	char* car = NULL;
	char* p;
//	char* busca = "$GPGGA";
	char* busca = "$GNRMC";

	str = strstr(mibuffer[mb].datos,busca);

	if (str)
	{
		car = strchr(str,'\n');
		if (car){
			mb++;
		}
		else {
			str = NULL;
			mb++;
			if (mb == 199){
				mb = 0;
			}
			return;
		}

	} else {
		str = NULL;
		mb++;
		if (mb == 199){
			mb = 0;
		}
		return;
	}

	if (mb == 199){
		mb = 0;
	}

	for (p = strtok(str + 1, ","); p; p = strtok( NULL, ",")) {
		nmea[i] = p;
		i++;
		if (i == 13){break;}
	}
	p = NULL;
	i = 0;
	tipo = *&nmea[0];
	// http://www.gpsinformation.org/dale/nmea.htm#GGA
	// Example GNRMC
	// Array index -->        0   |      1      |    2    |     3     | 4 |     5      | 6 |   7  |   8    |    9   |10|11|  12  | 13  |   14
	// Example     -->     $GNRMC ,  014035.00 	,    A    , 4317.3578 , N , 00159.1794 , W , 0.00 , 100.74 , 060417 ,  ,  , A*67 ,
	// Description -->            |  Time UTC  	|  status | latitude  |N/S| longitude  |W/E|Speed |  Angle |  Date	|  |  |Checksum
	//                              01:40:35 UTC| A=active
	//                                          |  V=Void

//	if (strcmp(tipo, "GNRMC") == 0) {
//		if ((fechaOK == 0) && (*&nmea[9] != '\0')) {
//			fecha = *&nmea[9];
//			fecha = formatoFecha(fecha);
//			strcpy(GPS[numGPS].dia, fecha);
//			fechaOK = numGPS;
//		}
//
//		hora = *&nmea[1];
//		hora = formatoHora(hora);
//		latitud = *&nmea[3];
//		latitud = formatoLat(latitud);
//		latCoor = *&nmea[4];
//		longitud = *&nmea[5];
//		longitud = formatoLong(longitud);
//		longCoor = *&nmea[6];
//
//		strcpy(GPS[numGPS].datos.hora, hora);
//		strcpy(GPS[numGPS].datos.latitud, latitud);
//		strcpy(GPS[numGPS].datos.latCoor, latCoor);
//		strcpy(GPS[numGPS].datos.longitud, longitud);
//		strcpy(GPS[numGPS].datos.longCoor, longCoor);
//
//		numGPS++;
//		if (numGPS == 999) {
//			numGPS = 0;
//		}
//	}
//
//// http://www.gpsinformation.org/dale/nmea.htm#GGA
//// Example GPGGA
//// Array index -->        0   |      1       |     2     | 3 |    4       | 5 |     6     |       7     |     8     |    9   |  10  |     11	 |  12  |13|   14
//// Example     -->     $GPGGA ,  092152.000  , 4317.3578 , N , 00159.1794 , W ,     1	  ,		06	    ,    1.5    , -10.4  ,   M  ,    51.0	 ,   M  ,  , 0000*54
//// Description -->            |   Time UTC   | Latitude  |N/S| Longitude  |W/E|Fix quality|N. satellites|H.diluition|Altitude|Meters|Heigth geoid|Meters|  |checksum
////                              09:21:52 UTC								  |1 = GPS fix
////
//	else if (strcmp(tipo, "GPGGA") == 0) {
//		hora = *&nmea[1];
//		hora = formatoHora(hora);
//		latitud = *&nmea[2];
//		latitud = formatoLat(latitud);
//		latCoor = *&nmea[3];
//		longitud = *&nmea[4];
//		longitud = formatoLong(longitud);
//		longCoor = *&nmea[5];
//
//		strcpy(GPS[numGPS].datos.hora, hora);
//		strcpy(GPS[numGPS].datos.latitud, latitud);
//		strcpy(GPS[numGPS].datos.latCoor, latCoor);
//		strcpy(GPS[numGPS].datos.longitud, longitud);
//		strcpy(GPS[numGPS].datos.longCoor, longCoor);
//
//		numGPS++;
//		if (numGPS == 999) {
//			numGPS = 0;
//		}
//	}
}

/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
 {
	if (UartHandle->Instance == USART1) {
		(&huart1)->gState = HAL_UART_STATE_READY;
		Uart1Ready = SET;
		(&huart6)->RxState = HAL_UART_STATE_READY;
	}
	else if (UartHandle->Instance == USART6) {
		(&huart6)->gState = HAL_UART_STATE_READY;
		UartReady = SET;
		(&huart6)->RxState = HAL_UART_STATE_READY;
	}
}


/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if (UartHandle->Instance == USART1) {
		(&huart1)->RxState = HAL_UART_STATE_READY;
		Uart1Ready = SET;
	}
	else if (UartHandle->Instance == USART6) {
		(&huart6)->RxState = HAL_UART_STATE_READY;
		imprimir(parsing);
		guardar_coordenadas(parsing);
		UartReady = SET;
	}
}
