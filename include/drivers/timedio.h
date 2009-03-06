#ifndef _TIMEDIO_H
	#define _TIMEDIO_H


#include	"kernel/thread.h"
#include	"drivers/ports.h"
#include	"drivers/devices.h"


template< typename ThreadType, uint8_t Bit, Port_BaseAddr Port >
class	TimedIO	:	public	ThreadType, public OnOffDev
{
public:
	TimedIO( int16_t period )
	:	ThreadType( period )
	{}

	void	on( );
	void	off( );

protected:
	void	run( )	{	this->run( );	}	///< Hides "run()". Use "on()" instead.
	void	stop( )	{	this->stop( );	}	///< Hides "stop()". Use "off()" instead.

};


template< uint8_t Bit, Port_BaseAddr Port >
class	ToggleIO	:	public	TimedIO< PeriodicThread, Bit, Port >
{
public:
	ToggleIO( int16_t period )
	:	TimedIO< PeriodicThread, Bit, Port >( period )
	{}

	void	exec( );
	void	toggle( );
};


template< uint8_t Bit, Port_BaseAddr Port >
class	OneShotIO	:	public	TimedIO< OneShotThread, Bit, Port >
{
public:
	OneShotIO( int16_t period )
	:	TimedIO< OneShotThread, Bit, Port >( period )
	{}

	void	exec( )		{	this->off( );	}
};


#ifndef _TIMEDIO_CPP
	#include	"../drivers/timedio.cpp"
#endif	// _TIMEDIO_CPP


#endif	//	_TIMEDIO_H
