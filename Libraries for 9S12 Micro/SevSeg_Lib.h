/********************************************************************/
// HC12 Program:  Header file for SevSeg()
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Jared McNamee
// Details:       A more detailed explanation of the program is entered here
                  
// Date:          Date Created
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
void SevSeg_Init(void);
void SevSeg_Char(unsigned char, unsigned char);		//character and digit address
void SevSeg_BlChar(unsigned char);					//digit address
void SevSeg_BlAll(void);
void SevSeg_dChar(unsigned char, unsigned char);	//character and digit address
void SevSeg_Top4(unsigned int);						//four chars compressed as four nibbles in an int
void SevSeg_Bot4(unsigned int);						//four chars compressed as four nibbles in an int
void SevSeg_Cust(unsigned char, unsigned char);		//selected segments and digit address
void SevSeg_Two(unsigned char, unsigned char);
void SevSeg_Twod(unsigned char, unsigned char);
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

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
