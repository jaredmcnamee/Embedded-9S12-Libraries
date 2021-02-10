/********************************************************************/
// HC12 Program:  SCI0_Lib.c
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Jared McNamee
// Details:       A more detailed explanation of the program is entered here
                  
// Date:          Date Created
// Revision History :
//  each revision will have a date + desc. of changes

/********************************************************************/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
/********************************************************************/
// Library includes
/********************************************************************/

/********************************************************************/
// Local Prototypes
/********************************************************************/

/********************************************************************/
// Global Variables
/********************************************************************/

int count;
int baud;
/********************************************************************/
// Constants
/********************************************************************/

/********************************************************************/
// Main Entry
/********************************************************************/

/********************************************************************/
// Functions
/********************************************************************/
void SCI0_Init(unsigned long bRate) //initializing to a user specified baud rate
{
    SCI0BD = (80000000/16/bRate +5) /10; //busclock/16/userBaudRate
    SCI0CR1 = 0b00000000;                //initializing Control Register 1
    SCI0CR2 = 0b00001100;                //configuring control register 2
}
void SCI0_Init9600(void)
{
    SCI0BD=52;          //busclock/19200 rounds to 26
    SCI0CR1 = 0b00000000;//initializing Control Register 1
    SCI0CR2 = 0b00001100;//configuring control register 2 
}		
void SCI0_Init19200(void)
{
    SCI0BD=26;          //busclock/19200 rounds to 26
    SCI0CR1 = 0b00000000;//initializing Control Register 1
    SCI0CR2 = 0b00001100;//configuring control register 2
}

void SCI0_TxChar(unsigned char cData)
{
    while(SCI0SR1_TDRE != 1); //checking to see if the register is ready to accept data

    SCI0DRL = cData;    //writing data to the register
}

unsigned char SCI0_RxChar(void)
{
    if(SCI0SR1_RDRF == 1)  //checking to see if the register is ready to recieve data
        return SCI0DRL;    //returning the transfered data
    else
    {
        return '\0';      //otherwise returning null
    }
    
}

void SCI0_TxString(char * cString)
{
    while(*cString != 0)        //watch for NULL terminator
        SCI0_TxChar(*cString++); //iterating through the string and printing the characters to the terminal
}
/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
