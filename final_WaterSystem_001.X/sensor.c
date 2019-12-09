/**
 * \file	C:\Source\FinalProject\final_WaterSystem_001.X\sensor.c.
 *
 * \brief	Sensor class
 */

#include "xc.h"
#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "buffer.h"

/** \brief	The low threshold */
int lowThreshold = 6;
/** \brief	The high t hreshold */
int highTHreshold = 9;

loggerCallback sensorLogger = NULL;


// Private functions

/**
 * \fn	void LogSensorMessage(char* message)
 *
 * \brief	Logs sensor message
 *
 * \author	Tyler Krussow, Alex Lema, Darnell Otterson, and Jacob Wenthe
 * \date	12/1/2019
 *
 * \param [in,out]	message	If non-null, the message.
 */

void LogSensorMessage(char* message)
{
    LOGMESSAGE(sensorLogger, message);
}

/**
 * \fn	void __attribute__ _ADC1Interrupt(void)
 *
 * \brief	ADC 1 interrupt
 *
 * \author	Tyler Krussow, Alex Lema, Darnell Otterson, and Jacob Wenthe
 * \date	12/1/2019
 */

void __attribute__((__interrupt__, __auto_psv__)) _ADC1Interrupt(void) {
    putVal(ADC1BUF0);
  //  AD1CON1bits.SAMP = 1;   // start sampling
    _AD1IF = 0;
 
}


/**
 * \fn	int initSensor(loggerCallback logger)
 *
 * \brief	Initializes the sensor
 *
 * \author	Tyler Krussow, Alex Lema, Darnell Otterson, and Jacob Wenthe
 * \date	12/1/2019
 *
 * \param	logger	The logger.
 *
 * \return	An int.
 */

int initSensor(loggerCallback logger){
    sensorLogger = logger;
	initBuffer(); // Initialize the buffer here
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
    
    AD1CON1bits.ADON = 1; // turn on module
    
    AD1CHS = 0;
    AD1CHSbits.CH0SA = 0;   // channel selected is channel 0 (doesn't matter for scan)
    _AD1IF = 0;             //Reset Flag
    _AD1IE = 1;             //Enable Interrupt for ADC


    return 1;
}

/**
 * \fn	int processSensorMessages()
 *
 * \brief	Process the sensor messages
 *
 * \author	Tyler Krussow, Alex Lema, Darnell Otterson, and Jacob Wenthe
 * \date	12/1/2019
 *
 * \return	An int.
 */

int processSensorMessages(){
    // Nothing to do here
    return 1;
}