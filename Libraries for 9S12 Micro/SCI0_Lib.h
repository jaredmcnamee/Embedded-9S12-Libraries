//Com port SCI0 initialization and commands
//Processor:  MC9S12XDP512
//Crystal:  16 MHz
//by Jared McNamee
//Oct 2019

void SCI0_Init(unsigned long);	//any valid baud rate can be passed to this; 8-bit, 1 Stop, No parity, no interrupts
void SCI0_Init9600(void);		//8-bit, 1 Stop, No parity, no interrupts
void SCI0_Init19200(void);		//8-bit, 1 stop, No parity, no interrupts
void SCI0_TxChar(unsigned char);
unsigned char SCI0_RxChar(void);	//Non-blocking; returns NULL if no new valid character is available
void SCI0_TxString(char *);		//Requires a NULL-terminated ASCII string in the main program
