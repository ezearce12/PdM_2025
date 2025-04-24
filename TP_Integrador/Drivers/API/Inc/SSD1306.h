/*
 * SSD1306.h
 *
 *  Created on: Apr 15, 2025
 *      Author: Eze
 */

#ifndef API_INC_SSD1306_H_
#define API_INC_SSD1306_H_

#include <string.h>
#include "stm32f4xx_hal.h"
#include <assert.h>

//Dimensiones del display
#define SSD1306_WIDTH           128
#define SSD1306_HEIGHT          64
#define SDD1306_MAX_CHARACTER	21
#define SDD1306_MAX_PAGE		7



void SSD1306_Init(void);
void SSD1306_Clear(void);
void SSD1306_DisplayOn(void);
void SSD1306_DisplayOff(void);
void SSD1306_WriteChar(char c);
void SSD1306_WriteString(char* str);
void SSD1306_PrintMesurement(uint16_t Actual,uint16_t Maxima,uint16_t Minima);
void SSD1306_PrintMuestreo(uint32_t muestreo);
void SSD1306_PrintSetup(uint8_t port,uint8_t cal);
#endif /* API_INC_SSD1306_H_ */
