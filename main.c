/*
* MCU board for hybrid portable radar system
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
const char DOPPLER = 0x01; // INTERFEROMETRY mode
char modeFlag = 0;         // FMCW: modeFlag==0, INTERFEROMETRY mode: other cases

int main(void)
{
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;

    // System Configration
    sysConfig();

    radarMode(modeFlag);
    P2OUT &= ~BIT5;
    P6OUT |= (BIT4 | BIT5);

    if (P2IN & 0x10)
    {
        modeFlag = FMCW;
        radarMode(modeFlag);
    }
    else
    {
        modeFlag = DOPPLER;
        radarMode(modeFlag);
    }

    // Enter LPM3, interrupts enabled
    __bis_SR_register(LPM3_bits + GIE);

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
        //modeFlag = ( modeFlag + 1 ) % 2;
        //radarMode( modeFlag );
        break;
    case 4:
        //CCR2 += 5000	// Freq = 32768 / 2 / offset
        break;
    case 10: // switch between INTERFEROMETRY mode and FMCW mode
        //modeFlag = ( modeFlag + 1 ) % 4;	// Freq = 0.25 Hz
        //radarMode( modeFlag );
        break;
    }
}
