/********************************************************************/
// HC12 Program:  YourProg - MiniExplanation
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        This B. You
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

// your includes go here
//#include "misc.h"
//#include "muxled.h"
//#include "sw_led.h"
#include "LCD_Lib.h"

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
void LCD_Init(void)
{
    PTH    =0b00000000;
    DDRH   =0b11111111; //data bus as output for write
    PORTK &=0b11111000; //preset RS low, R/W low, En Low
    DDRK  |=0b00000111; //activate three control lines

    asm LDD #0;   //need a 49.15 ms delay
    asm DBNE D,*; //24.576 ms delay
    asm DBNE D,*; //...twice

    PTH =0b00111000; //function set of commands, data: 8-bit(high),2(high),font: 5x8 matrix (low)

    PORTK|=0b00000001; //write a control byte
    PORTK&=0b11111000; //resting state
    /*              |||____En(Enable: High to enable)
                    ||_____R/W(LOW for WRITE)
                    |______RS (Register Select: LOW for control)
    */
    asm LDD  #11000;   //need 4.125 ms delay
    asm DBNE D,*;

    PORTK|=0b00000001; //RS low, R/W low, EN high to write a control
    PORTK&=0b11111000; //resting state

    asm LDD #267;      //need 100 us delay
    asm DBNE D,*;

    PORTK|=0b00000001; //RS low, R/W low, En high to write a control
    PORTK&=0b11111000; //resting state

    asm LDD #267;      //need 100 us delay
    asm DBNE D,*;

    LCD_Ctrl(0b00111000); // same as a bove, but using LCD_Ctrol (Busy is active)

    LCD_Ctrl(0b00001110); //display controls
    /*             ||||___Blink:   LOW for off
                   |||____Cursor:  High for on
                   ||_____Display: High for on
                   |______Display Control commands   
    */
    LCD_Ctrl(0b00000001); //clear display, home position
    LCD_Ctrl(0b00000110); //mode controls
    /*              |||__Shift: Low for no display shift
                    ||___Inc/Dec: High for increment (to the left)
                    |____Entry Mode commands
    */
}

void LCD_Ctrl(unsigned char cCommand)
{
    while(LCD_Busy() != 0); //wait for the Busy Flag to be CLEARED
    PTH=cCommand;           //put control byte on the bus

    PORTK|=0b00000001;      //RS low, R/W low, En high to write a control
    PORTK&=0b11111000;      //resting state
}

unsigned char LCD_Busy(void)
{
    unsigned char cBusy;    

    DDRH=0b00000000;        //data bus as inputs for read

    PORTK|=0b00000011; 
    /*              |||_____EN (Enable: HIGH to enable)
                    ||______R/W(HIGH for Read)
                    |_______RS (Register Select: Low for Status)    
    */
    PORTK&=0b11111000;      //resting state
    cBusy=PTH&0b10000000;   //Busy Flag is the MSB of the Status Register
    DDRH = 0b11111111;      //data bus returned to outputs for next write

    return cBusy;
}

void LCD_Char(unsigned char cData)
{
    while(LCD_Busy() != 0); //wait for the Busy Flag to be CLEARED
    PTH=cData;              //put the data on the bus

    PORTK|=0b00000101;      //RS High, R/W low, En high to write a control
    PORTK&=0b11111000;      //resting state
}
void LCD_Addr(unsigned char cAddr)
{
    LCD_Ctrl(cAddr|0b10000000); //sending the address to write to through LCD Ctrl
}

void LCD_String(char * cString)
{
    while(*cString != 0)        //watch for NULL terminator
        LCD_Char(*cString++);   //send next character
}

void LCD_Pos(unsigned char cRow,unsigned char cCol)
{                           //zero-based for both row and column
    if(cRow >3 || cCol>19)  //display invalid inputs at home location
    {
        LCD_Addr(0);
    }
    else
    {
        switch (cRow)
        {
            case 0:         //first row is 0 to 19
            LCD_Addr(cCol);
            break;

            case 1:
            LCD_Addr(cCol+64);//second row is 64 to 83
            break;

            case 2:
            LCD_Addr(cCol+20); //third row is 20 to 63
            break;

            case 3:
            LCD_Addr(cCol+84);//fourth row is 84 to 103
            break;
        }
    }
    
}
/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
