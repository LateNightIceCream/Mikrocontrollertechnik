// just some incomplete code to capture the general idea

// 4bit mode lcd

#include "lcd.h"

void lcd_read_byte(void) { // read Address counter bits and Busy Flag bit

  // 4 bit mode, so 

  char return_byte = 0; // return byte

  // configure lcd port control bits as output (1)
  LCD_DIR |= (1 << RS | 1 << RW | 1 << E);

  // configure lcd port D-Bits as input
  LCD_DIR &= (0x0F << DB4);

  // set read operation
  // RS: 0 (instruction register)
  // RW: 1 (read)
  LCD_OUT &= ~(1 << RS);
  LCD_OUT |=  (1 << RW);

  LCD_ENABLE_HIGH; // read data is on port after enable high (not high low transition) see figure 28 timing diagram

  // data delay time?? t_dd

  // now the data can be put into the upper nibble of the return value
  return_byte = (( (LCD_IN & (0x0F << DB4) ) >> DB4) << 4);

  LCD_ENABLE_LOW;

  delay_us( 10 ); // t_cycE?


  LED_ENABLE_HIGH;

  // get lower nibble
  return_byte |= (LCD_IN & (0x0F << DB4)) >> DB4;

  LED_ENABLE_LOW;

  // reset 

}
