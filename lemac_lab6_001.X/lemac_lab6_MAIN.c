/*
 * File:   lemac_lab6_MAIN.c
 * Author: Alex
 *
 * Created on November 13, 2019, 2:44 AM
 */


#include "xc.h"
#include "lemac_lab6_lcd.h"
#include "lemac_lab6_Buffer_H.h"
//#include "lemac_servoHe.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include"stdlib.h"

#pragma config ICS = PGx1 // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF // JTAG Port Enable (JTAG port is disabled)

// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME // Clock Switching and Monitor (Clock switching is enabled,
                                // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))



void delay(int delay_in_ms) // delay in miliseconds 
{
    while(delay_in_ms>0)
    {
      lem_wait_1ms(); // this funtion works with assembly file and header 
      delay_in_ms--; // counter/ decrement 
    }//end while(delay_in_ms>0) 
}//end delay



 void setup() 
 {
    CLKDIVbits.RCDIV = 0;
 
    AD1PCFG = 0x9ffe; //all digital inputs and outputs
    TRISA |= 0x0001;
    
    I2C2BRG = 157;
    I2C2CONbits.I2CEN = 1;
    _I2CSIDL = 0;
    IFS3bits.MI2C2IF=0;
    
    lcd_init();
    initBuffer();
    //initServo();
   
    
    AD1CON1 = 0;    // use default settings, AND turn off A/D for now
    AD1CON1bits.SSRC = 0b010;  // timer 3 rollover stops sampling, starts conversion
    AD1CON1bits.ASAM = 1; // auto-sample
 
    
    AD1CON2 = 0;
    AD1CON2bits.CSCNA = 0; // turn off scan 0 = Use the channel selected by the CH0SA bits as the MUX A input
    AD1CON2bits.SMPI = 0b0111; // 0000 = Interrupts at the completion of conversion for each sample/convert sequence
    AD1CON2bits.BUFM = 0;  // two 8-word buffers 0 = Buffer configured as one 16-word buffer (ADC1BUF0 to ADC1BUFF)
    
    AD1CON3bits.ADCS = 1;
    AD1CON3bits.ADRC = 0;
    AD1CON3bits.SAMC = 2;
    /*
    AD1CON2 = 0;
    AD1CON2bits.CSCNA = 0; // turn on scan
    AD1CON2bits.SMPI = 0; // interrupt after 8 conversions
    AD1CON2bits.BUFM = 0;  // two 8-word buffers

    AD1CON3bits.ADCS = 1; // Tad = 2*Tcy = 125ns
    AD1CON3bits.ADRC = 0;
    AD1CON3bits.SAMC = 2; // sample time = 125ns
 
     */
    
    AD1CON1bits.ADON = 1; // turn on module
    
    AD1CHS = 0;
    AD1CHSbits.CH0SA = 0; //// channel selected is channel 0 (doesn't matter for scan)

    //3333333333333333
    T3CON = 0;
    TMR3 = 0;
    T3CONbits.TCKPS = 0b10;
    PR3 =10;//15624;//1/16
     //1562
    T3CONbits.TON = 1;
    //2222222222222222222222222
    T2CON = 0;
    TMR2 = 0;
    T2CONbits.TCKPS = 0b10;
    PR2 = 300;//24999;  // 1 second
    //25000 
    // Configure Timer 2 for display every 100 milliseconds
    // 1/((62.5*(10^(-9)))*256) = 62500 using 256 prescaler
 
    T2CONbits.TON = 1;
    IEC0bits.T2IE = 1; // enable interrupt on T2 completion
    IFS0bits.T2IF = 0;
    //ADCCCCCCCCCCCCCCCCCCCCCC
    IEC0bits.AD1IE = 1; // enable interrupt on ADC completion
    IFS0bits.AD1IF = 0;


}


void __attribute__((__interrupt__, __auto_psv__)) _ADC1Interrupt(void) {
    putVal(ADC1BUF0);
  //  AD1CON1bits.SAMP = 1;   // start sampling
    _AD1IF = 0;
 
}

void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void) {
    _T2IF = 0;
  //  TMR2 = 0;
}



void setServo(int Val){
    OC1RS= Val;  
}
int main(void) {
    setup();
    //initServo();
    unsigned long adValue;
    //char adStr[20]; 
    char str[20];    //create an empty string to store number
    float number=1234;
    //int my_int;
//   my_int = (int)my_float;
    while(1) {
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
            //setServo(1000);//maxinum left 90
            //delay(10000);
            //setServo(3000);
        }   
        else if((number<=8)&&(number>4))
        {
           
            lcd_setCursor(1, 0);
            lcd_printStr("NORMAL  ");
            setServo(1000);//maxinum left 90
         //   delay(10000);
           // setServo(3000); 
        }
        else if((number<=4)&&(number>=0))
        {
            lcd_setCursor(1, 0);
            lcd_printStr(" WET   ");
        }    
        _T2IF=0;
        }
    }
}
 
 