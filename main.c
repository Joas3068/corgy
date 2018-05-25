
#include <msp430G2553.h>
#define ON 10000



int main(void)

{

        WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  // int i = 0;
  // char buff[2] = {'L','e','\0'};

  if (CALBC1_1MHZ==0xFF)                    // If calibration constant erased

  {

    while(1);                               // do not load, trap CPU!!

  }

  DCOCTL = 0;                               // Select lowest DCOx and MODx settings

  BCSCTL1 = CALBC1_1MHZ;                    // Set DCO

  DCOCTL = CALDCO_1MHZ;

  P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD

  P1SEL2 = BIT1 + BIT2 ;                    // P1.1 = RXD, P1.2=TXD

  UCA0CTL1 |= UCSSEL_2;                     // SMCLK

  UCA0BR0 = 104;                            // 1MHz 9600

  UCA0BR1 = 0;                              // 1MHz 9600

  UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1

  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**

  IE2 |= UCA0RXIE;

// Enable USCI_A0 RX interrupt

P1DIR |= 0x01;

//P2DIR |= BIT2;  //GPIO
//P2DIR |= BIT5;  //GPIO


P2DIR &= ~BIT1;  //PWM 1 Direction
P2SEL &= ~BIT1; //PWM SELECT

P2DIR &= ~BIT4;
P2SEL &= ~BIT4;

P2DIR &= ~BIT2;
P2SEL &= ~BIT2;

P2DIR &= ~BIT5;
P2SEL |= BIT5;

TA1CCTL1 = OUTMOD_7;    //2.1
TA1CCTL2 = OUTMOD_7;    //2.4

P2SEL2 &= ~BIT1; //2.1
P2SEL2 &= ~BIT4;
P2SEL2 &= ~BIT2;
P2SEL2 &= ~BIT5;

TA1CCR0 = 0;         //period

TA1CCR1 = 5000;  //duty
TA1CCR2 = 0;

TA1CTL = (TASSEL_2 | MC_1);  //PWM selection clock



P1DIR |= BIT0;
P1OUT |=BIT0;

while(1)
{

    switch(UCA0TXBUF)
    {
         case '1': //RIGHT


             //Half DOOTY cycle

             P2SEL |= BIT1; //PWM on
             P2DIR |= BIT1;

             P2SEL &= ~BIT2;//PWM OFF
             P2DIR &= ~BIT2;


             P2SEL &= ~BIT4; //OFF and NOT HIGH
             P2DIR &= ~BIT4;

//             P2SEL |= BIT5; //OFF and NOT HIGH
//             P2DIR |= BIT5;//ON RECENT CODE

             P2SEL |= BIT5; //OFF and NOT HIGH
             P2DIR &= ~BIT5;//OFFF OLDCODE

             TA1CCR0 = ON; //period

             TA1CCR1 = 2000;   //duty
             TA1CCR2 = 2000;
             P1OUT |=BIT0;
             break;

         case '2': //Left

              P2SEL &= ~BIT1;
              P2DIR &= ~BIT1;

              P2DIR |= BIT2;
              P2SEL |= BIT2;
//
//              P2SEL |= BIT4; //OFF and NOT HIGH
//              P2DIR |= BIT4;

              P2SEL &= ~BIT4; //OFF and NOT HIGH
              P2DIR &= ~BIT4;

              P2SEL |= BIT5; //OFF and NOT HIGH
              P2DIR &= ~BIT5;

              TA1CCR0 = ON; //period

              TA1CCR1 = 2000;   //duty
              TA1CCR2 = 2000;
              P1OUT |=BIT0;
              break;
//         case '3': //FORWARD
//
//
//
//
//
//              TA1CCR0 = 10000; //period
//              TA1CCR1 = 3000;   //duty
//              P1OUT |=BIT0;
//              //TA1CCR2 = 10000;
//              break;
//         case '4':  //BACKWARD
//
//
//             TA1CCR0 = ON;
//             TA1CCR1 = 0;
//             TA1CCR2 = 3000;
//             P1OUT |=BIT0;
//             break;

         default:  //default turns all pins OFF which means setting them high

             P2SEL &= ~BIT1;
             P2DIR &= ~BIT1;

             P2SEL &= ~BIT2;
             P2DIR &= ~BIT2;

             P2SEL &= ~BIT4; //OFF and NOT HIGH
             P2DIR &= ~BIT4;

             P2SEL |= BIT5; //OFF and NOT HIGH
             P2DIR &= ~BIT5;

             TA1CCR0 = 0;
             TA1CCR1 = 0;
             TA1CCR2 = 0;
             break;
}
  __bis_SR_register(GIE);       // Enter LPM0, interrupts enabled

}

}


//  Echo back RXed character, confirm TX buffer is ready first

#pragma vector=USCIAB0RX_VECTOR

__interrupt void USCI0RX_ISR(void)

{

  while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
  {
  UCA0TXBUF = UCA0RXBUF;                    // TX -> RXed character

  }
}
