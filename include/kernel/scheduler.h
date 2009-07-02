#ifndef __SCHEDULER_H
	#define __SCHEDULER_H


#include	"types.h"
#include	<lib/list.h>


#include	<kernel/ithread.h>


template< uint8_t MaxTasks = 32 >
class Scheduler
{
public:
	Scheduler( );

	static void	schedule( )					__attribute__(( naked ));
	static bool	add( IThread &thread );
	static bool	remove( IThread &thread );

	static int	count( )		{	return	threads.count( );	}

	static void	tick( );		///<	Notify sleeping threads of a system timer tick.

protected:
	typedef CircularList< IThread *, MaxTasks >		ThreadList;

	static ThreadList	threads;
	static typename  ThreadList::iterator	curr_thread;

	static inline void	critical_enter( )	__attribute__(( always_inline ));
	static inline void	critical_exit( )	__attribute__(( always_inline ));
};


	#include	<../kernel/scheduler.cpp>


#endif // __SCHEDULER_H
