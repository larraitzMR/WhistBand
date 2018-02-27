/*
 * gprs.h
 *
 *  Created on: 11 dic. 2017
 *      Author: Laraitz
 */

void send_ATCommand(char *cmd, uint32_t esp_timeout);
int send_ATCommand_DMA(char *cmd);
void read_buffer();
void leerBuffer();


#define AT_HTTTP			"AT#HTTPCFG="
#define SERVER_ADDRESS		"larraitz.myruns.com"
#define SERVER_PORT			80

