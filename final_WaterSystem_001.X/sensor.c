/*
 * File:   LM393.c
 * Author: rfolk
 *
 * Created on November 28, 2019, 2:21 PM
 */


#include "xc.h"
#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "buffer.h"

int lowThreshold = 3;
int highTHreshold = 8;

loggerCallback sensorLogger = NULL;


// Private functions
void LogSensorMessage(char* message)
{
    LOGMESSAGE(sensorLogger, message);
}

void __attribute__((__interrupt__, __auto_psv__)) _ADC1Interrupt(void) {
    putVal(ADC1BUF0);
  //  AD1CON1bits.SAMP = 1;   // start sampling
    _AD1IF = 0;
 
}

// Public Functions

void setThresholds(int low, int high){
    char output[50];
    lowThreshold = low;
    highTHreshold = high;
    
    sprintf(output, "Sensor: Setting thresholds: %d to %d", low, high);
    LogSensorMessage(output);
}

int getLowThreshold(){
    return lowThreshold;
}

int getHighThreshold(){
    return highTHreshold;
}

int initSensor(loggerCallback logger){
    sensorLogger = logger;
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


    return 1;
}

int processSensorMessages(){
    
    return 1;
}