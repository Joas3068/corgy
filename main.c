
#include <msp430G2553.h>
#define FULL 10000
#define HIGH 8000

void uartInit();
void pinInit();

int main(void)

{

  WDTCTL = WDTPW + WDTHOLD;                 // Stop WD
  if (CALBC1_1MHZ==0xFF)                    // If calibratiFULL cFULLstant erased
  {
    while(1);                               // do not load, trap CPU!!
  }




  uartInit();
  pinInit();


while(1)
{

    switch(UCA0TXBUF)
    {
         case '1': //Forward

             P2DIR |= BIT1; //Toggle PWM Signals
             P2DIR |= BIT2;
             P2DIR |= BIT4;
             P2DIR |= BIT5;

             TA1CCR0 = FULL; //period

             TA1CCR1 = HIGH;   //duty
             TA1CCR2 = HIGH;

             P1OUT |=BIT0;
             P1OUT &= ~BIT6; //direction control

             break;

         case '2': //Right

             P2DIR &= ~BIT1; //PWM Toggle
             P2DIR |= BIT2;
             P2DIR &= ~BIT4;
             P2DIR |= BIT5;

             TA1CCR0 = FULL; //period

             TA1CCR1 = HIGH;   //duty
             TA1CCR2 = HIGH;

             P1OUT |= BIT0;
             P1OUT &= ~BIT6;

              break;
         case '3': //Left

             P2DIR |= BIT1;
             P2DIR &= ~BIT2;
             P2DIR |= BIT4;
             P2DIR &= ~BIT5;

             TA1CCR0 = FULL; //period

             TA1CCR1 = HIGH;   //duty
             TA1CCR2 = HIGH;

             P1OUT |= BIT0;
             P1OUT &= ~BIT6;


              break;
         case '4':  //BACKWARD

             break;

         default:  //default turns all pins OFF which means setting them high

             P2DIR &= ~BIT1;
             P2DIR &= ~BIT2;
             P2DIR &= ~BIT4;
             P2DIR &= ~BIT5;

//             TA1CCR0 = 0;
//             TA1CCR1 = 0;
//             TA1CCR2 = 0;

             P1OUT |= BIT6;  //Both Directions On Stops Motors
             P1OUT |= BIT0;
             break;
}
  __bis_SR_register(GIE);      

}

}



#pragma vector=USCIAB0RX_VECTOR

__interrupt void USCI0RX_ISR(void)

{

	while (!(IFG2&UCA0TXIFG));              
  	{	
		UCA0TXBUF = UCA0RXBUF;                    
  	}
}

void pinInit()
{
    P1DIR |= BIT0;
    P1OUT |= BIT0;
    P1DIR |= BIT6;


    P2DIR &= ~BIT1;  //PWM 1 DirectiFULL
    P2SEL |= BIT1; //PWM SELECT

    P2DIR &= ~BIT4;
    P2SEL |= BIT4;

    P2DIR &= ~BIT2;
    P2SEL |= BIT2;

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

    TA1CTL = (TASSEL_2 | MC_1);  //PWM selectiFULL clock
}

void uartInit()
{

    DCOCTL = 0;                               // Select lowest DCOx and MODx settings

    BCSCTL1 = CALBC1_1MHZ;                    // Set DCO

    DCOCTL = CALDCO_1MHZ;

    P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD

    P1SEL2 = BIT1 + BIT2 ;                    // P1.1 = RXD, P1.2=TXD

    UCA0CTL1 |= UCSSEL_2;                     // SMCLK

    UCA0BR0 = 104;                            // 1MHz 9600

    UCA0BR1 = 0;                              // 1MHz 9600

    UCA0MCTL = UCBRS0;                        // ModulatiFULL UCBRSx = 1

    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**

    IE2 |= UCA0RXIE;


}

