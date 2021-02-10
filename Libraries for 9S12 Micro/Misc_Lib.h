//Miscellaneous generally-useful routines - header file for Misc_Lib
//Processor:  MC9S12XDP512
//Crystal:  16 MHz
//by Jared McNamee
//September 2019

//Binary-Coded Decimal conversion routines

unsigned int HexToBCD(unsigned int);	//integer math; result is BCD - not converted to ASCII; make it 4-digits for sev-seg display
unsigned int BCDToHex(unsigned int);	//integer math; requires BCD - not ASCII equivalent; make it 4-digits to complement HexToBCD


//ASCII-Code handling routines

unsigned char ToUpper(unsigned char);       //converts to uppercase
unsigned char ToLower(unsigned char);       //converts to lowercase
unsigned char HexToASCII(unsigned char);	//single character converter to ascii char
unsigned char ASCIIToHex(unsigned char);	//single character converter to hex char


//9S12X simple timer routines

void TimInit125ns(void);
void TimInit8us(void);
void Sleep_ms(unsigned int);			//requires TimInit8us setup; blocking delay

