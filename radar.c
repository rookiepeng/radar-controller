/*
* Radar Configuration File
*
* Software Version: 1.1
* Hardware Version: Jun. 2015
*
* Zhengyu Peng
* Texas Tech University
*/

#include "io430.h"
#include "radar.h"

// Radar Mode Switch
void radarMode(char mode)
{
    switch (mode)
    {
    case 0x00:
        fmcwMode();
        break;
    default:
        interferometryMode();
        break;
    }
}

// FMCW mode
void fmcwMode()
{
    //P4OUT &= ~ BIT6;
    P6OUT &= ~(BIT3);
}

// INTERFEROMETRY mode
void interferometryMode()
{
    //P4OUT |= BIT6;
    P6OUT |= BIT3;
}
