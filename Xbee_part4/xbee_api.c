/* Xbee AT conmmand support functions */

#define _xbee_api_c_

#include "project_config.h"
#include <string.h>

///* strings	*/
////const rom char *const rom resp_err = "ERROR\r";
///* AT commands	*/
//const rom char *const rom three_pluses = "+++";
//const rom char *const rom at_cmdmode_exit = "ATCN\r";
///* External variables	*/
extern DWORD uptime;


/* Xbee initialization */
/* De-asserts reset and waits for modem status "reset" from Xbee	*/
BOOL Xbee_init( void )
{
 BYTE modem_reset[ 5 ] = { 0x00, 0x02, 0x8a, 0x00, 0x75 };	//modem status "Reset" frame including checksum
 BYTE buf[ 5 ] = { 0 };
 BYTE i = 0;
 BYTE tmp;
 DWORD delay = uptime + 2000;		//2 seconds from current uptime				
	XBEE_RST = 1;	//release reset
	while( uptime < delay ) {
		if( CharInQueue()) {
    		tmp = recvchar();
    		if( tmp == 0x7e ) {			//waiting for frame delimiter
    			break;
    		}
    	}
    }
	if( tmp != 0x7e ) {				//fail if no frame delimiter was received in 2 sec. Typical reset time is 300ms
		return( FALSE );
	}
	while(( i < 5 ) && ( uptime < delay )) {
		if( CharInQueue()) {
			buf[ i ] = recvchar();
			i++;
		}
	}
	if( memcmp( (const far char *)modem_reset, (const far char *)buf, 5 ) == 0 ) {	//if received packet is modem status "Reset" 
    	return( TRUE );
    }
    else {
    	return( FALSE );
    }
}

/* sends transmit request to addr, network				*/
/* 256 bytes max.payload; Xbee max.payload is 100 bytes	*/
void Xbee_xmit_request( BYTE *addr64, WORD network, BYTE nbytes, BYTE *data, BYTE frameid, BYTE bradius, BYTE options )
{
 //WORD_VAL tmpw = 0;
 WORD tmpword = 0;
 BYTE checksum = XMIT_REQ;				//frame type is added since it
 BYTE tmpbyte;
 char i;
 char *byteptr = NULL;
	Nop();
	set_BPNT_0;
 	sendchar( FRM_DLM );			//send frame delimiter
 	tmpword = nbytes + 14;			//payload plus frame type, frame ID, addr, network, broadcast radius, options
 	byteptr = ( char *)&tmpword;	//little-endian assumed
	for( i = 1; i > -1; i-- ) {
		sendchar( *( byteptr + i ));
	}
	sendchar( XMIT_REQ );				//send frame type
	/* starting calculating checksum from here	*/
	checksum += sendchar( frameid );	//send frame ID

	for( i = 7; i > -1; i-- ) {			//send address
		tmpbyte = *( addr64 + i);
		checksum += sendchar( *( addr64 + i ));
	}
	byteptr = ( char *)&network;
	for( i = 1; i > -1; i-- ) {			//send network
		checksum += sendchar( *( byteptr + i ));
	}
	checksum += sendchar( bradius );	//send broadcast radius
	checksum += sendchar( options );	//send options
	for( i = 0; i < nbytes; i++) {		//send payload
		tmpbyte = *data;
		checksum += sendchar( *data );
		data++;
	}
	checksum = 	0xff - checksum;
	sendchar( checksum );
	clr_BPNT_0;
	Nop();
}
		
	
	
	
	
 																																					
	
///* switching Xbee to command mode   */
///* returns TRUE if OK is received   */
///* assumes DWORD uptime incrementing each millisecond   */
//BOOL Xbee_cmdmode_enter( void )
//{
// DWORD delay;
//    delay = uptime + 2000;
//    while( uptime < delay );    //wait 1 sec guard time
//    send_string(three_pluses);
//    if( Xbee_waitfor_OK() ) {
//        return( TRUE );
//    }
//    return( FALSE );
//}
///* command mode assumed         */
///* returns TRUE if OK received  */
//BOOL Xbee_cmdmode_exit( void )
//{
// //DWORD delay; 
//    send_string( at_cmdmode_exit );
//    if( Xbee_waitfor_OK() ) {
//        return( TRUE );
//    }
//    //delay = uptime + 500;
//    //while( uptime < delay );  //wait half sec
//    return( FALSE );
//}
///* reads serial stream expecting OK                 */
///* returns TRUE if received                         */
///* assumes 'uptime' incrementing each millisecond   */
//BOOL Xbee_waitfor_OK( void )
//{
// BOOL rcode = FALSE;
// char buf[ 4 ] = { 0 };
// BYTE i = 0;
// DWORD timeout;
// rom char *resp_ok = "OK\r";
//    timeout = uptime + 2000;    //timeout 2 sec
//    while( uptime < timeout ) {
//    if( CharInQueue()) {
//            buf[ i ] = recvchar();
//            i++;
//        }
//        if( strcmppgm2ram( buf, (const far rom char *)"OK\r" ) == 0 ) {
//            rcode = TRUE;
//            break;
//        }
//    }
//    return( rcode );    
//}
