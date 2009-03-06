#ifndef __SCHEDULER_H
	#define __SCHEDULER_H


#include	"types.h"
#include	<lib/list.h>


class	Thread;


template< uint8_t MaxTasks = 32 >
class Scheduler
{
public:
	Scheduler( );

	static void	schedule( );
	static bool	add( Thread &thread );
	static bool	remove( Thread &thread );

	static int	count( )		{	return	threads.count( );	}

	static void	tick( );		///<	Notify sleeping threads of a system timer tick.

protected:
	typedef CircularList< Thread *, MaxTasks >		ThreadList;

	static ThreadList	threads;
	static typename  ThreadList::iterator	curr_thread;

	static inline void	context_load( )	__attribute__(( always_inline ));
	static inline void	context_save( )	__attribute__(( always_inline ));
	static void			context_switch( );

	static inline void	yield( ) __attribute__(( naked ));

	static inline void	critical_enter( )	__attribute__(( always_inline ));
	static inline void	critical_exit( )	__attribute__(( always_inline ));
};


	#include	<../kernel/scheduler.cpp>


#endif // __SCHEDULER_H
