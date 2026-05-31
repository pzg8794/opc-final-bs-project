/* xbee_at.c support file */

#ifndef _xbee_api_h_
#define _xbee_api_h_

/* API fields	*/

#define FRM_DLM	0x7e		//frame delimiter

/* frame types	*/
#define XMIT_REQ	0x10	//transmit request	

/* typedefs	*/

/* Function declarations */ 

BOOL Xbee_init( void );
void Xbee_xmit_request( BYTE *addr64, WORD network, BYTE nbytes, BYTE *data, BYTE frameid, BYTE bradius, BYTE options );
//BOOL Xbee_cmdmode_enter( void );
//BOOL Xbee_cmdmode_exit( void );
//BOOL Xbee_waitfor_OK( void );

#endif // _xbee_at_h_