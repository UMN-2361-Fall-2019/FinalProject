/**
 * \file	C:\Source\FinalProject\final_WaterSystem_001.X\buffer.c.
 *
 * \brief	Buffer class
 */

#include <stddef.h>

#include "xc.h"
#include "buffer.h"
#define BUFFER_SIZE 100

/** \brief	the buffer[ buffer size] */
volatile int theBuffer[BUFFER_SIZE];
/** \brief	The buffer front */
volatile int bufferFront;
/** \brief	Size of the buffer */
int buffSize = 0;
/** \brief	The state */
int state = 0;

/**
 * \fn	void putVal(int newValue)
 *
 * \brief	Puts a value
 *
 * \author	Tyler Krussow, Alex Lema, Darnell Otterson, and Jacob Wenthe
 * \date	12/1/2019
 *
 * \param	newValue	The new value.
 */

void putVal(int newValue)
{
    theBuffer[bufferFront++] = newValue;
    if(bufferFront == BUFFER_SIZE)
        bufferFront = 0;
    if(buffSize < BUFFER_SIZE)
        buffSize++;
}

/**
 * \fn	int getAvg()
 *
 * \brief	Gets the average
 *
 * \author	Tyler Krussow, Alex Lema, Darnell Otterson, and Jacob Wenthe
 * \date	12/1/2019
 *
 * \return	The calculated average.
 */

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

/**
 * \fn	int getCount()
 *
 * \brief	Gets the count
 *
 * \author	Tyler Krussow, Alex Lema, Darnell Otterson, and Jacob Wenthe
 * \date	12/1/2019
 *
 * \return	The count.
 */

int getCount()
{
    return buffSize;
}

/**
 * \fn	int getState(void)
 *
 * \brief	Gets the state
 *
 * \author	Tyler Krussow, Alex Lema, Darnell Otterson, and Jacob Wenthe
 * \date	12/8/2019
 *
 * \return	The state.
 */

int getState(void){
    return state;
}

/**
 * \fn	void setState(int level)
 *
 * \brief	Sets a state
 *
 * \author	Tyler Krussow, Alex Lema, Darnell Otterson, and Jacob Wenthe
 * \date	12/8/2019
 *
 * \param	level	The level.
 */

void setState(int level){
    state = level;
}

/**
 * \fn	void initBuffer()
 *
 * \brief	Initializes the buffer
 *
 * \author	Tyler Krussow, Alex Lema, Darnell Otterson, and Jacob Wenthe
 * \date	12/1/2019
 */

void initBuffer()
{
    int counter = 0;
    bufferFront = 0;
    for(counter = 0;counter<BUFFER_SIZE;theBuffer[counter++] = 0);
}