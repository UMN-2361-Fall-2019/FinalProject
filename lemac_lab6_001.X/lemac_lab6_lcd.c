/*
 * File:   lemac_lab6_lcd.c
 * Author: Alex
 *
 * Created on November 13, 2019, 2:44 AM
 */
#include "xc.h"
#include "lemac_lab6_lcd.h"
#include "string.h"
                 
void lcd_cmd(char command)
{
    I2C2CONbits.SEN = 1;  // Initiate Start condition
    while(I2C2CONbits.SEN);   // SEN will clear when Start Bit is complete
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
{   delay(50);
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00111001); // function set, extended instruction mode
    lcd_cmd(0b00010100); // interval osc
    lcd_cmd(0b01110000); // contrast C3-C0 
    lcd_cmd(0b01011110); // Ion, Bon, C5-C4 
    lcd_cmd(0b01101100); // follower control
    delay(200);
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00001100); // Display On
    lcd_cmd(0b00000001); // Clear Display
    delay(2);

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
    I2C2CONbits.SEN = 1;  // Initiate Start condition
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
{
    
    int counter_FOR_size_minus_1;
    int sizeSTRING = strlen(s);//SIZEOF()//It takes the size of the string "s"
    
    I2C2CONbits.SEN = 1;  // <Initiate Start condition>
    while(I2C2CONbits.SEN);   //wait// SEN will clear when Start Bit is complete
    IFS3bits.MI2C2IF=0;  // Set when an I2C action is complete
  
    
    I2C2TRN= 0b01111100;// 8-bits consisting of the slave address and the R/nW bit
                        //I2C2TRN = <slave_chip_addr><R/nW>
    while(!IFS3bits.MI2C2IF);   //wait for ACK;
    IFS3bits.MI2C2IF=0;// Set when an I2C action is complete  
    
    
    
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
    
    //For the last bit of the string  
    I2C2TRN = 0b01000000;  // 8-bits consisting of control byte /w RS=1 <CO=0 RS=1>
                           //I2C2TRN = <slave__mem_addr>
    while(!IFS3bits.MI2C2IF);   //wait for ACK;
    IFS3bits.MI2C2IF=0;// Set when an I2C action is complete
    I2C2TRN = s[sizeSTRING-1];;//// 8-bits consisting of the data bytE
                     //I2C2TRN = <data_for_slave>
    while(!IFS3bits.MI2C2IF);   //wait for ACK;
    IFS3bits.MI2C2IF=0;// Set when an I2C action is complete
    
    I2C2CONbits.PEN = 1;  //Send Sto(P) sequence //PEN = 0;  
    while(I2C2CONbits.PEN);//wait  //// PEN will clear when Stop bit is complete*/
}
