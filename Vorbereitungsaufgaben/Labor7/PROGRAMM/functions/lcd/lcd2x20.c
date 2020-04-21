/*
 * lcd2x20.c
 *
 *  Created on: Apr 19, 2020
 *      Author: R. Grünert
 */

#include "functions/lcd/lcd2x20.h"

///////////////////////////////////////////////////////

uint8_t lcd_read_byte() { // 4 bit mode read Busy flag (DB7) and Address Counter (DB6-DB0)

    uint8_t return_byte = 0;

    // configure LCD Port with output direction for control bits
    LCD_DIR |=  (1<<RS) | (1<<RW) | (1<<EN);

    // configure LCD Port with input direction for data bits
    LCD_DIR &= ~(0x0F << D4);

    // set read operation
    // 0: Instruction register
    // 1: Data register
    LCD_OUT &= ~(1<<RS);

    LCD_OUT |=  (1<<RW);

    LCD_ENABLE_HIGH; // read data is on port after enable high (not high low transition) see figure 28 timing diagram

        // data delay time? t_dd

    // now the data can be put into the upper nibble of the return byte
    return_byte = ( ( (LCD_IN & (0x0F << D4)) >> D4) << 4);

    LCD_ENABLE_LOW;
    DELAY_10US; // t_cycE?

    LCD_ENABLE_HIGH;
    // get lower nibble
    return_byte |= ((LCD_IN & (0x0F << D4)) >> D4);

    LCD_ENABLE_LOW;

    // reset control bit
    LCD_OUT &= ~(1<<RW);

    return(return_byte);
}


///////////////////////////////////////////////////////

void lcd_write_byte_IR(uint8_t write_byte) { // write byte to instruction register

    while(LCD_IS_BUSY) {} // check busy flag and continue if 0

    /// setup ports

    // configure LCD Port with output direction for control bits and data bits
    LCD_DIR |=  (1<<RS) | (1<<RW) | (1<<EN) | (0x0F << D4);

    // reset
    LCD_OUT &= ~((1<<RS) | (1<<RW) | (1<<EN) | (0x0F << D4));

    // configure read operation, register select: Instruction Register
    LCD_OUT &= ~((1<<RW) | (1<<RS));


    /// writing data

    // upper nibble

        LCD_ENABLE_HIGH;

        // set data on port while enable is high
        LCD_OUT |= (((write_byte & 0xF0) >> 4) << D4);

        LCD_ENABLE_LOW; // data read by LCD on high-->low transition

        DELAY_10US;

    // lower nibble


        LCD_ENABLE_HIGH;

        LCD_OUT &= ~(0x0F << D4);         // remove previous upper nibble data
        LCD_OUT |= ((write_byte & 0x0F) << D4);

        LCD_ENABLE_LOW;

}


///////////////////////////////////////////////////////

void lcd_write_byte_DR(uint8_t write_byte) { // write byte to data register

    while(LCD_IS_BUSY) {} // check busy flag and continue if 0

    /// setup ports

    // configure LCD Port with output direction for control bits and data bits
    LCD_DIR |=  (1<<RS) | (1<<RW) | (1<<EN) | (0x0F << D4);

    // reset
    LCD_OUT &= ~((1<<RS) | (1<<RW) | (1<<EN) | (0x0F << D4));

    // configure write operation
    LCD_OUT &= ~(1<<RW);

    // register select: Data register
    LCD_OUT |=  (1<<RS);


    /// writing data

    // upper nibble

        LCD_ENABLE_HIGH;

        // set data on port while enable is high
        LCD_OUT |= (((write_byte & 0xF0) >> 4) << D4);

        LCD_ENABLE_LOW; // data read by LCD on high-->low transition

        DELAY_10US;

    // lower nibble




        LCD_ENABLE_HIGH;

        LCD_OUT &= ~(0x0F << D4); // remove previous upper nibble data
        LCD_OUT |= ((write_byte & 0x0F) << D4);

        LCD_ENABLE_LOW;

}


///////////////////////////////////////////////////////

uint8_t lcd_pos(uint8_t row, uint8_t col) {

    if( (row >= LCD_ROWS) ) { // out of range
        return 1;
    }

    // set row and column, see data sheet commands, this sets the DDRAM address
    lcd_write_byte_IR( (1<<7) | (row << 6) | col  ); // BIT 7 for DDRAM address write instruction, BIT6 selects row (1 or 0)

    return 0;


}


///////////////////////////////////////////////////////

uint8_t lcd_print(char* text, uint8_t row, uint8_t col) {

    if((col >= LCD_COLS) || (row >= LCD_ROWS)) {
        return 1;
    }

    lcd_pos(row, col);

    while(*text != '\0' ) { // as long as string character it is not zero/ the termination character, the != '\0' could also be omitted

        lcd_write_byte_DR( *(text++) );

    }

    return 0;

}


///////////////////////////////////////////////////////

void lcd_init() {

    /// Port config

        // output direction for whole port
        LCD_DIR |=  ( (1<<RS) | (1 << RW) | (1<<EN) | (0x0F << D4));

        // reset pins
        LCD_OUT &= ~( (1<<RS) | (1 << RW) | (1<<EN) | (0x0F << D4));

        delay_ms(100);

    /// Initialization
        LCD_ENABLE_HIGH;

        LCD_OUT |= (0x03 << D4); // see Datasheet

        LCD_ENABLE_LOW; // data read by lcd on High-->Low transition

        // delay 4.1 ms min
        delay_ms(10);

            LCD_ENABLE_HIGH;

            DELAY_10US; // tw

            LCD_ENABLE_LOW;

        delay_us(200); ////////////////

            LCD_ENABLE_HIGH;

            DELAY_10US; // tw

            LCD_ENABLE_LOW;


        // 4-bit-mode (function set)
        LCD_ENABLE_HIGH;

        LCD_OUT &=~ (0x0F << D4);

        LCD_OUT |= (0x02 << D4);

        DELAY_10US; //

        LCD_ENABLE_LOW;

        // function set (BIT5), 0 for 4 bit DL, 1 for 2 lines,
        lcd_write_byte_IR((1<<5) | (DATA_LENGTH << 4) | (NR_LINES << 3) | (FONT << 2));

        delay_us(50);

        LCD_OFF;
        LCD_CLEAR_SCREEN;
        LCD_HOME;

        lcd_write_byte_IR((1<<2) | (CURSOR_MOVE_DIR << 1));

        LCD_ON;

}
