#ifndef _TIMEDIO_CPP
	#define _TIMEDIO_CPP


#include	"drivers/timedio.h"


//------------- TimedIO -------------------------------------------------------

///	Turn on IO. It will turn off after "period" ms.
template< typename ThreadType, uint8_t Bit, Port_BaseAddr Port >
void TimedIO< ThreadType, Bit, Port >::on( )
{
	IOPort< Port >::bitOn( Bit );

	sleep( this->_period );	// Sleep until "period" elapsed.
}

///	Turn off IO and stop the Thread.
template< typename ThreadType, uint8_t Bit, Port_BaseAddr Port >
void TimedIO< ThreadType, Bit, Port >::off( )
{
	IOPort< Port >::bitOff( Bit );

	stop( );
}

//------------- ToggleIO ------------------------------------------------------

///	Toggle IO state on/off.
template< uint8_t Bit, Port_BaseAddr Port >
void	ToggleIO< Bit, Port >::toggle( )
{
	IOPort< Port >::bitToggle( Bit );
}


template< uint8_t Bit, Port_BaseAddr Port >
void	ToggleIO< Bit, Port >::exec( )
{
	toggle( );
	PeriodicThread::exec( );		// Put Thread back to sleep.
}


#endif
