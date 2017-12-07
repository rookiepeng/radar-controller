/**
 * 
 * MIT License
 * 
 * Copyright (c) 2017 Zhengyu Peng
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 **/

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
