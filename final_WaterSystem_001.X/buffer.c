/**********************************************************************************************//**
 * \file	D:\source\UMN\EE2361\Labs\otter_lab6_v001.X\otter_buffer.c.
 *
 * \brief	Otter buffer class
 **************************************************************************************************/

#include <stddef.h>

#include "xc.h"
#include "buffer.h"
#define BUFFER_SIZE 1024

volatile int theBuffer[BUFFER_SIZE];
volatile int bufferFront;
int buffSize = 0;

/**********************************************************************************************//**
 * \fn	void putVal(int newValue)
 *
 * \brief	Puts a value
 *
 * \author	Darnell Otterson
 * \date	11/17/2019
 *
 * \param	newValue	The new value.
 **************************************************************************************************/

void putVal(int newValue)
{
    theBuffer[bufferFront++] = newValue;
    if(bufferFront == BUFFER_SIZE)
        bufferFront = 0;
    if(buffSize < BUFFER_SIZE)
        buffSize++;
}

/**********************************************************************************************//**
 * \fn	void putVals(int newValues[4])
 *
 * \brief	Puts the vals
 *
 * \author	Darnell Otterson
 * \date	11/17/2019
 *
 * \param	newValues	The new values.
 **************************************************************************************************/

void putVals(int newValues[4])
{
    theBuffer[bufferFront++] = newValues[0];
    theBuffer[bufferFront++] = newValues[1];
    theBuffer[bufferFront++] = newValues[2];
    theBuffer[bufferFront++] = newValues[3];
    if(bufferFront == BUFFER_SIZE)
        bufferFront = 0;    
}

/**********************************************************************************************//**
 * \fn	int getAvg()
 *
 * \brief	Gets the average
 *
 * \author	Darnell Otterson
 * \date	11/17/2019
 *
 * \returns	The calculated average.
 **************************************************************************************************/

int getAvg()
{
    long int avg = 0;
    int realPos=0;
    int offsetPos=bufferFront-1;
    // For test purposes
    // return theBuffer[offsetPos];
    
    for(realPos=0;realPos<BUFFER_SIZE;realPos++)
    {
        if(theBuffer[offsetPos] != 0){
            avg+=theBuffer[offsetPos];
        }
        if((++offsetPos)==BUFFER_SIZE)
            offsetPos = 0;
    }
    if(buffSize==0)
        return 0;
    
    return (int)(avg/buffSize);
    
}

/**********************************************************************************************//**
 * \fn	int getCount()
 *
 * \brief	Gets the count
 *
 * \author	Darnell Otterson
 * \date	11/17/2019
 *
 * \returns	The count.
 **************************************************************************************************/

int getCount()
{
    return buffSize;
}

/**********************************************************************************************//**
 * \fn	void initBuffer()
 *
 * \brief	Initializes the buffer
 *
 * \author	Darnell Otterson
 * \date	11/17/2019
 **************************************************************************************************/

void initBuffer()
{
    int counter = 0;
    bufferFront = 0;
    for(counter = 0;counter<BUFFER_SIZE;theBuffer[counter++] = 0);
}