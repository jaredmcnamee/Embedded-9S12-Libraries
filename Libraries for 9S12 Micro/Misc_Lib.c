/********************************************************************/
// HC12 Program:  Misc_Lib - various useful functions that don't easily fit into one category
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Jared McNamee
// Details:       
                  
// Date:          Sept 27 2019
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
#include "Misc_Lib.h"

/********************************************************************/
// Local Prototypes
/********************************************************************/

/********************************************************************/
// Global Variables
/********************************************************************/

/********************************************************************/
// Constants
/********************************************************************/

/********************************************************************/
// Main Entry
/********************************************************************/


/********************************************************************/
// Functions
/********************************************************************/
unsigned int HexToBCD(unsigned int iHexIn)
{
    unsigned int iBCDOut = 0;  //the output in BCD
    unsigned char cCount;      //the counter that represents the digit we are on
    unsigned int iPow = 1;     //the power of 16 we are working with currently

    
        for(cCount = 0; cCount<4;cCount++)    //four digits in a loop
        {
            if(iHexIn <= 0x270f)              //converter can only accept number less than 9999
            {
                iBCDOut+=(iHexIn%10)*iPow;   //Division-remainder conversion
                iHexIn/=10;
                iPow*=16;                    //16^cCount
            }
            else                             //if the value is too big giving a 0000 as an error and leaving the subroutine
                {
                    iBCDOut=0;
                    break;
                }
        }

    
    return iBCDOut;
}
//BCDToHex handles numbers up to 4 digits (9999), returns Hex as 2 bytes (int)
//does not require the math.h C library
unsigned int BCDToHex(unsigned int iBCDIn)
{
    unsigned int iHexOut = 0;     //the Hexidecimal output 
    unsigned char cDigit,cCount;  //cDigit = the current digit we are working on| cCount = the digit in the display we are working on
    unsigned int iPow = 1;        //the current power of ten

    for(cCount=0; cCount<4;cCount++)
    {
        cDigit=iBCDIn%16; //locate right-most digit
        if(cDigit<10)     //not valid BCD (0-9)? might as well quit!
        {
            iHexOut+=cDigit*iPow; //multiply by correct power of 10 and add
            iBCDIn/=16;           //get ready for next digit
            iPow*=10;             //10^bcount
        }
        else
        {
            iHexOut=0;            //error? return zero
            break;                //...and break out of loop
        }
    }
    return iHexOut;               //returning the converted hex number
}

unsigned char ToUpper(unsigned char cData)
{
  if(cData < 0x61 || cData > 0x7a )  //if the passed value is not between the correct values
  {
      return cData; //escaping the function
  }
  cData -= 0x20;  //subtracting a hex value to make into uppercase

  return cData;   //returning uppercase value
}
unsigned char ToLower(unsigned char cData)
{
    if (cData < 0x41 || cData > 0x5a) //if the passed value is not between the correct values
    {
        return cData; //escaping the function
    }
    cData += 0x20;  //adding a hex value to make into lowercase
    
    return cData;   //returning lowercase value
}
unsigned char HexToASCII(unsigned char cData)
{
    if(cData > 0x0f)                            //if the value is larger than 15 breaking out of the program
        return 0;
    if(cData >= 0 && cData <= 9)                //if the value is between 0 and 9 
    {
        cData += 0x30;                          //adding the appropriate hex value to make the character an ascii character
        return cData;                           //returning the new value
    }    
    if(cData >= 0x0a && cData <= 0x0f)          //if the value is a letter
    {
        cData += 0x37;                          //adding the appropriate hex value to make the character an ascii character
        return cData;                           //returning the new value
    } 
}	
unsigned char ASCIIToHex(unsigned char cData)
{
    if(cData >= 0x30 && cData <= 0x39)          //if the value is between 0 and 9
    {
        cData -= 0x30;                          //removing the appropriate hex value to make to decode the ascii
        return cData;                           //returning the new value
    }
    cData = ToUpper(cData);                     //converting the date to uppercase
    if(cData >= 0x41 && cData <= 0x46)          //if its between A and F
    {
        cData -= 0x37;                          //removing the appropriate hex value to make to decode the ascii
        return cData;                           //returning the new value
    }
    return 0;                                   //if the the character makes it this far returning zero
}

void TimInit8us(void)
{
    TSCR1 |= 0b10000000; //enable timer module
    TSCR2 &= 0b11111000; //set prescale to bus/64 (8 us per tick)
    TSCR2 |= 0b00000110; //...continued
    TIOS  |= 0b00000001; //set IOS0 to output compare
    TCTL2 &= 0b11111100; //set PT0 to toggle mode
    TCTL2 |= 0b00000001; //...continued
    TFLG1  = 0b00000001; //clear flag
}
void TimInit125ns(void)
{
    TSCR1 |= 0b10000000; //enable timer module
    TSCR2 &= 0b11111000; //set prescale to bus/1 (8 us per tick)
    
    TIOS  |= 0b00000001; //set IOS0 to output compare
    TCTL2 &= 0b11111100; //set PT0 to toggle mode
    TCTL2 |= 0b00000001; //...continued
    TFLG1  = 0b00000001; //clear flag 
}
void Sleep_ms(unsigned int iTime) //requires TimInit8us()
{
    unsigned int iCount;
    TC0=TCNT+125;                 //first target -- 125 counts at 8 us = 1 ms
    for(iCount = 1; iCount<=iTime;iCount++)
    {
        TFLG1  = 0b00000001;      //clear flag
        while((TFLG1&0b00000001) == 0); //blocking wait for flag
        TC0+=125;                 //next target -- based on previous target for accuracy
    }
}
/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
