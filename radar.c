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
        dopplerMode();
        break;
    }
}

// FMCW mode
void fmcwMode()
{
    P6OUT &= ~(BIT3);
}

// doppler mode
void dopplerMode()
{
    P6OUT |= BIT3;
}
