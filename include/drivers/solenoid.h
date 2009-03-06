#ifndef __SOLENOID_H
	#define __SOLENOID_H


#include	"kernel/thread.h"
#include	"drivers/devices.h"


template< uint8_t Bit, Port_BaseAddr Port = Port_BaseAddr::Base_PORTA >
class	Solenoid	:	public	OneShotThread, public OnOffDev
{
public:
	Solenoid( int16_t period )
	:	OneShotThread( period )
	{}

	void	on( );
	void	off( );

protected:
	void	run( )	{	OneShotThread::run( );		}	///< Hides "run()". Use "on()" instead.
	void	stop( )	{	OneShotThread::stop( );	}		///< Hides "stop()". Use "off()" instead.

};

#endif // __SOLENOID_H
