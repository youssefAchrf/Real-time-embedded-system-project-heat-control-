#include "lcd.h"

/* LCD and GPIOB initialization Function */ 
void LCD_init(void)
{
 SYSCTL_RCGCGPIO_R |=(1<<1); /* Enable Clock to GPIOB */
 GPIO_PORTB_DIR_R |=0xFF; /* Set GPIOB all pins a digital output pins */
 GPIO_PORTB_DEN_R |=0xFF; /* Declare GPIOB pins as digital pins */

 LCD_Cmd(Set5x7FontSize);  /* select 5x7 font size and 2 rows of LCD */
 LCD_Cmd(Function_set_4bit); /* Select 4-bit Mode of LCD */
 LCD_Cmd(clear_display); /* clear whatever is written on display */
 
}

void LCD_Cmd(unsigned char command)
{
    LCD_Write_Nibble(command >> 4, 0);   /* Write upper nibble to LCD */
    LCD_Write_Nibble(command, 0);     /* Write lower nibble to LCD */
    
    if (command < 4)
        delay_ms(2);         /* 2ms delay for commands 1 and 2 */
    else
        delay_us(40);        /* 40us delay for other commands */
}


void LCD_Write_Nibble(unsigned char data, unsigned char control)
{

    data &= 0x0F;       /* Extract lower nibble for data */
    control &= 0xF0;    /* Extract upper nibble for control */
		GPIO_PORTB_DATA_R = data | control; 
    GPIO_PORTB_DATA_R= data | control | EN;  /* Provide Pulse to Enable pin to perform wite operation */
    delay_us(0);
    GPIO_PORTB_DATA_R = data; /*Send data */
    GPIO_PORTB_DATA_R= 0; /* stop writing data to LCD */
}
void LCD_Write_Char(unsigned char data)
{
    LCD_Write_Nibble(data >> 4, RS);    /* Write upper nibble to LCD and RS = 1 to write data */
    LCD_Write_Nibble(data, RS);      /* Write lower nibble to LCD and RS = 1 to write data */
    delay_us(40);
}


void LCD_String (char *str)	/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)  /* Send each char of string till the NULL */
	{
		LCD_Write_Char(str[i]);  /* Call LCD data write */
	}
}

/* Mili seconds delay function */
void delay_ms(int n)
{
 volatile int i,j;
 for(i=0;i<n;i++)
 for(j=0;j<3180;j++)
 {}
}

/* Micro seconds delay function */
void delay_us(int n)
{
 volatile int i,j;
 for(i=0;i<n;i++)
 for(j=0;j<3;j++)
 {}
 
}
void LCD_Write_Integer(int x)
{
	
	char mySize = 0;
	char Str[32];
	
	if (x == 0)
	{
		LCD_Write_Char('0');
	}
	/* 
	* looping to add each digit in array form least significant to most significant ()
	* if number is 1 2 3 4 5 
	* it will be stored 5 4 3 2 1 
	*/
	else
	{
		while (x != 0)
		{
			Str[mySize]=x%10;
			x=x/10;
			mySize++;
		}
	
	/*
	* Loop to print the assigned numbers in an array and print it reversed (From most to least)
	* if the array has stored values 5 4 3 2 1 
	*	it will print 1 2 3 4 5
	*/
		for (int i=mySize-1;i>=0;i--)
		{
			LCD_Write_Char(48+Str[i]);
		}
	}
}
char * _float_to_char(float x, char *s) {
    uint16_t decimals = (int)(x * 100) % 100;
    int units = (int)x;

    s[4] = (decimals % 10) + '0';
    decimals /= 10; // repeat for as many decimal places as you need
    s[3] = (decimals % 10) + '0';
    s[2] = '.';

		s[1] = (units % 10) + '0';
		units /= 10;
	  if (units>0){
		s[0] = (units % 10) + '0';
		units /= 10;
		}else{
		s[0] = ' ';
		}
    return s;
}