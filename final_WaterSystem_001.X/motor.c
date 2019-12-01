/*
 * File:   motor.c
 * Author: rfolk
 *
 * Created on November 28, 2019, 2:28 PM
 */


#include "xc.h"
#include <stdio.h>
#include <string.h>
#include "defs.h"

int motorSpeed = 5; // Value must be between 0 = off, and 10 = high
int motorStep = 0; // This is calculated from the RS value for the Output Compare /10
loggerCallback motorLogger = NULL;

#define MOTORSPEED (motorSpeed*motorStep)

// Private functions
void LogMotorMessage(char* message)
{
    LOGMESSAGE(motorLogger, message);
}

// Public Functions

int initMotor(loggerCallback logger) {
    motorLogger = logger;
    T3CONbits.TON = 0; // Turn off Timer 3
    T3CON = 0x10; // 0x00; //8010; // Set default pr multiplier to 1:8
    PR3 = 40000-1; // Set the servo time
    TMR3 = 0x00; //Clear contents of the timer register
    IPC2bits.T3IP = 5; // set TNR3 interrupt priority
    IFS0bits.T3IF = 0; // clear the interrupt flag
    IEC0bits.T3IE = 1; //Enable Timer3 interrupts

    // Timer 3 setup should happen before this line
    motorStep = PR3/10;
    OC1R = MOTORSPEED; // servo start position. We wont touch OC1R again
    OC1RS = MOTORSPEED; // We will only change this once PWM is turned on
    OC1CONbits.OCTSEL = 1; // Use Timer 3 for compare source
    OC1CONbits.OCM = 0b110; // Output compare PWM w/o faults
    T3CONbits.TON = 1; // Enable timer 3 and get this started

    return 1;
}

void motorOn(){
    // Enable interrupt for PWM
    // OC1RS= MOTORSPEED;  
    IEC0bits.T3IE = 1;
}

void motorOff(){
    // Disable interrupt for PWM
    IEC0bits.T3IE = 0;
}

void setMotorHigh(){
    motorSpeed = 10;
}

void setMotorLow(){
    motorSpeed = 3;
}

int getMotorSpeed(){
    return motorSpeed;
}

int setMotorSpeed(int newSpeed){
    if(newSpeed <0)
        motorSpeed = 0;
    else if(newSpeed > 10)
        motorSpeed = 10;
    else
        motorSpeed = newSpeed;
    return motorSpeed; // returns actual speed set
}

int processMotorMessages(){
    char output[50];
    // Update motor speed on each cycle incase a change has occurred
    if(MOTORSPEED != OC1RS){
        sprintf(output, "Motor: Updating speed from %d to %d",OC1RS, MOTORSPEED);
        LogMotorMessage(output);
    OC1RS= MOTORSPEED; 
    }
    
    return 1;
}

void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt(void)
{
    // TODO: Put interrupt code here
    // Clear timer 3 and reset it
    TMR3 = 0;
    IFS0bits.T3IF = 0;   

}
