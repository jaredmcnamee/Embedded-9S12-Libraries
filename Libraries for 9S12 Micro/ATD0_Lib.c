//ATD0 Library Files
//Processor: MC9S12XDP512
//Crystal: 16 MHZ
//by Jared McNamee
//November 15 2019

#include <hidef.h>
#include "derivative.h"
#include "ATD0_Lib.h"

void ATD0_Init(void)
{
    DDR1AD0  = 0b00000000; //enable all channels as input
    ATD0DIEN = 0b00000000; //ensure they are Analog

    ATD0CTL2 = 0b11000000;
/*               ||||||||
                 ||||||| --Interrupt flag (input - don't care)
                 |||||| ---Interrupt disabled 
                 ||||| ----External trigger disabled
                 |||| -----External trigger polarity (don't care)
                 ||| ------External trigger interrupt disabled 
                 || -------ATD continues in wait mode
                 | --------Fast flag -- clears on read
                  ---------ATD power up
*/

    asm LDX #134;          //need a 50 us delay before continuing
    asm DBNE X,*;

    ATD0CTL3 = 0b00001010;
/*               ||||||||
                 ||||||| --\
                 |||||| ---/finish conversion before freezing
                 ||||| ----result maps into corresponding register
                 |||| -----\
                 ||| ------ \
                 || ------- / 8 conversion per sequence
                 | --------/
                  ---------don't care
*/
    ATD0CTL4 = 0b00000001;
/*               ||||||||
                 ||||||| --\
                 |||||| --- \
                 ||||| ----  \ bus clock divide by 14 - 571.4 khz (1.75 us period)
                 |||| ----- /  with 4 clocks per sample, this is 7 us per sample
                 ||| ------/   which is the required minimum
                 || -------\
                 | --------/ 4 A/D conversion clock periods per sample
                  ---------10-bit resolution
*/
    ATD0CTL5 = 0b10100000;
/*               ||||||||
                 ||||||| --\ starts filling the results registers
                 |||||| --- / at the bottom, mapping channelx to DRx 
                 ||||| ----/
                 |||| -----(don't care)
                 ||| ------sample all 0 channels
                 || -------continuous scan conversion
                 | --------unsigned (single quadrant)
                  ---------right-justified
*/
}
unsigned int ATD0_AN0(void)
{
    
    while(!(ATD0STAT0&0x80)); //wait for conversion to complete
                                //and return to calling program
                                //with sample in result register 0
    
    return ATD0DR0;
}
unsigned int ATD0_Read(char chan);