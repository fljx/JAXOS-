#ifndef _BUZZER_H
	#define _BUZZER_H

#include	"kernel/thread.h"
#include	"drivers/devices.h"
#include	<avr/io.h>

#define	BUZZER_BIT		PG2
#define	BUZZER_PORT		PORTG


/**	Buzzer is a one-shot style thread.
 Once "beeped", it will run only once to shut off the buzzer after a specified "time".	*/
class	Buzzer	:	public	OneShotThread, public OnOffDev
{
public:
	Buzzer( )
	:	OneShotThread( 1 )
	{}

	void	beep( uint16_t time );

	void on( )
	{	BUZZER_PORT |= _BV( BUZZER_BIT );	}

	void off( )
	{	BUZZER_PORT &= ~_BV( BUZZER_BIT );	}
};


extern	Buzzer	buzzer;

#endif // _BUZZER_H
