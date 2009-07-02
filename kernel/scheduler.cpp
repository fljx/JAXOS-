#ifndef _SCHEDULER_CPP
	#define _SCHEDULER_CPP


#include	<kernel/scheduler.h>
//#include	<kernel/thread.h>


template< uint8_t MaxTasks >
typename Scheduler< MaxTasks >::ThreadList	Scheduler< MaxTasks >::threads;

template< uint8_t MaxTasks >
typename Scheduler< MaxTasks >::ThreadList::iterator	Scheduler< MaxTasks >::curr_thread;


template< uint8_t MaxTasks >
Scheduler< MaxTasks >::Scheduler( )
{
}

/// Schedule next task to run.
///	\note	There will always be at least one thread, the idle one.
template< uint8_t MaxTasks >
void	Scheduler< MaxTasks >::schedule( )
{
	( *curr_thread )->context_save( );	// Save current Context.

	++curr_thread;					// Next thread.

	( *curr_thread )->context_load( );	// Load Context of this next thread.

	// *curr_thread will never be NULL as we insert tasks by reference.
	if ( ( *curr_thread )->state( ) == IThread::Running )
		( *curr_thread )->exec( );

	asm volatile (
		"sei	\n\t"
		"ret	\n\t":: );	// schedule is "naked", we must return to force the task stack load.
}

template< uint8_t MaxTasks >
bool	Scheduler< MaxTasks >::add( IThread &thread )
{
	return	threads.push_back( &thread );
}

template< uint8_t MaxTasks >
bool	Scheduler< MaxTasks >::remove( IThread &thread )
{
	return	threads.erase( &thread );
}

template< uint8_t MaxTasks >
inline void	Scheduler< MaxTasks >::tick( )
{
	typename ThreadList::iterator	it = threads.begin( );
	typename ThreadList::iterator	begin = it;

	if ( it != threads.end( ) )
		do
		{
			if ( ( *it )->state( ) == IThread::Sleeping && ( *it )->ring( ) )
				( *it )->run( );		// Wake up from Sleeping. Will run in the next schedule.
		}
		while ( ++it != begin );
}

template< uint8_t MaxTasks >
inline void Scheduler< MaxTasks >::critical_enter( )
{
	asm volatile(
		"in	__tmp_reg__, __SREG__       \n\t"
		"cli                            \n\t"
		"push   __tmp_reg__             \n\t"::);
}

template< uint8_t MaxTasks >
inline void Scheduler< MaxTasks >::critical_exit( )
{
	asm volatile(
		"pop	__tmp_reg__                \n\t"
		"out     __SREG__, __tmp_reg__     \n\t":: );
}


#endif		// _SCHEDULER_CPP
