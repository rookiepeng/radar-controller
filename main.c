/*
* Controller for 5.8-GHz & 24-GHz multi-mode radar systems
*
* Software Version: 1.0
* Hardware Version: Jun. 2015
*
* Zhengyu Peng
* Texas Tech University
*/

#include "io430.h"
#include "sysconfig.h"
#include "radar.h"

const char FMCW = 0x00;    // FMCW mode
const char DOPPLER = 0x01; // Doppler mode with low-IF modulation
char modeFlag;

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer to prevent time out reset

    sysConfig(); // System Configration

    if (P2IN & 0x10) // FMCW mode when pin 2.4 is high
    {
        modeFlag = FMCW;
        radarMode(modeFlag);
    }
    else // Doppler mode when pin 2.4 is low
    {
        modeFlag = DOPPLER;
        radarMode(modeFlag);
    }

    __bis_SR_register(LPM3_bits + GIE); // Enter LPM3, interrupts enabled

    return 0;
}

// Timer A0 interrupt service routine
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{
    //P2OUT ^= BIT4;     // Freq = 32768 / 2 / offset
}

// Timer_A1 Interrupt Vector (TAIV) handler
#pragma vector = TIMERA1_VECTOR
__interrupt void Timer_A1(void)
{
    switch (TAIV)
    {
    case 2:          // control the mixer switch in INTERFEROMETRY mode
        CCR1 += 512; // Freq = 32768 / 2 / offset
        if (modeFlag != FMCW)
        {
            P6OUT ^= (BIT4 | BIT5); // switch mixer
        }
        else
        {
            P6OUT |= (BIT4 | BIT5);
        }
        break;
    case 4:
        break;
    case 10:
        break;
    }
}
