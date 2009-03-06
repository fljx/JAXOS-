#include	"drivers/buzzer.h"


Buzzer	buzzer;

//-----------------------------------------------------------------------------

void	Buzzer::beep( uint16_t time )
{
	_period = time;		//	Buzz time counter in milliseconds.
	_per_count = 0;
	on( );
	Thread::run( );
}

