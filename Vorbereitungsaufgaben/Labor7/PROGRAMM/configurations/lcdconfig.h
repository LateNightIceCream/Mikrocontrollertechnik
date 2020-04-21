/*
 * lcdconfig.h
 *
 *  Created on: Apr 19, 2020
 *      Author: R. Grünert
 */

#ifndef CONFIGURATIONS_LCDCONFIG_H_
#define CONFIGURATIONS_LCDCONFIG_H_

// LCD general bit settings
#define FONT_5_10       1
#define FONT_5_8        0
#define DL_4_BIT        0
#define DL_8_BIT        1
#define LINES_1         0
#define LINES_2         1
#define INCREMENT       1
#define DECREMENT       0

/// LCD Properties
#define LCD_ROWS        2
#define LCD_COLS        20

#define FONT            FONT_5_8
#define NR_LINES        LINES_2
#define DATA_LENGTH     DL_4_BIT

#define CURSOR_MOVE_DIR INCREMENT

/// Port 6.0 to 6.6 as LCD Port
#define LCD_DIR  P6DIR
#define LCD_OUT  P6OUT
#define LCD_IN   P6IN

/// LCD Pins
#define RS   0
#define RW   1
#define EN   2
#define D4   3
#define D5   4
#define D6   5
#define D7   6



/// useful things
#define LCD_ENABLE_HIGH (LCD_OUT |=  (1<<EN))
#define LCD_ENABLE_LOW  (LCD_OUT &= ~(1<<EN))


#define LCD_IS_BUSY      (lcd_read_byte() & 0x80)

#define LCD_CLEAR_SCREEN (lcd_write_byte_IR(0x01))

#define LCD_OFF          (lcd_write_byte_IR(0x08))

#define LCD_ON           (lcd_write_byte_IR(0x0F))

#define LCD_HOME         (lcd_write_byte_IR(0x02))

#endif /* CONFIGURATIONS_LCDCONFIG_H_ */
