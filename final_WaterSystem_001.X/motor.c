/*
 * File:   motor.c
 * Author: Tyler Krussow, Alex Lema, Darnell Otterson, and Jacob Wenthe
 *
 * Created on November 28, 2019, 2:28 PM
 */


#include "xc.h"
#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "buffer.h"

/** \brief	Value must be between 0 = off, and 10 = high */
int motorSpeed = 5;
/** \brief	This is calculated from the RS value for the Output Compare /10 */
int motorStep = 0;
/** \brief	The motor logger */
loggerCallback motorLogger = NULL;

#define MOTORSPEED (motorSpeed*motorStep)

/**
 * \fn	void LogMotorMessage(char* message)
 *
 * \brief	Logs motor message
 *
 * \author	Tyler Krussow, Alex Lema, Darnell Otterson, and Jacob Wenthe
 * \date	12/1/2019
 *
 * \param [in,out]	message	If non-null, the message.
 */

void LogMotorMessage(char* message)
{
    LOGMESSAGE(motorLogger, message);
}

/**
 * \fn	int initMotor(loggerCallback logger)
 *
 * \brief	Initializes the motor
 *
 * \author	Tyler Krussow, Alex Lema, Darnell Otterson, and Jacob Wenthe
 * \date	12/1/2019
 *
 * \param	logger	The logger.
 *
 * \return	An int.
 */

void initMotor(loggerCallback logger){
   TRISBbits.TRISB6 = 0;       //Set RB6 to output
   
    //set TMR3 for 1 second
    T3CON = 0x0010;             //Prescalar 1:256, resolution: 16 microseconds
    TMR3 = 0;
    _T3IF = 0;
    PR3 = 62499;                //(62499+1)*256*62.5 ns = 1 s
    T3CONbits.TON = 1;
    
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPOR3bits.RP6R = 18;        // Use Pin RP6 for Output Compare 1 = "18" (Table 10-3)
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock PPS
    
    OC1CON = 0;
    OC1R = 0;                   // Initialize the start PWM to 0 s
    OC1RS = 0;
    OC1CONbits.OCTSEL = 1;      // Use Timer 3 for compare source
    OC1CONbits.OCM = 0b110;     // Output compare PWM w/o faults
    return;
}

void pumpSet(int level){
    switch (level){
        case 0:
            OC1RS = 0;          // Disable motor
            break;
        case 1:
            OC1RS = 20625;      // Pump is on for 0.33s at a time
            break;
        case 2:
            OC1RS = 41875;      // Pump is on for 0.67s at a time
            break;
        case 3:
            OC1RS = 62500;      // Pump is on for 1 seconds (out of 1 second period)
            break;
    }
    return;
}

/**
 * \fn	int processMotorMessages()
 *
 * \brief	Process the motor messages
 *
 * \author	Tyler Krussow, Alex Lema, Darnell Otterson, and Jacob Wenthe
 * \date	12/1/2019
 *
 * \return	An int.
 */

int processMotorMessages(){
       int newState = getState();
    
    if(newState){
            pumpSet(3);             //Set motor to pump 67% of the time
    }
    else
    {
            pumpSet(0);             //Turn off motor
    }
       return 1;
}

/**
 * \fn	void __attribute__ _T3Interrupt(void)
 *
 * \brief	Timer 3 interrupt
 *
 * \author	Tyler Krussow, Alex Lema, Darnell Otterson, and Jacob Wenthe
 * \date	12/1/2019
 */

void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt(void)
{
    // TODO: Put interrupt code here
    // Clear timer 3 and reset it
    TMR3 = 0;
    IFS0bits.T3IF = 0;   

}
