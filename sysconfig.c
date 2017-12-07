/*
* System Configuration File
* 
* Software Version: 1.1
* Hardware Version: Jun. 2015
*
* Zhengyu Peng
* Texas Tech University
*/

#include "io430.h"
#include "sysconfig.h"

// System Configuration
void sysConfig()
{
    clockConfig();
    gpioConfig();
    interruptConfig();
}

// GPIO Configuration
void gpioConfig()
{
    // test ports
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