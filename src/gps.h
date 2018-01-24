/*
 * gps.h
 *
 *  Created on: 20 nov. 2017
 *      Author: Larraitz
 */

#include "main.h"

void inicializar_gps(void);
void leer_parsing(void);
void guardar_coordenadas(char buffer[]);
void obtener_coordenadas() ;



struct datosGPS {
	char hora[8];
	char latitud[10];
	char latCoor[1];
	char longitud[10];
	char longCoor[1];
};

struct datosBuffer {
	char datos[200];
};

struct diaDatos{
	char dia[8];
	struct datosGPS datos;
};
