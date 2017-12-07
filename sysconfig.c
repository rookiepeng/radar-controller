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

// System Configuration
void sysConfig()
{
    clockConfig();
    gpioConfig();
    interruptConfig();

    P2OUT &= ~BIT5;
    P6OUT |= (BIT4 | BIT5);
}

// GPIO Configuration
void gpioConfig()
{
    // Mode selection ports
    P2SEL &= ~(BIT4 | BIT5);
    P2DIR &= ~BIT4;
    P2DIR |= BIT5;

    // Output ports
    P6SEL &= ~(BIT3 | BIT4 | BIT5);
    P6DIR |= (BIT3 | BIT4 | BIT5);
}

// MCU Clock Configuration
void clockConfig()
{
    if (CALBC1_8MHZ == 0xFF) // If calibration constant erased
    {
        while (1)
            ; // do not load, trap CPU!!
    }
    DCOCTL = 0;            // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_8MHZ; // Set DCO to 8MHz
    DCOCTL = CALDCO_8MHZ;

    // Select 32kHz Crystal for ACLK
    // BCSCTL1 &= (~XTS);	// ACLK = LFXT1CLK
    // BCSCTL2 &= ~(BIT4|BIT5);	// 32768Hz crystal on LFXT1

    // Clock output
    //            MSP430F261x/241x
    //             -----------------
    //         /|\|              XIN|-
    //          | |                 | 32kHz
    //          --|RST          XOUT|-
    //            |                 |
    //            |             P5.6|-->ACLK = 32kHz
    //            |             P5.5|-->SMCLK = 8MHz
    //            |             P5.4|-->MCLK = DCO
    //            |             P5.3|-->MCLK/10
    // P5DIR |= 0x78;
    // P5SEL |= 0x70;
}

// System Interrupt Configuration
void interruptConfig()
{
    CCTL0 = CCIE; // CCR0 interrupt enabled
    CCTL1 = CCIE; // CCR1 interrupt enabled
    CCTL2 = CCIE; // CCR2 interrupt enabled
    CCR0 = 10;
    CCR1 = 0;
    CCR2 = 0;
    TACTL = TASSEL_1 + MC_2 + TAIE; // ACLK, contmode, TAIE enabled
}
