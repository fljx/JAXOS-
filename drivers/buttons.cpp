#ifndef _BUTTONS_CPP
	#define _BUTTONS_CPP

#include	"drivers/buttons.h"

#include	<avr/interrupt.h>
#include	<avr/io.h>
#include	"iomxx0_1.hpp"

#include	<drivers/uart.h>	//;;;;

//-------------------------------------------------------------
//------------- Button ----------------------------------------
//-------------------------------------------------------------

template< uint8_t ButtonBit >
Button< ButtonBit >::Button( )
{
	Bit::interruptOn( );
	Bit::input( );	// Pin as input.
}

//-------------------------------------------------------------
//------------- NotifyingButton -------------------------------
//-------------------------------------------------------------

template< uint8_t ButtonBit, uint8_t MessageHeader, uint16_t DebounceTime >
NotifyingButton< ButtonBit, MessageHeader, DebounceTime >::NotifyingButton( INotifier< data16 > &notifier )
	:	Button< ButtonBit >::Button( ),
		OneShotThread( DebounceTime ),
		notifier( notifier )
{
}

/**	Generate Panic Button pressed or released notification.

	When the button is pressed or released, it will be a 10 ms delay
	before reading its actual state for debouncing.
	Any further interrupt will be ignored.
*/
template< uint8_t ButtonBit, uint8_t MessageHeader, uint16_t DebounceTime >
void NotifyingButton< ButtonBit, MessageHeader, DebounceTime >::exec( )
{
	bool curr_state = this->status( );

	if ( button_state != curr_state )
	{
		button_state = curr_state;
//uart1.put( button_state ? ':' : '.' );	//;;;;
		notifier.notify( data16( MessageHeader, ( button_state ? 0x80 : 0x00 ) | ButtonBit ) );
	}
	OneShotThread::exec( );				// Thread will stop after this.
}

/**	Start Button press or release monitoring.

	Actual button reading will be done later in Thread::exec( ) for debouncing.
*/
template< uint8_t ButtonBit, uint8_t MessageHeader, uint16_t DebounceTime >
void	NotifyingButton< ButtonBit, MessageHeader, DebounceTime >::change( )
{
	if ( Thread::state == Stopped )			// If not Stopped, already debouncing.
		this->sleep( _period );			// Reset timer and prepare to run the Thread.
}


//-------------------------------------------------------------
//------------- Bimanual --------------------------------------
//-------------------------------------------------------------

template< uint8_t ButtonBit0, uint8_t ButtonBit1, uint8_t MessageHeader >
Bimanual< ButtonBit0, ButtonBit1, MessageHeader >::Bimanual( INotifier< data16 > &notifier )
	:	OneShotThread( 500 ),
		notifier( notifier ),
		button0( notifier ), button1( notifier )
{
}

/**	Checks for both buttons pressing.

	When one of the bimanual buttons is pressed, it will be a timeout of 0,5 s
	for pressing the second button or the activation event will not be fired.

	The time elapsed from pin change interrupt till the exec() call serves as debouncing.
*/
template< uint8_t ButtonBit0, uint8_t ButtonBit1, uint8_t MessageHeader >
void Bimanual< ButtonBit0, ButtonBit1, MessageHeader >::exec( )
{
	stop( );	// Cancel activation.
}


template< uint8_t ButtonBit0, uint8_t ButtonBit1, uint8_t MessageHeader >
void	Bimanual< ButtonBit0, ButtonBit1, MessageHeader >::change( uint8_t changed )
{
	if ( changed & _BV( ButtonBit0 ) )
		button0.change( );

	if ( changed & _BV( ButtonBit1 ) )
		button1.change( );
}


///	Receives button state change notifications.
template< uint8_t ButtonBit0, uint8_t ButtonBit1, uint8_t MessageHeader >
void	Bimanual< ButtonBit0, ButtonBit1, MessageHeader >::notify( const data16 &/*msg*/ )
{
//	if ( ( msg.low & 0x7f ) == ButtonBit1 )				// Clear previous button state.
//		button_state &= ~_BV( ButtonBit1 );
//	else
//		button_state &= ~_BV( ButtonBit0 );
//
//	button_state |= _BV( msg.high & 0x07 );
////;;;;
//	if ( Thread::state == Stopped )		// Don't reset thread timer if it is already running.
//	{
//		button_state = _BV( msg.high & 0x07 );	// Record 1st button pressed.
//		_per_count = _period;
//		this->state = Sleeping;
//	}
//	else
//	{
//		button_state |= _BV( msg.high & 0x07 );
//		if ( button_state == button_mask )
//			notifier.notify( data16( MessageHeader, 3 ) );
//	}
//
////;;;;
//	const uint8_t	button_mask = _BV( ButtonBit0 ) | _BV( ButtonBit1 );
//// FIXME:
//	if ( ring( ) )	// Timeout waiting for both buttons.
//	else if ( ( button_state & button_mask ) == button_mask )	// Both buttons pressed?
//	{
//		notifier.notify( data16( MessageHeader, 3 ) );
//		button_state = 0;
//	}
}


#endif // _BUTTONS_CPP
