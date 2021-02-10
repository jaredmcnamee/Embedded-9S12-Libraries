/********************************************************************/
// HC12 Program:  SwLED_Lib.h - header file for the SwLED
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Jared McNamee
// Details:       Hold the prototypes for SwLED_Lib
                  
// Date:          Sept, 11 2019
// Revision History :
//  each revision will have a date + desc. of changes

/********************************************************************/
//#include <hidef.h>      /* common defines and macros */
//#include "derivative.h" /* derivative-specific definitions */
/********************************************************************/
// Library includes
/********************************************************************/

// your includes go here
//#include "misc.h"
//#include "muxled.h"
//#include "sw_led.h"

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
void SwLED_Init(void); //LEDs as outputs, switches as inputs, dig inputs enabled
void LED_On(char); //accepts R, G, Y, A (for all)
void LED_Off(char); //accepts R, G, Y, A (for all)
void LED_Tog(char); //accepts R, G, Y, A (for all)
char Sw_Ck(void); //returns debounced condition of all switches in a byte, LED values = 0;
/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
