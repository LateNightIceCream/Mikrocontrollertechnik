/*
 * lcd2x20.h
 *
 *  Created on: Apr 16, 2020
 *      Author: R. Grünert
 */

#ifndef FUNCTIONS_LCD_LCD2X20_H_
#define FUNCTIONS_LCD_LCD2X20_H_

#include <stdint.h>
#include "functions/delay/delays.h"
#include "configurations/lcdconfig.h"

uint8_t lcd_read_byte(); // reads busy flag and address counter bits

void    lcd_init();
uint8_t lcd_pos(uint8_t row, uint8_t col);               // set lcd row and col position (DDRAM address)
uint8_t lcd_print(char* text, uint8_t row, uint8_t col); // print string to lcd
void    lcd_write_byte_IR(uint8_t write_byte);
void    lcd_write_byte_DR(uint8_t write_byte);


#endif /* FUNCTIONS_LCD_LCD2X20_H_ */
