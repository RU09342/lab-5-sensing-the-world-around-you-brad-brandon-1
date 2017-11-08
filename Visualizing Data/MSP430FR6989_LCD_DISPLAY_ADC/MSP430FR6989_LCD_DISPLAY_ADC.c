/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 * 
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP430FR69xx Demo - eUSCI_A0 UART echo at 9600 baud using BRCLK = 8MHz
//
//  Description: This demo echoes back characters received via a PC serial port.
//  SMCLK/ DCO is used as a clock source and the device is put in LPM3
//  The auto-clock enable feature is used by the eUSCI and SMCLK is turned off
//  when the UART is idle and turned on when a receive edge is detected.
//  Note that level shifter hardware is needed to shift between RS232 and MSP
//  voltage levels.
//
//  The example code shows proper initialization of registers
//  and interrupts to receive and transmit data.
//  To test code in LPM3, disconnect the debugger.
//
//  ACLK = VLO, MCLK =  DCO = SMCLK = 8MHz
//
//                MSP430FR6989
//             -----------------
//       RST -|     P2.0/UCA0TXD|----> PC (echo)
//            |                 |
//            |                 |
//            |     P2.1/UCA0RXD|<---- PC
//            |                 |
//
//   William Goh
//   Texas Instruments Inc.
//   April 2014
//   Built with IAR Embedded Workbench V5.60 & Code Composer Studio V6.0
//******************************************************************************
#include <msp430.h>
#include "LCDDriver.h"
long temp;
void LCDInit();
int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop Watchdog Timer
  LCDInit();                                // Call LCD Initialization Function
  // Configure GPIO
  P2SEL0 |= BIT0 | BIT1;                    // USCI_A0 UART operation
  P2SEL1 &= ~(BIT0 | BIT1);
  P8SEL1 |= BIT4;                           // Configure P8.4 for ADC
  P8SEL0 |= BIT4;


  PM5CTL0 &= ~LOCKLPM5;                     // Disable the GPIO power-on default high-impedance mode to activate
                                            // previously configured port settings

  // Startup clock system with max DCO setting ~8MHz
  CSCTL0_H = CSKEY >> 8;                    // Unlock clock registers
  CSCTL1 = DCOFSEL_3 | DCORSEL;             // Set DCO to 8MHz
  CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
  CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
  CSCTL0_H = 0;                             // Lock CS registers

  // Configure USCI_A0 for UART mode
  UCA0CTLW0 = UCSWRST;                      // Put eUSCI in reset
  UCA0CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK
  // Baud Rate calculation
  // 8000000/(16*9600) = 52.083
  // Fractional portion = 0.083
  // User's Guide Table 21-4: UCBRSx = 0x04
  // UCBRFx = int ( (52.083-52)*16) = 1
  UCA0BR0 = 52;                              // 8000000/16/9600
  UCA0BR1 = 0x00;
  UCA0MCTLW |= UCOS16 | UCBRF_1 | 0x4900;
  UCA0CTLW0 &= ~UCSWRST;                     // Initialize eUSCI
  UCA0IE |= UCRXIE;                          // Enable USCI_A0 RX interrupt

  // By default, REFMSTR=1 => REFCTL is used to configure the internal reference
    //while(REFCTL0 & REFGENBUSY);              // If ref generator busy, WAIT
    //REFCTL0 |= REFVSEL_0 | REFON;             // Select internal ref = 1.2V
                                              // Internal Reference ON
    // Configure ADC12
    ADC12CTL0 = ADC12SHT0_2 | ADC12ON;
    ADC12CTL1 = ADC12SHP;                     // ADCCLK = MODOSC; sampling timer
    ADC12CTL2 |= ADC12RES_2;                  // 12-bit conversion results
    ADC12IER0 |= ADC12IE0;                    // Enable ADC conv complete interrupt
    ADC12MCTL0 |= ADC12INCH_8; // A1 ADC input select pin 8.4; Vref= external reference 3.3V

    //while(!(REFCTL0 & REFGENRDY));            // Wait for reference generator
                                              // to settle

    while(1)
    {
      //__delay_cycles(5000);                   // Delay between conversions
        temp = ADC12MEM0;
      ADC12CTL0 |= ADC12ENC | ADC12SC;        // Sampling and conversion start
       __bis_SR_register(CPUOFF | GIE);    // Enter LPM3, interrupts enabled

      __no_operation();                       // For Debugger
    }
}

//#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
  switch(__even_in_range(UCA0IV,USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
      while(!(UCA0IFG&UCTXIFG));
      if(UCA0RXBUF == 'A')                           // Upon Typing the Letter A
      {
          UCA0TXBUF = ((ADC12MEM0>>12)&(0x00FF));    // Transmit Part of ADC12MEM0 Back to PC for Verification
          showChar(((ADC12MEM0>>12)+48), 3);         // Truncate and Display Part of ADC12MEM0 on LCD Digit 3

          while(!(UCA0IFG&UCTXIFG));
          UCA0TXBUF = ((ADC12MEM0>>8)&(0x00FF));
          switch (((ADC12MEM0>>8)&(0x00FF)))         // Switch statement to handle display of letters on digit 4
          {
              case 10:showChar('A',4);break;
              case 11:showChar('B',4);break;
              case 12:showChar('C',4);break;
              case 13:showChar('D',4);break;
              case 14:showChar('E',4);break;
              case 15:showChar('F',4);break;
              default: showChar((((ADC12MEM0>>8)&(0x00FF))+48), 4);  // Truncate and Display Part of ADC12MEM0 on LCD Digit 4
          }
          while(!(UCA0IFG&UCTXIFG));
          UCA0TXBUF = ((ADC12MEM0>>4)&(0x00FF));
          switch (((ADC12MEM0>>4)&(0x000F)))        // Switch statement to handle display of letters on digit 5
          {
              case 10:showChar('A',5);break;
              case 11:showChar('B',5);break;
              case 12:showChar('C',5);break;
              case 13:showChar('D',5);break;
              case 14:showChar('E',5);break;
              case 15:showChar('F',5);break;
              default: showChar((((ADC12MEM0>>4)&(0x000F))+48), 5);  // Truncate and Display Hex Value from ADC12MEM0 on LCD Digit 6
          }
          while(!(UCA0IFG&UCTXIFG));
          UCA0TXBUF = ((ADC12MEM0)&(0x00FF));
          switch (((ADC12MEM0)&(0x000F)))
          {
              case 10:showChar('A',6);break;
              case 11:showChar('B',6);break;
              case 12:showChar('C',6);break;
              case 13:showChar('D',6);break;
              case 14:showChar('E',6);break;
              case 15:showChar('F',6);break;
              default: showChar((((ADC12MEM0)&(0x000F))+48), 6);  // Truncate and Display Character on LCD Digit 6
          }
        showChar('0', 1);                           // Automatically Set Digit 1 to 0 to display hex numbers in proper format
        showChar('x', 2);                           // Automatically Set Digit 2 to x to display hex numbers in proper format
           }
      else
          UCA0TXBUF = 0;                            // If A is Not Sent Over UART, Just Return 0 (Should Never Happen)
      break;
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
    default: break;
  }
}
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch (__even_in_range(ADC12IV, ADC12IV_ADC12RDYIFG))
  {
    case ADC12IV_NONE:        break;        // Vector  0:  No interrupt
    case ADC12IV_ADC12OVIFG:  break;        // Vector  2:  ADC12MEMx Overflow
    case ADC12IV_ADC12TOVIFG: break;        // Vector  4:  Conversion time overflow
    case ADC12IV_ADC12HIIFG:  break;        // Vector  6:  ADC12BHI
    case ADC12IV_ADC12LOIFG:  break;        // Vector  8:  ADC12BLO
    case ADC12IV_ADC12INIFG:  break;        // Vector 10:  ADC12BIN
    case ADC12IV_ADC12IFG0:                // Vector 12:  ADC12MEM0 Interrupt
        __bic_SR_register_on_exit(LPM0_bits); // Exit active CPU
      break;                                // Clear CPUOFF bit from 0(SR)
    case ADC12IV_ADC12IFG1:   break;        // Vector 14:  ADC12MEM1
    case ADC12IV_ADC12IFG2:   break;        // Vector 16:  ADC12MEM2
    case ADC12IV_ADC12IFG3:   break;        // Vector 18:  ADC12MEM3
    case ADC12IV_ADC12IFG4:   break;        // Vector 20:  ADC12MEM4
    case ADC12IV_ADC12IFG5:   break;        // Vector 22:  ADC12MEM5
    case ADC12IV_ADC12IFG6:   break;        // Vector 24:  ADC12MEM6
    case ADC12IV_ADC12IFG7:   break;        // Vector 26:  ADC12MEM7
    case ADC12IV_ADC12IFG8:   break;        // Vector 28:  ADC12MEM8
    case ADC12IV_ADC12IFG9:   break;        // Vector 30:  ADC12MEM9
    case ADC12IV_ADC12IFG10:  break;        // Vector 32:  ADC12MEM10
    case ADC12IV_ADC12IFG11:  break;        // Vector 34:  ADC12MEM11
    case ADC12IV_ADC12IFG12:  break;        // Vector 36:  ADC12MEM12
    case ADC12IV_ADC12IFG13:  break;        // Vector 38:  ADC12MEM13
    case ADC12IV_ADC12IFG14:  break;        // Vector 40:  ADC12MEM14
    case ADC12IV_ADC12IFG15:  break;        // Vector 42:  ADC12MEM15
    case ADC12IV_ADC12IFG16:  break;        // Vector 44:  ADC12MEM16
    case ADC12IV_ADC12IFG17:  break;        // Vector 46:  ADC12MEM17
    case ADC12IV_ADC12IFG18:  break;        // Vector 48:  ADC12MEM18
    case ADC12IV_ADC12IFG19:  break;        // Vector 50:  ADC12MEM19
    case ADC12IV_ADC12IFG20:  break;        // Vector 52:  ADC12MEM20
    case ADC12IV_ADC12IFG21:  break;        // Vector 54:  ADC12MEM21
    case ADC12IV_ADC12IFG22:  break;        // Vector 56:  ADC12MEM22
    case ADC12IV_ADC12IFG23:  break;        // Vector 58:  ADC12MEM23
    case ADC12IV_ADC12IFG24:  break;        // Vector 60:  ADC12MEM24
    case ADC12IV_ADC12IFG25:  break;        // Vector 62:  ADC12MEM25
    case ADC12IV_ADC12IFG26:  break;        // Vector 64:  ADC12MEM26
    case ADC12IV_ADC12IFG27:  break;        // Vector 66:  ADC12MEM27
    case ADC12IV_ADC12IFG28:  break;        // Vector 68:  ADC12MEM28
    case ADC12IV_ADC12IFG29:  break;        // Vector 70:  ADC12MEM29
    case ADC12IV_ADC12IFG30:  break;        // Vector 72:  ADC12MEM30
    case ADC12IV_ADC12IFG31:  break;        // Vector 74:  ADC12MEM31
    case ADC12IV_ADC12RDYIFG: break;        // Vector 76:  ADC12RDY
    default: break;
  }
}

void LCDInit()
{
    PJSEL0 = BIT4 | BIT5;                   // For LFXT

    // Initialize LCD segments 0 - 21; 26 - 43
    LCDCPCTL0 = 0xFFFF;
    LCDCPCTL1 = 0xFC3F;
    LCDCPCTL2 = 0x0FFF;

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    //PM5CTL0 &= ~LOCKLPM5;

    // Configure LFXT 32kHz crystal
    CSCTL0_H = CSKEY >> 8;                  // Unlock CS registers
    CSCTL4 &= ~LFXTOFF;                     // Enable LFXT
    do
    {
      CSCTL5 &= ~LFXTOFFG;                  // Clear LFXT fault flag
      SFRIFG1 &= ~OFIFG;
    }while (SFRIFG1 & OFIFG);               // Test oscillator fault flag
    CSCTL0_H = 0;                           // Lock CS registers

    // Initialize LCD_C
    // ACLK, Divider = 1, Pre-divider = 16; 4-pin MUX
    LCDCCTL0 = LCDDIV__1 | LCDPRE__16 | LCD4MUX | LCDLP;

    // VLCD generated internally,
    // V2-V4 generated internally, v5 to ground
    // Set VLCD voltage to 2.60v
    // Enable charge pump and select internal reference for it
    LCDCVCTL = VLCD_1 | VLCDREF_0 | LCDCPEN;

    LCDCCPCTL = LCDCPCLKSYNC;               // Clock synchronization enabled

    LCDCMEMCTL = LCDCLRM;                   // Clear LCD memory
    //Turn LCD on
    LCDCCTL0 |= LCDON;
}

