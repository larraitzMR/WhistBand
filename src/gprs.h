/*
 * gprs.h
 *
 *  Created on: 11 dic. 2017
 *      Author: Laraitz
 */

void send_ATCommand(char *cmd, uint32_t esp_timeout);
void send_ATCommand_DMA(char *cmd);
void read_buffer();
void leerBuffer();
