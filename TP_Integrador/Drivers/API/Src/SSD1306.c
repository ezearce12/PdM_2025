/*
 * SSD1306.c
 *
 *  Created on: Apr 15, 2025
 *      Author: Eze
 */

#include "../../Drivers/API/Inc/SSD1306.h"
#include "../../Drivers/API/Inc/SSD1306_Port.h"
#include "../../Drivers/API/Inc/font.h"
#include <stdlib.h>
#include <stdio.h>

//Comandos de trama del display SSD1306
#define SSD1306_COMMAND         	0x00
#define SSD1306_DATA           		0x40

//Maximo de caractreres por pagina (fuente 5x7)
#define BUFFER_TO_PRINT_LENGTH		22

//Configuraciones de inicializacion del display
#define CLOCK_DIV_CONFIG			0x80
#define MULTIPLEX_RATIO_CONFIG		0x3F
#define COMM_PIN_CONFIG 			0x12
#define CONTRAST_CONFIG				0x7F
#define PRECHARGE_CONFIG			0xF1
#define PAGE_ADDRESSING				0xb0


//Comandos del display
typedef enum {

    SSD1306_CMD_SET_CONTRAST            = 0x81,
    SSD1306_CMD_ENTIRE_DISPLAY_RESUME   = 0xA4,
    SSD1306_CMD_ENTIRE_DISPLAY_ON       = 0xA5,
    SSD1306_CMD_SET_NORMAL_DISPLAY      = 0xA6,
    SSD1306_CMD_SET_INVERT_DISPLAY      = 0xA7,
    SSD1306_CMD_DISPLAY_OFF             = 0xAE,
    SSD1306_CMD_DISPLAY_ON              = 0xAF,

    SSD1306_CMD_SCROLL_RIGHT            = 0x26,
    SSD1306_CMD_SCROLL_LEFT             = 0x27,
    SSD1306_CMD_SCROLL_VERTICAL_RIGHT   = 0x29,
    SSD1306_CMD_SCROLL_VERTICAL_LEFT    = 0x2A,
    SSD1306_CMD_DEACTIVATE_SCROLL       = 0x2E,
    SSD1306_CMD_ACTIVATE_SCROLL        	= 0x2F,
    SSD1306_CMD_SET_VERTICAL_SCROLL_AREA = 0xA3,

    SSD1306_CMD_SET_LOWER_COLUMN        = 0x00,
    SSD1306_CMD_SET_HIGHER_COLUMN       = 0x10,
	SSD1306_CMD_MEMORY_ADDR_MODE        = 0x20,
    SSD1306_CMD_COLUMN_ADDR             = 0x21,
    SSD1306_CMD_PAGE_ADDR               = 0x22,

    SSD1306_CMD_SET_START_LINE          = 0x40,
    SSD1306_CMD_SET_SEGMENT_REMAP_0     = 0xA0,
    SSD1306_CMD_SET_SEGMENT_REMAP_1     = 0xA1,
    SSD1306_CMD_SET_MULTIPLEX_RATIO     = 0xA8,
    SSD1306_CMD_COM_SCAN_INC            = 0xC0,
    SSD1306_CMD_COM_SCAN_DEC            = 0xC8,
    SSD1306_CMD_SET_DISPLAY_OFFSET      = 0xD3,
    SSD1306_CMD_SET_COM_PINS            = 0xDA,

    SSD1306_CMD_SET_DISPLAY_CLOCK_DIV   = 0xD5,
    SSD1306_CMD_SET_PRECHARGE           = 0xD9,
    SSD1306_CMD_SET_VCOM_DESELECT       = 0xDB,
    SSD1306_CMD_NOP                     = 0xE3,

    SSD1306_CMD_CHARGE_PUMP             = 0x8D,
	SSD1306_CMD_ENA_CHARGE_PUMP         = 0x14

} SSD1306_Command;

void SSD1306_SendCommand(uint8_t command);
void SSD1306_SendData(uint8_t* data, size_t size);
void SSD1306_SetCursor(uint8_t x, uint8_t page);

//Envia comando al display
//Recibe el comando a enviar
void SSD1306_SendCommand(uint8_t command) {

	uint8_t data[2];
    data[0] = SSD1306_COMMAND;
    data[1] = command;

    SSD1306_I2C_Transmit(data, 2);

}

//Envio de datos al display
//Recibe direccion de memoria de los datos a enviar y el tamaño a enviar.
void SSD1306_SendData(uint8_t* data, size_t size) {

	//Se define un buffer con el ancho de pixeles del display + el byte de control
	static uint8_t buffer[SSD1306_WIDTH + 1];

	//Verificacion de longitud de datos a enviar
	if (size + 1 > sizeof(buffer)) return;


	memset(buffer, 0, sizeof(buffer));
    buffer[0] = SSD1306_DATA;
    memcpy(&buffer[1], data, size);

    SSD1306_I2C_Transmit(buffer, size + 1);

}

//Inicializa el display
//Inicializa el puerto i2c y realiza la secuencia de inico del display
void SSD1306_Init(void) {


    SSD1306_SendCommand(SSD1306_CMD_DISPLAY_OFF);
    SSD1306_SendCommand(SSD1306_CMD_SET_DISPLAY_CLOCK_DIV);
    SSD1306_SendCommand(CLOCK_DIV_CONFIG);

    SSD1306_SendCommand(SSD1306_CMD_SET_MULTIPLEX_RATIO);
    SSD1306_SendCommand(MULTIPLEX_RATIO_CONFIG);

    SSD1306_SendCommand(SSD1306_CMD_SET_DISPLAY_OFFSET);
    SSD1306_SendCommand(SSD1306_CMD_SET_LOWER_COLUMN);

    SSD1306_SendCommand(SSD1306_CMD_SET_START_LINE);

    SSD1306_SendCommand(SSD1306_CMD_CHARGE_PUMP);
    SSD1306_SendCommand(SSD1306_CMD_ENA_CHARGE_PUMP);

    SSD1306_SendCommand(SSD1306_CMD_MEMORY_ADDR_MODE);
    SSD1306_SendCommand(SSD1306_CMD_SET_LOWER_COLUMN);

    SSD1306_SendCommand(SSD1306_CMD_SET_SEGMENT_REMAP_1);
    SSD1306_SendCommand(SSD1306_CMD_COM_SCAN_DEC);

    SSD1306_SendCommand(SSD1306_CMD_SET_COM_PINS);
    SSD1306_SendCommand(COMM_PIN_CONFIG);
    SSD1306_SendCommand(SSD1306_CMD_SET_CONTRAST);
    SSD1306_SendCommand(CONTRAST_CONFIG);

    SSD1306_SendCommand(SSD1306_CMD_SET_PRECHARGE);
    SSD1306_SendCommand(PRECHARGE_CONFIG);

    SSD1306_SendCommand(SSD1306_CMD_SET_VCOM_DESELECT);
    SSD1306_SendCommand(SSD1306_CMD_SET_START_LINE);
    SSD1306_SendCommand(SSD1306_CMD_ENTIRE_DISPLAY_RESUME);
    SSD1306_SendCommand(SSD1306_CMD_SET_NORMAL_DISPLAY);
    SSD1306_SendCommand(SSD1306_CMD_DISPLAY_ON);

}

//Limpia todo el display, limpiando una pagina a la vez
void SSD1306_Clear(void) {
    uint8_t zero_buffer[SSD1306_WIDTH];
    memset(zero_buffer, 0x00, sizeof(zero_buffer));

    for (uint8_t page = 0; page < 8; page++) {
        SSD1306_SendCommand(0xB0 + page);       // Page start
        SSD1306_SendCommand(SSD1306_CMD_SET_LOWER_COLUMN);              // Lower column start
        SSD1306_SendCommand(SSD1306_CMD_SET_HIGHER_COLUMN);              // Higher column start
        SSD1306_SendData(zero_buffer, SSD1306_WIDTH);
    }
}

//Enciende el display
void SSD1306_DisplayOn(void) {
    SSD1306_SendCommand(SSD1306_CMD_DISPLAY_ON);
}

//Apaga el display
void SSD1306_DisplayOff(void) {
    SSD1306_SendCommand(SSD1306_CMD_DISPLAY_OFF);
}

//Configra el cursor en el mapa de bits del display
//Recibe el indice de caracter 0 - 21.
//Recibe la pagina del display 0 - 7.
void SSD1306_SetCursor(uint8_t x, uint8_t page) {

	assert(x <= SDD1306_MAX_CHARACTER);
	assert(page <= SDD1306_MAX_PAGE);

	static uint8_t cursor5x7;
	cursor5x7 = x * 6;			//Se redimensiona el cursor por la fuente que se usa 5x7 + espacio entre caracteres.

    SSD1306_SendCommand(PAGE_ADDRESSING + page);               		// Configura la pagina
    SSD1306_SendCommand(SSD1306_CMD_SET_LOWER_COLUMN + (cursor5x7 & 0x0F));         // bit inferior
    SSD1306_SendCommand(SSD1306_CMD_SET_HIGHER_COLUMN + ((cursor5x7 >> 4) & 0x0F));  // bit superor

}

//Envia un caracter mas el espacio al display
//Recibe una caracter
void SSD1306_WriteChar(char c) {
	static uint8_t space = 0x00;

	if (c < ASCII_MIN || c > ASCII_MAX) c = '?'; // Caracteres no soportados

    SSD1306_SendData((uint8_t*)Font5x7[c - ASCII_OFFSET], 5); // Enviar 5 columnas
    SSD1306_SendData(&space, 1); 							// Espacio entre caracteres
}


//Envia un string al display
//Recibe la direccion de memopria del string a enviar
void SSD1306_WriteString(char* str) {

	assert(str != NULL);
	assert(SDD1306_MAX_CHARACTER >= strlen(str));

	while (*str) {
        SSD1306_WriteChar(*str++);
    }

}

//Imprime mediciones en el display
//Recibe la medicion actual, maxima y minima
void SSD1306_PrintMesurement(uint16_t Actual,uint16_t Maxima,uint16_t Minima){

	static char buffer[BUFFER_TO_PRINT_LENGTH];

	sprintf(buffer, "%2d.%d[cm]", Actual / 10, Actual % 10);
	SSD1306_SetCursor(10, 0);
	SSD1306_WriteString(buffer);

	sprintf(buffer, "%2d.%d[cm]", Maxima / 10, Maxima % 10);
	SSD1306_SetCursor(7, 1);
	SSD1306_WriteString(buffer);

	sprintf(buffer, "%2d.%d[cm]", Minima / 10, Minima % 10);
	SSD1306_SetCursor(7, 2);
	SSD1306_WriteString(buffer);

}


//Imprime el muestreo en el display
//recibe el muestreo a imprimr
void SSD1306_PrintMuestreo(uint32_t muestreo){

	static char buffer[BUFFER_TO_PRINT_LENGTH];

	sprintf(buffer, "%lu[ms]", muestreo);
	SSD1306_SetCursor(9, 3);
	SSD1306_WriteString(buffer);
}


//Imprime las etiquetas estaticas en el display
//Recibe el puerto y la calibracion del sensor
void SSD1306_PrintSetup(uint8_t port,uint8_t cal){

	static char buffer[BUFFER_TO_PRINT_LENGTH];

	sprintf(buffer, "Distancia:");
	SSD1306_SetCursor(0, 0);
	SSD1306_WriteString(buffer);

	sprintf(buffer, "Maxima:");
	SSD1306_SetCursor(0, 1);
	SSD1306_WriteString(buffer);

	sprintf(buffer, "Minima:");
	SSD1306_SetCursor(0, 2);
	SSD1306_WriteString(buffer);

	sprintf(buffer, "Muestreo:");
	SSD1306_SetCursor(0, 3);
	SSD1306_WriteString(buffer);

	sprintf(buffer, "%s",
			(cal == 0x00) ? "Sin calibrar" :
            (cal == 0x01) ? "Crosstalk calibrado" :
            (cal == 0x02) ? "Offset calibrado" :
            (cal == 0x03) ? "Calibracion completa" : "Desconocida");
	SSD1306_SetCursor(0, 5);
	SSD1306_WriteString(buffer);


	sprintf(buffer, "%s",
			(port == 0x41) ? "UART+I2C" :
            (port == 0x49) ? "Solo I2C" :
            (port == 0x55) ? "Solo UART" : "Desconocido");

	SSD1306_SetCursor(0, 6);
	SSD1306_WriteString(buffer);

}

