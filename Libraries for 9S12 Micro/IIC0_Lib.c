//IICO Library files
//Processor: MC9S12XDP512
//Crystal: 16 MHZ
//by Jared McNamee
//November 29 2019

#include <hidef.h>
#include "derivative.h"
#include "IIC0_Lib.h"


/*void IIC0_Init(void)
{
    char count;
    IIC0_IBCR &= 0b01111111; //try to turn off module
    DDRJ |= 0b10000000; // manulally control scl line
    for (count;count < 20; count++)
    {
        asm LDAA #13; //5us half a period
        asm DBNE A,*;
        PTJ ^= 0b10000000;
    }
    DDRJ &= 0b01111111; 
    IIC0_IBFD = 0x47; // 100khz, SDA hold = 20 cks, SCL Hold Start = 32 SCL Hold Stop = 42
    IIC0_IBCR|=0b10000000; //enable the bus - must be done first
    IIC0_IBCR&=0b10111110;//no interrrupts, normal WAi
}*/
void IIC0_WriteDAC(unsigned char cAddr, unsigned char cCommand, unsigned int iData)
{
    iData *= 16; //move data into the upper 12 bits

    while(IIC0_IBSR & 0b00100000); //wait for not busy flag
    IIC0_IBCR |= 0b00110000; //micro as master, start transmitting

    IIC0_IBDR = cAddr & 0b11111110; //place address on bus with /write
    while(!(IIC0_IBSR & 0b00000010));//wait for flag
    IIC0_IBSR |= 0b00000010; //clear flag

    IIC0_IBDR = cCommand; //send the desired command
    while(!(IIC0_IBSR & 0b00000010)); //wait for flag
    IIC0_IBSR |= 0b00000010; //clear flag

    IIC0_IBDR = (unsigned char)(iData/256); //send first unsigned char of the data
    while(!(IIC0_IBSR & 0b00000010)); //wait for flag
    IIC0_IBSR |= 0b00000010; //clear flag

    IIC0_IBDR = (unsigned char)(iData&0b0000000011111111);
    while(!(IIC0_IBSR & 0b00000010)); //wait for flag
    IIC0_IBCR &= 0b11001111; //stop transmitting, exit Master mode
    IIC0_IBSR |= 0b00000010; //clear flag
}
void IIC0_Write(unsigned char cAddr, unsigned char cReg, unsigned char cData)
{
    while(IIC0_IBSR&0b00100000); //wait for not busy flag
    IIC0_IBCR |= 0b00110000; //micro as master, start transmitting

    IIC0_IBDR = cAddr & 0b11111110;//place address on bus with /write
    while(!(IIC0_IBSR & 0b00000010)); //wait for flag
    IIC0_IBSR |= 0b00000010; //clear flag

    IIC0_IBDR = cReg; //locate desired register
    while(!(IIC0_IBSR & 0b00000010)); //wait for flag
    IIC0_IBSR |= 0b00000010; //clear flag

    IIC0_IBDR = cData; //send data
    while(!(IIC0_IBSR & 0b00000010)); //wait for flag
    IIC0_IBCR &= 0b11001111; //stop transmitting, exit master mode
    IIC0_IBSR |= 0b00000010; //clear flag
}
unsigned char IIC0_Read(unsigned char cAddr, unsigned char cReg)
{
    unsigned char cData;

    while(IIC0_IBSR&0b00100000); //wait for not busy flag
    IIC0_IBCR |= 0b00110000; //micro as master, start transmitting

    IIC0_IBDR = cAddr & 0b11111110; //place address on bus with /write
    while(!(IIC0_IBSR & 0b00000010)); //wait for flag
    IIC0_IBSR |= 0b00000010; //clear flag

    IIC0_IBDR = cReg; //locate desired register
    while(!(IIC0_IBSR & 0b00000010)); //wait for flag
    IIC0_IBSR |= 0b00000010; //clear flag

    IIC0_IBCR |= 0b00000100; //restart

    IIC0_IBDR = (cAddr | 0b00000001); //place address on bus with Read
    while(!(IIC0_IBSR & 0b00000010)); //wait for flag
    IIC0_IBSR |= 0b00000010; //clear flag

    IIC0_IBCR |= 0b00001000; //reading 1 unsigned char only
    IIC0_IBCR &= 0b11101111; //receive unsigned char
    cData = IIC0_IBDR; //not actually --start the process
    while(!(IIC0_IBSR & 0b00000010)); //wait for flag
    IIC0_IBCR &= 0b11011111; //stop
    IIC0_IBSR |= 0b00000010; //clear flag
    cData = IIC0_IBDR; //for real this time

    return cData;
}

void IIC0_Init(void)
{
  IIC0_IBCR&=0b01111111;  //kill the bus if doing a reset
  
  DDRJ|=0b10000000;       //manually access SCL
  
  asm LDX #20;            //twenty times 5 us is enough to clock out 10 bits
  asm BigLoop:
    asm LDAA #10;
    asm DBNE A,*;         //5 us in a toggle for 100kHz clk
    PTJ^=0b10000000;      //toggle SCL
  asm DBNE X,BigLoop;

  DDRJ&=0b01111111;       //free up SCL

  IIC0_IBFD=0x47;	  //100 kHz, SDA Hold = 20 cks, SCL Hold Start = 32 SCL Hold Stop = 42
  IIC0_IBCR|=0b10000000;  //enable the bus - must be done first
  IIC0_IBCR&=0b10111110;  //no interrupts, normal WAI
}


/************The following are genericized routines that can be used************/
/************for any device, given the timing diagrams provided*****************/
/************Borrowed heavily from Simon Walker, with some variations***********/

int IIC0_WaitForBus(void)           //function prevents lockup if bus isn't available
{                                   //returns FFFF after 10000 tries if not successful
  int iSpinCount = 0;
  
  while (++iSpinCount < 10000)
    if (!(IIC0_IBSR_IBB))           //IBB must be zero (bus free)
      return 0; 
  
  return -1;
}


//This routine expects the advertized IIC address, not left justified!!!!
int IIC0_Addr(char Address, char Read, char Restart, char WaitForBus) //don't wait for bus after Restart
{
    if (WaitForBus)
    {      
      if (IIC0_WaitForBus()) return -1;   //bus is busy -- deal with it in main.c
    }
    if(Restart)IIC0_IBCR_RSTA = 1;        //got bus -- restart if desired
    else IIC0_IBCR |= 0b00110000;         // master mode, transmitting (start)    

    if (!Read)
      IIC0_IBDR = Address*2 & 0b11111110; //send slave address w/write
    else
      IIC0_IBDR = Address*2 | 0x00000001; //send slave address w/read
      
    while (!IIC0_IBSR_IBIF);              //wait for completion

    IIC0_IBSR_IBIF = 1;                   //clear interrupt/flag
    
    
    if (!(IIC0_IBSR_RXAK)) return 0;      //all devices ack on the address announcement
    else IIC0_IBCR &=0b11001111;          //not master, not transmitting = stop 
    return -1;                            //likely no device at that address to respond
}  


int IIC0_WriteByte(char Val, char Stop)
{
  int iSpinCount = 0;

  IIC0_IBDR = Val;                
  
  // wait for interrupt flag w/bail on timeout
  while ((!(IIC0_IBSR_IBIF)) && (++iSpinCount < 10000));
  if (iSpinCount >= 10000) return -1;
  
  // no timeout, got flag, issue stop and clear flag (must be in this order)
  if (Stop) IIC0_IBCR&=0b11001111;            //not master, not transmitting = stop                   
  
  IIC0_IBSR_IBIF = 1;           
  
  // no error
  return 0;
}


int IIC0_ReadByte(char *buff, char Ack, char Stop)
//requires an 8-bit buffer in the main program; Restart for devices with calls to internal registers
//this function follows an address call and maybe a register call
{
  char Junk;    //needed for first fake-read of register
  
  if(Ack) IIC0_IBCR_TXAK = 0;  //on
  else IIC0_IBCR_TXAK = 1;     //off
  
  IIC0_IBCR &= 0b11101111;    //enable receive
  
  Junk = IIC0_IBDR;           //fake read
  
  while(!(IIC0_IBSR_IBIF));   //wait for operation complete flag
  
  if(Stop) IIC0_IBCR_MS_SL = 0;
  
  IIC0_IBSR_IBIF = 1;         //clear flag first
  *buff = IIC0_IBDR;          //real read
  
  return 0;                   //success
}