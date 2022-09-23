#include <stdint.h>
#include "TM4C123GH6PM.h"

#define LCD GPIOB   /* Define "LCD" as a symbolic name for GPIOB */
#define RS 0x40 /* PORTD BIT6 mask */
//#define RW 0x20 /* PORTD BIT5 mask */
#define EN 0x80 /* PORTD BIT7 mask */
#define HIGH 1
#define LOW 0

/*define useful symbolic names for LCD commands */
#define clear_display     0x01
#define returnHome        0x02
#define moveCursorRight   0x06
#define moveCursorLeft    0x08
#define shiftDisplayRight 0x1C
#define shiftDisplayLeft  0x18
#define cursorBlink       0x0F
#define cursorOff         0x0C
#define cursorOn          0x0E
#define Function_set_4bit 0x28
#define Function_set_8bit 0x38
#define Entry_mode        0x06
#define Function_8_bit    0x32
#define Set5x7FontSize    0x20
#define FirstRow          0x80
#define SecondRow          0xC0

/* prototypes of LCD functions */
void delay_ms(int n); /* mili second delay function */
void delay_us(int n); /* micro second delay function */ 
void LCD_init(void);  /* LCD initialization function */
void LCD_Cmd(unsigned char command); /*Used to send commands to LCD */
void LCD_Write_Char(unsigned char data); /* Writes ASCII character */
void LCD_Write_Nibble(unsigned char data, unsigned char control); /* Writes 4-bits */
void LCD_String (char *str);	/* Send string to LCD function */
void LCD_Write_Integer(int x);
char * _float_to_char(float x, char *s);