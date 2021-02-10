/********************************************************************/
// HC12 Program:  SwLED_Lib.c - c file for the SwLED
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Jared McNamee
// Details:       Contains functions for initializing the Switches and LEDS
                  
// Date:          Sept, 11 2019
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
#include "SwLED_Lib.h"

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
void SwLED_Init(void)   //LEDs as outputs, switches as inputs, dig inputs enabled
{
  DDR1AD1=0b11100000;     //LEDs as outputs,switches as inputs
  ATD1DIEN1=0b00011111;   //Digitially initlaizing the switches
  PT1AD1&=0b000111111;    //Turn off all LEDs as inital condition
}
void LED_On(char x) //accepts R, G, Y, A 
{
   switch(x) {
    case 'R':
      PT1AD1 |= 0b10000000; //turning the red LED on
    break;
    case 'Y':
      PT1AD1 |= 0b01000000; //turning the yellow LED on
    break;
    case 'G':
      PT1AD1 |= 0b00100000; //turning the green  LED on
    break;
    case 'A':
      PT1AD1 |= 0b11100000; //turning all the LEDs on
    break;
   }
      
}
void LED_Off(char x) //accepts R, G, Y, A 
{
    switch(x){
      case 'R':
        PT1AD1 &= 0b01111111; //turning off the Red LED
      break;
      case 'Y':
        PT1AD1 &= 0b10111111; //turning off the Yellow LED
      break;
      case 'G':
        PT1AD1 &= 0b11011111;   //turning off the Green LED
      break;
      case 'A':
        PT1AD1 &= 0b00011111;   //turning off all the LEDs
      break;
    }
}
void LED_Tog(char x) //accepts R, G, Y, A 
{

    switch(x){
      case 'R':
        PT1AD1 ^= 0b10000000; //toggling the Red LED
      break;
      case 'Y':
        PT1AD1 ^= 0b01000000; //toggling the Yellow LED
      break;
      case 'G':
        PT1AD1 ^= 0b00100000; //toggling the Green LED
      break;
      case 'A':
        PT1AD1 ^= 0b11100000; //toggling All the LEDs
    
    }
}
char Sw_Ck(void)
{
    char cSample1=1; //itializing two variables that will be different to lock into the while loop
    char cSample2=0;

    while(cSample1!=cSample2)
    {
        cSample1=PT1AD1&0b00011111;//loading the value of PT1AD1 into the first var
        asm LDX     #13334; // waiting 5ms
        asm DBNE    X,*;
        cSample2=PT1AD1&0b00011111;//loading the same value of PT1AD1 into var 2 breaking loop
    }
    return cSample1;//returning debounced value
}
/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
