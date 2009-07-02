#ifndef _THREAD_H
	#define _THREAD_H


//#include	"types.h"
#include	<kernel/ithread.h>
#include	<kernel/context.h>



//-----------------------------------------------------------------------------
template< typename ContextType = FullContext >
class Thread	:	IThread
{
public:
	Thread( );
	virtual ~Thread( );

	virtual void	run( )		{	_state = Running;	}	///<	Puts the thread in "Running" state.
	virtual void	stop( )		{	_state = Stopped;	}	///<	Puts the thread in "Stopped" state.
	virtual void	pause( )	{	_state = Paused;		}	///<	Puts the thread in "Paused" state.

	inline void context_load( )
	{		ContextType::load( );	}

	inline void context_save( )
	{		ContextType::save( );	}

	State state( )
	{	return	_state;	}

	static inline void	yield( )	__attribute__(( always_inline ));

protected:

	State	_state;
};


//-----------------------------------------------------------------------------
template< typename ContextType = FullContext >
class TimedThread	:	public Thread< ContextType >
{
public:
	TimedThread( int16_t period )
	:	Thread< ContextType >( ), _period( period )
	{	}

	virtual ~TimedThread( )
	{	}

	void	exec( )
	{	}

	virtual void	sleep( int16_t period );				///<	Suspend the Thread execution for "period" ms.

protected:
	int16_t _period;
	int16_t _per_count;

	bool	ring( );	///<	Tells if it is time to run the thread.
};


template< class ContextType >
void	TimedThread< ContextType >::sleep( int16_t period )
{
	_per_count = period;
	this->_state = IThread::Sleeping;
}

template< class ContextType >
bool	TimedThread< ContextType >::ring( )
{
	return ( ( this->_state == IThread::Sleeping ||
		this->_state == IThread::Running ) && ( --_per_count == 0 ) );
}

//-----------------------------------------------------------------------------
/// TimedThread that runs continuously.
template< typename ContextType = FullContext >
class PeriodicThread	:	public TimedThread< ContextType >
{
public:
	PeriodicThread( int16_t period )
	:	TimedThread< ContextType >( period )
	{	}

	virtual ~PeriodicThread( )
	{	}

	void	exec( )
	{	sleep( this->_period );	}
};

//-----------------------------------------------------------------------------
/// TimedThread that runs once on its time-base then goes "Stopped".
template< typename ContextType = FullContext >
class OneShotThread	:	public TimedThread< ContextType >
{
public:
	OneShotThread( int16_t period )
	:	TimedThread< ContextType >( period )
	{	}

	virtual ~OneShotThread( )
	{	}

	void	exec( )
	{	}
};

//-----------------------------------------------------------------------------

	#include	<../kernel/thread.cpp>


#endif // _THREAD_H
