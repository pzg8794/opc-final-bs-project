/* pic18f26k20-based light energy harvest module */

#define _xbee_sensor_c_


#pragma config FOSC = INTIO7, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDTEN = OFF
#pragma config MCLRE = ON, HFOFST = ON, PBADEN = OFF, STVREN = ON, LVP = OFF, XINST = OFF, DEBUG = ON

#include "project_config.h"

/* Global vars	*/
volatile DWORD uptime = 0;
BYTE buf[ 64 ] = { 0 };

volatile BYTE USART_Tx_buf [ USART_RX_BUFSIZE ] = { 0 };
volatile BYTE USART_Tx_head = 0;
volatile BYTE USART_Tx_tail = 0;
volatile BYTE USART_Rx_buf [ USART_RX_BUFSIZE ] = { 0 };
volatile BYTE USART_Rx_head = 0;
volatile BYTE USART_Rx_tail = 0;

/* Prototypes	*/
void highPriorityISR(void);
void lowPriorityISR(void);
void Board_init( void );
void APItest( void );

/* ISRs	*/
#pragma code high_vector=0x08
void interruptAtHighVector(void)
{
	_asm GOTO highPriorityISR _endasm
}
#pragma code
#pragma code low_vector=0x18
void interruptAtLowVector(void)
{
	_asm GOTO lowPriorityISR _endasm
}
#pragma code
#pragma interruptlow lowPriorityISR
void lowPriorityISR(void)
{
 BYTE data,tmphead,tmptail;           //USART vars
/* USART handler start */
    if(PIR1bits.RCIF) {
        data = RCREG;                                                   // read the received data 
        tmphead = ( USART_Rx_head + 1 ) & USART_RX_BUF_MASK;            // calculate buffer index 
        USART_Rx_head = tmphead;                                        // store new index 
        if ( tmphead == USART_Rx_tail ) {
        // ERROR! Receive buffer overflow 
        }
        USART_Rx_buf[ tmphead ] = data;                                 // store received data in buffer 
  }
    //? need to check for Tx IF
    if( TXSTAbits.TRMT /* && ( CTS == 0 ) */) { 
                                                                        // check if all data is transmitted  
        if ( USART_Tx_head != USART_Tx_tail ) {                                     
            tmptail = ( USART_Tx_tail + 1 ) & USART_TX_BUF_MASK;        // calculate buffer index  
            USART_Tx_tail = tmptail;                                    // store new index  
            TXREG = USART_Tx_buf[ tmptail ];                            // start transmition  
        }
        else {
            PIE1bits.TXIE = 0;         // disable TX interrupt  
        }
    } 
/* USART handler end */
}

/* Timer1 interrupt handler */
#pragma interrupt highPriorityISR
void highPriorityISR(void)
{
	//tgl_BPNT_0;
	uptime++;
	PIR2bits.CCP2IF = 0;
}



void main ( void )
{
	Board_init();
	USART_init();
	if( Xbee_init() == FALSE ) {
		while( 1 );
	}
	//APItest();
//	CLI_init();
	while(1) {						//main loop
//		CLI_Task();
	APItest();
	}
}

void Board_init( void )
{
// DWORD init_delay;
	/* Internal clock is 1MHz by default	*/
	INTCONbits.GIEH = 0;	//disable interrupts
	RCONbits.IPEN = 1;		//enable interrupt priority
	/* Configure all port pins as digital outputs	*/
	TRISA 	= 0x00;
	LATA  	= 0x00;
	ADCON1	= 0x0f;
	PORTA = 0x00;

	TRISB = 0x00;
	LATB = 0x00;
	PORTB = 0x00;

	TRISC = 0x00;
	LATC = 0x00;
	PORTC = 0x00;
	/* Configure inputs	*/
	CTS_TRIS = 1;
	/* Initial pin states if not 0	*/
//	SL_RQ = 0;			//Sleep off			
//	XBEE_RST = 0;		//Hold Xbee in reset
//	BPNT_0 = 0;
//	BPNT_1 = 0;			
	/* Clock configuration	*/
	/* Internal oscillator 1MHz	*/
//	OSCCON |= 0x70;			//IRCF = 111
//	OSCTUNE |= 0x40;		//PLL
	/* Timer3 configuration */
	TMR3L = 0;
	TMR3H = 0;
	T3CONbits.T3CCP2 = 1;	//CCP2 compare source
	T3CON |= 0x00;			//1:1 prescale
	T3CONbits.TMR3CS = 0;	//internal clock
	T3CONbits.TMR3ON = 1;	//start timer
	/* CCP2 configuration. CCP2 reloads Timer3 and generates interrupt	*/
	CCPR2H = 0x00;			// 250 cycles, 4us cycle, 1ms between interrupts
	CCPR2L = 0xfa;
	CCP2CON |= 0x0b;		//CCP mode 1011, compare triggers special event
	PIE2bits.CCP2IE = 1;	//enable CCP2 interrupt
	IPR2bits.CCP2IP = 1;	//set high priority
    /* A/D configuration    */
    ADCON2bits.ADFM = 1;    //output format right-justified
    ADCON2 |= 0x3e;         // conversion times ACQT 111, ADSC 110
	/* Interrupts	*/		
	INTCONbits.GIEL = 1;		//enable low-priority interrupts
	INTCONbits.GIEH = 1;		//enable all interrupts
}

void APItest( void )
{
 char tmpstr[] = "Hello\r";
 BYTE coord[ 8 ] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
 Xbee_xmit_request( coord, 0xfffe, 6, tmpstr, 1, 0, 0 );
}