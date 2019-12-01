/*
 * File:   display.c
 * Author: rfolk
 *
 * Created on November 28, 2019, 2:29 PM
 */


#include "xc.h"
#include <string.h>
#include <stdio.h>
#include "defs.h"
#include "time.h"
#include "buffer.h"

loggerCallback displayLogger = NULL;
// Private functions
void logDisplayMessage(char* message)
{
    LOGMESSAGE(displayLogger, message);
}

void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void) {
    _T2IF = 0;
  //  TMR2 = 0;
}

// Public Functions

void lcd_cmd(char command)
{
    I2C2CONbits.SEN = 1;  ///star////Initiate Start condition
    while(I2C2CONbits.SEN);   //wait// SEN will clear when Start Bit is complete
    IFS3bits.MI2C2IF=0;  // Set when an I2C action is complete
    
    I2C2TRN= 0b01111100;// 8-bits consisting of the slave address and the R/nW bit
                        //I2C2TRN = <slave_chip_addr><R/nW>
    while(!IFS3bits.MI2C2IF);   //wait for ACK;
    IFS3bits.MI2C2IF=0;// Set when an I2C action is complete  
    
    I2C2TRN = 0b00000000;  ///// 8-bits consisting of control byte 
                           //I2C2TRN = <slave__mem_addr>
    while(!IFS3bits.MI2C2IF);   //wait for ACK;
    IFS3bits.MI2C2IF=0;// Set when an I2C action is complete
    
    I2C2TRN = command;//// 8-bits consisting of the data bytE
                     //I2C2TRN = <data_for_slave>
    while(!IFS3bits.MI2C2IF);   //wait for ACK;
    IFS3bits.MI2C2IF=0;// Set when an I2C action is complete
    
    I2C2CONbits.PEN = 1;  //Send Sto(P) sequence //PEN = 1;  
    while(I2C2CONbits.PEN);//wait  //// PEN will clear when Stop bit is complete*/
}

void lcd_init(void)
{   AD1PCFG &= 0x9FFE;
    TRISA |= 0x0001;
    I2C2BRG = 157;
    I2C2CONbits.I2CEN = 1;
    _I2CSIDL = 0;
    IFS3bits.MI2C2IF=0;

    Delay(50);
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00111001); // function set, extended instruction mode
    lcd_cmd(0b00010100); // interval osc
    lcd_cmd(0b01110000); // contrast C3-C0 
    lcd_cmd(0b01011110); // Ion, Bon, C5-C4 
    lcd_cmd(0b01101100); // follower control
    Delay(200);
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00001100); // Display On
    lcd_cmd(0b00000001); // Clear Display
    Delay(2);

}

void lcd_setCursor(char x, char y) {

    lcd_cmd(0b10000000 | (0x40 * y + x));
    /*         ^                 ^    ^
               |                 |    |
               |                 |    |
            < MSB>           // <row>// <column>
   
    Notice however, that the MSB is always 1>><0b10000000>
    An equation can map this relationship.  
    For example, the address to write row 1, column 5; <0x40*(row) + (column)> = 0x45
    page 13 EE2361 - Lab5 - I2C LCD - Background*/
}

void lcd_printChar(char myChar) 
{
    I2C2CONbits.SEN = 1;  ///star////<Initiate Start condition>
    while(I2C2CONbits.SEN);   //wait// SEN will clear when Start Bit is complete
    IFS3bits.MI2C2IF=0;  // Set when an I2C action is complete
    I2C2TRN= 0b01111100;// 8-bits consisting of the slave address and the R/nW bit
                        //I2C2TRN = <slave_chip_addr><R/nW>
    while(!IFS3bits.MI2C2IF);   //wait for ACK;
    IFS3bits.MI2C2IF=0;// Set when an I2C action is complete  
    
    I2C2TRN = 0b01000000;  // 8-bits consisting of control byte /w RS=1 
                           //I2C2TRN = <slave__mem_addr>
    while(!IFS3bits.MI2C2IF);   //wait for ACK;
    IFS3bits.MI2C2IF=0;// Set when an I2C action is complete
    I2C2TRN = myChar;//// 8-bits consisting of the data bytE
                     //I2C2TRN = <data_for_slave>
    while(!IFS3bits.MI2C2IF);   //wait for ACK;
    IFS3bits.MI2C2IF=0;// Set when an I2C action is complete
   
    I2C2CONbits.PEN = 0;  //Send Sto(P) sequence //PEN = 0;  
    while(!I2C2CONbits.PEN);//wait  //// PEN will clear when Stop bit is complete*/
}//*/

void lcd_printStr(const char *s) 
{   ///https://stackoverflow.com/questions/17131863/passing-string-to-a-function-in-c-with-or-without-pointers
    
    int counter_FOR_size_minus_1;
    int sizeSTRING = strlen(s);//SIZEOF()//It takes the size of the string "s"
    
    I2C2CONbits.SEN = 1;  ///star////<Initiate Start condition>
    while(I2C2CONbits.SEN);   //wait// SEN will clear when Start Bit is complete
    IFS3bits.MI2C2IF=0;  // Set when an I2C action is complete
  
    
    I2C2TRN= 0b01111100;// 8-bits consisting of the slave address and the R/nW bit
                        //I2C2TRN = <slave_chip_addr><R/nW>
    while(!IFS3bits.MI2C2IF);   //wait for ACK;
    IFS3bits.MI2C2IF=0;// Set when an I2C action is complete  
    
    
    
    /////////////<///////////////////////
    //All the bits of the string, beside the last one
    for ( counter_FOR_size_minus_1 = 0; counter_FOR_size_minus_1 < sizeSTRING - 1; counter_FOR_size_minus_1++) //
    {
    I2C2TRN = 0b11000000;  // 8-bits consisting of control byte /w RS=1 <CO=1 RS=1>
                           //I2C2TRN = <slave__mem_addr>
    while(!IFS3bits.MI2C2IF);   //wait for ACK;
    IFS3bits.MI2C2IF=0;// Set when an I2C action is complete
    //DATA
    I2C2TRN = s[counter_FOR_size_minus_1];//// 8-bits consisting of the data bytE
                     //I2C2TRN = <data_for_slave>
    while(!IFS3bits.MI2C2IF);   //wait for ACK;
    IFS3bits.MI2C2IF=0;// Set when an I2C action is complete<0 NO complite>
    }
    ///////////////////////////////////////////
    
    
    //For the last bit of the string  
    I2C2TRN = 0b01000000;  // 8-bits consisting of control byte /w RS=1 <CO=0 RS=1>
                           //I2C2TRN = <slave__mem_addr>
    while(!IFS3bits.MI2C2IF);   //wait for ACK;
    IFS3bits.MI2C2IF=0;// Set when an I2C action is complete
    I2C2TRN = s[sizeSTRING-1];;//// 8-bits consisting of the data bytE
                     //I2C2TRN = <data_for_slave>
    while(!IFS3bits.MI2C2IF);   //wait for ACK;
    IFS3bits.MI2C2IF=0;// Set when an I2C action is complete
    //////////////////////////////// 
    
    
   
    
    I2C2CONbits.PEN = 1;  //Send Sto(P) sequence //PEN = 0;  
    while(I2C2CONbits.PEN);//wait  //// PEN will clear when Stop bit is complete*/
/////////////////////////////////////////////////
}//*/

int initDisplay(loggerCallback logger) {
    displayLogger = logger;
    T2CONbits.TON = 1;
    IEC0bits.T2IE = 1; // enable interrupt on T2 completion
    IFS0bits.T2IF = 0;
    T2CON = 0;
    TMR2 = 0;
    T2CONbits.TCKPS = 0b10;
    PR2 = 300;    
    lcd_init();
    return 1;
}

int processDisplayMessages(){
    // Display information here when new info is available
    int adValue = 0;
    double number = 0.0f;
    char str[40];
    if(_T2IF) 
     {
     adValue = getAvg(); //adValue = ADC1BUF0;
     number=(3.3/1023)*adValue;
     number= 10*number/3.3;
     sprintf(str, "%4.0f", number); 
     lcd_setCursor(1,1);
     lcd_printStr(str);
     if((number<=10)&&(number>8))
     {
         lcd_setCursor(1, 0);   
         lcd_printStr(" DRY   ");
         logDisplayMessage("DRY");
         //setServo(1000);//maxinum left 90
         //delay(10000);
         //setServo(3000);
     }   
     else if((number<=8)&&(number>4))
     {

         lcd_setCursor(1, 0);
         lcd_printStr("NORMAL  ");
         logDisplayMessage("NORMAL");

      //   delay(10000);
        // setServo(3000); 
     }
     else if((number<=4)&&(number>=0))
     {
         lcd_setCursor(1, 0);
         lcd_printStr(" WET   ");
         logDisplayMessage("WET");
     }    
     _T2IF=0;
     }

    return 1;
}
