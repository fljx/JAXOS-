#ifndef _THREAD_CPP
	#define _THREAD_CPP

#include	<kernel/thread.h>
#include	<kernel/scheduler.h>


template< typename ContextType >
Thread< ContextType >::Thread( )
:	_state( Stopped )
{
	Scheduler<>::add( *this );
}

template< typename ContextType >
Thread< ContextType >::~Thread( )
{
	Scheduler<>::remove( *this );
}

template< typename ContextType >
void	Thread< ContextType >::yield( )
{
	Scheduler<>::schedule( );
}

#endif // _THREAD_CPP
