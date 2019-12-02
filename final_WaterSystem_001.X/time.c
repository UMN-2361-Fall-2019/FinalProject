/**
 * \file	C:\Source\UMN\EE2361\Labs\otter_lab5_v001.X\time.c.
 *
 * \brief	Time class.
 */

#include "xc.h"
#include "support.h"

/**
 * \fn	void Delay(int milleseconds)
 *
 * \brief	Delays processing.
 *
 * \author	Tyler Krussow, Alex Lema, Darnell Otterson, and Jacob Wenthe
 * \date	11/6/2019
 *
 * \remarks
 *		Example: Delay(20);
 * \param	milleseconds	The delay in milleseconds.
 */

void Delay(int milleseconds){
    int counter = 0;
    for(counter = 0; counter < milleseconds; counter++){
        wait_1mS();
    }
}