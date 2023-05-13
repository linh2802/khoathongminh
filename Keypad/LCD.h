/******************************************************************************
 * File: LCD.h
 *****************************************************************************/

#ifndef LCD_H_
#define LCD_H_

//////////////////////////////////////////////////////////////////////////////////////////

#define RS              GPIO_PIN_0 // RS
#define RW              GPIO_PIN_1// RW
#define E               GPIO_PIN_2// E
#define LCDPORT         GPIO_PORTB_BASE// PORTB
#define highpin         GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7 // D4 D5 D6 D7


void has_lcd_4bitsetup(); //make all pin as output pin, Enable Peripheral of GPIO_portB

void has_lcd_erase();//erase the characters was appeared in LCD Display

void has_lcd_sendchar(uint8_t ch);//send a char to LCD a char by setting E pin to 1

void has_lcd_sendcommand(uint8_t x);//send a command by clearing E pin

void has_lcd_write(uint8_t line , uint8_t column,char str[]);//write an array of charecters in spercific line and column

void has_lcd_position(uint8_t line , uint8_t column);//send the cursor to specific line and column

void has_lcd_switchwrite();//allowing to use thw SW1 and SW2 in Tiva kit to enter charecters

////////////////////////////////////////////////////////////////////////////////////////////

#endif /* HASLCD_JR_H_*/
