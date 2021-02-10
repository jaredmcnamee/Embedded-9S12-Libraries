//Hitachi 44780 initialization and commands
//Processor:  MC9S12XDP512
//Crystal:  16 MHz
//by Jared McNamee
//Oct 2 2019

void LCD_Init(void);			//8-bit, 2-line, 5x8 chars, disp on, curs on, blink off, inc curs mode, no shift, clear, home
void LCD_Ctrl(unsigned char);
unsigned char LCD_Busy(void);
void LCD_Char(unsigned char);
void LCD_Addr(unsigned char);	//raw LCD DDRAM address -- requires knowledge of device
void LCD_Pos(unsigned char,unsigned char);	//Row and Column, zero based; out of range values go to home location
void LCD_String(char *);		//requires a NULL-terminated string of ASCII characters in main program