/* Xbee sensor PIC18F26K20 project configuration file	*/

#ifndef _project_config_h_
#define _project_config_h_

/* define MCU family and configuration fuses if any */
#include <p18cxxx.h>

#include "GenericMacros.h"
#include "GenericTypeDefs.h"
#include "USARTio.h"
#include "xbee_api.h"

/*	PIC pin functional assignments	*/

#define CTS         PORTAbits.RA7
#define CTS_TRIS    TRISAbits.TRISA7

#define BPNT_0		PORTBbits.RB4
#define BPNT_1		PORTBbits.RB5

#define SL_RQ		PORTCbits.RC0		//Xbee Sleep RQ pin. 1 sleeps
#define XBEE_RST	PORTCbits.RC2		//Xbee reset pin. 200ns 0 resets

#define set_BPNT_0 	BPNT_0 = 1;
#define	clr_BPNT_0	BPNT_0 = 0;
#define tgl_BPNT_0	set_BPNT_0; clr_BPNT_0
#define set_BPNT_1 	BPNT_1 = 1;
#define	clr_BPNT_1	BPNT_1 = 0;
#define tgl_BPNT_1	set_BPNT_1; clr_BPNT_1


/* Async serial settings					*/
/* USART transmit and receive buffer sizes	*/
/* must be a power of 2						*/
#define USART_TX_BUFSIZE 16
#define USART_TX_BUF_MASK ( USART_TX_BUFSIZE - 1 )
#if (USART_TX_BUFSIZE & USART_TX_BUF_MASK)
	#error USART Tx buffer size is not a power of 2
#endif

#define USART_RX_BUFSIZE 16
#define USART_RX_BUF_MASK ( USART_RX_BUFSIZE - 1 )
#if (USART_RX_BUFSIZE & USART_RX_BUF_MASK)
	#error USART Rx buffer size is not a power of 2
#endif



#endif // _project_config_h