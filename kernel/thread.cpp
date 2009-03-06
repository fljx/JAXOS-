#include	"kernel/thread.h"
#include	"kernel/scheduler.h"


Thread::Thread( )
:	state( Stopped )
{
	Scheduler<>::add( *this );
}


Thread::~Thread( )
{
	Scheduler<>::remove( *this );
}



///	Suspend the Thread execution for "_period" ms.
void	TimedThread::sleep( int16_t period )
{
	_per_count = period;
	state = Sleeping;
}


/**	Tells if it is time to run the thread.
	Counts base-time clocks (e.g. 1ms) till the thread time.
*/
bool	TimedThread::ring( )
{
	return ( ( state == Sleeping || state == Running ) && ( --_per_count == 0 ) );
}


///	Sleep the Thread until the next "period" elapse.
void	PeriodicThread::exec( )
{
	sleep( _period );
}


///	Stop the Thread after executing once.
void	OneShotThread::exec( )
{
	stop( );
}
