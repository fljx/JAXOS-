#ifndef _THREAD_H
	#define _THREAD_H


#include	"types.h"


class	Thread
{
public:
	Thread( );

	~Thread( );

	virtual void	exec( ) = 0;							///<	Executes the thread code.
	virtual void	run( )		{	state = Running;	}	///<	Puts the thread in "Running" state.
	virtual void	stop( )		{	state = Stopped;	}	///<	Puts the thread in "Stopped" state.
	virtual void	pause( )	{	state = Paused;	}		///<	Puts the thread in "Paused" state.

protected:
	enum	State
	{
		Stopped,		///<	Thread is not running anyway.
		Running,		///<	Thread is running or ready to.
		Waiting,		///<	Thread is waiting some event to run.
		Sleeping,
		Paused			///<	Thread was explicitly paused and will only run again by an explicit "run()".
	};

	State	state;

	template< uint8_t MaxTasks > friend class Scheduler;

	virtual bool	ring( ) = 0;	///<	Placeholder to a "waking" signal.
};


/// Abstract class for threads that depend on a time-base to run.
class	TimedThread	:	public	Thread
{
public:
	TimedThread( int16_t period )
	:	Thread( ), _period( period )
	{}

	virtual void	sleep( int16_t period );				///<	Suspend the Thread execution for "period" ms.

protected:
	int16_t _period;
	int16_t _per_count;

	bool	ring( );	///<	Tells if it is time to run the thread.
};


/// TimedThread that runs continuously.
class	PeriodicThread	:	public	TimedThread
{
public:
	PeriodicThread( int16_t period )
	:	TimedThread( period )
	{}

	void	exec( );
};


/// TimedThread that runs once on its time-base then goes "Stopped".
class	OneShotThread	:	public	TimedThread
{
public:
	OneShotThread( int16_t period )
	:	TimedThread( period )
	{}

	void	exec( );
};


#endif // _THREAD_H
