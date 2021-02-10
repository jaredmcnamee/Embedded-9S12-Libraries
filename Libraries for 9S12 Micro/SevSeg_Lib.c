/********************************************************************/
// HC12 Program:  SevSeg_Lib - the library that controls the Seven Segement display
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Jared McNamee
// Details:       
                  
// Date:          9/27/2019
// Revision History :
//  each revision will have a date + desc. of changes

/********************************************************************/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
/********************************************************************/
// Library includes
/********************************************************************/

// your includes go here
//#include "misc.h"
//#include "muxled.h"
//#include "sw_led.h"
  #include "SevSeg_Lib.h"

/********************************************************************/
// Local Prototypes
/********************************************************************/

/********************************************************************/
// Global Variables
/********************************************************************/
int i;
/********************************************************************/
// Constants
/********************************************************************/

/********************************************************************/
// Main Entry
/********************************************************************/

/********************************************************************/
// Functions
/********************************************************************/
  
  //initialize the displays
  void SevSeg_Init(void){
      PORTA |= 0b00000011; //prset control line high
      DDRA  |= 0b00000011; //A0:1 oupts, A1 - mode, A0 - /write
      DDRB   = 0b11111111; //DataBus

      SevSeg_BlAll();      //blanking the display
  }
  
  
  //function to display a digit to a SevSeg ( data you want to add, digit you want to display to)
  void SevSeg_Char(unsigned char cData, unsigned char cDigit){
      PORTB = cDigit & 0b00000111; //clearing the address of any other extraneous data
      PORTB |= 0b01011000; //single digit,hex,decode,no shutdown,bankA, information
      PORTA_PA0 = 0; //strobe not write with mode high (A1)
      PORTA_PA0 = 1;

      PORTB = cData & 0b00001111; //clean up the hex value
      PORTB |= 0b10000000; //turn off decimal point
      PORTA &= 0b11111100; //both mand and /w low
      PORTA |= 0b00000011; //back to resting state
  }
  
  //function that will clear the sevseg it is called to
  void SevSeg_BlChar(unsigned char cDigit){
      cDigit &= 0b00000111;//clearing the address of any other extraneous data
      cDigit |= 0b01111000;//single digit, (don't care) , no decode, no SD, code be decoding ,no data coming, zero
      PORTB = cDigit;
      PORTA &= 0b11111110;//mode stays high, strobe /Write
      PORTA |= 0b00000011;//back to resting stat - MOde andd /Write high
      
      PORTB  = 0b10000000;//turn off all segments and dp
      PORTA &= 0b11111100;//Mode low for data, strob /Write
      PORTA |= 0b00000011;//back to resting state - Mode and /Write high
  }
  
  //function that will blank every sevseg in the array
  void SevSeg_BlAll()
  {
    for ( i = 0; i < 8; i++)
    {
      SevSeg_BlChar(i);//iterating through ever SevSeg and clearing it
    }
    
  }
  
  //fucntion for displaying a char to a display with the decimal point
  void SevSeg_dChar(unsigned char cData, unsigned char cDigit)
  {
      PORTB = cDigit & 0b00000111; //clearing the address of any other extraneous data
      PORTB |= 0b01011000; //single digit,hex,decode,no shutdown,bankA, information
      PORTA_PA0 = 0; //strobe not write with mode high (A1)
      PORTA_PA0 = 1;

      PORTB = cData & 0b00001111; //clean up the hex value
      PORTA &= 0b11111100; //both mand and /w low
      PORTA |= 0b00000011; //back to resting state
  }
  
  //function that will display the input value to the top 4 sevseg displays
  void SevSeg_Top4(unsigned int cData)
  {
    for ( i = 0; i < 4; i++)  //iterating throught the digits in the input
    {
      SevSeg_Char((unsigned char)cData, 3 - i);  //writing the values right to left on the display
      cData >>= 4;                      //bit shifting to the next digit
    }
  }
  
  //function that will display the input value to the bottom 4 sevseg displays
  void SevSeg_Bot4(unsigned int cData)
  {
    for(i = 7; i > 3; i--)  //iterating throught the digits in the input
    {
      SevSeg_Char((unsigned char)cData, i ); //writing the values right to left on the display
      cData >>= 4;                  //bit shifting to the next digit
    }
  }
  
  //displaying a custom character to the seven segment display
  void SevSeg_Cust(unsigned char cData, unsigned char cDigit)
  {
      PORTB = cDigit & 0b00000111; //clearing the address of any other extraneous data
      PORTB |= 0b01111000; //single digit,hex, no decode,no shutdown,bankA, information
      PORTA_PA0 = 0; //strobe not write with mode high (A1)
      PORTA_PA0 = 1;

      PORTB = cData; //displaying the character to the sev seg
      
      PORTA &= 0b11111100; //both mand and /w low
      PORTA |= 0b00000011; //back to resting state
  }

void SevSeg_Two(unsigned char cChars, unsigned char cDigit)
{
  SevSeg_Char((cChars/16), cDigit++);
  SevSeg_Char((cChars&0b00001111), cDigit);
}
void SevSeg_Twod(unsigned char cChars, unsigned char cDigit)
{
  SevSeg_Char((cChars/16), cDigit++);
  SevSeg_dChar((cChars&0b00001111), cDigit);
}

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
