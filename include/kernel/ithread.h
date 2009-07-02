#ifndef _ITHREAD_H
	#define _ITHREAD_H


#include	<inttypes.h>

//-----------------------------------------------------------------------------
/// Thread interface class.
class IThread
{
public:
	virtual void	exec( ) = 0;	///<	Executes the thread code.
	virtual void	run( ) = 0;		///<	Puts the thread in "Running" state.
	virtual void	stop( ) = 0;	///<	Puts the thread in "Stopped" state.
	virtual void	pause( ) = 0;	///<	Puts the thread in "Paused" state.

	virtual void context_load( ) = 0;
	virtual void context_save( ) = 0;

	enum	State
	{
		Stopped,		///<	Thread is not running anyway.
		Running,		///<	Thread is running or ready to.
		Waiting,		///<	Thread is waiting some event to run.
		Sleeping,
		Paused			///<	Thread was explicitly paused and will only run again by an explicit "run()".
	};

	virtual State state( ) = 0;
//	static inline void	yield( ) = 0; //__attribute__(( always_inline ));

protected:

	template< uint8_t MaxTasks > friend class Scheduler;

	virtual bool	ring( ) = 0;	///<	Placeholder to a "waking" signal.
};


#endif // _ITHREAD_H
