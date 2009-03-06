#ifndef _SCHEDULER_CPP
	#define _SCHEDULER_CPP


#include	<kernel/scheduler.h>
#include	<kernel/thread.h>

#include	<drivers/uart.h>	//;;;;
//#include	<lib/nmea.h>	//;;;;


template< uint8_t MaxTasks >
typename Scheduler< MaxTasks >::ThreadList	Scheduler< MaxTasks >::threads;

template< uint8_t MaxTasks >
typename Scheduler< MaxTasks >::ThreadList::iterator	Scheduler< MaxTasks >::curr_thread;


template< uint8_t MaxTasks >
Scheduler< MaxTasks >::Scheduler( )
{
}

template< uint8_t MaxTasks >
void	Scheduler< MaxTasks >::schedule( )
{
	context_switch( );
}

template< uint8_t MaxTasks >
bool	Scheduler< MaxTasks >::add( Thread &thread )
{
	return	threads.push_back( &thread );
}

template< uint8_t MaxTasks >
bool	Scheduler< MaxTasks >::remove( Thread &thread )
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
			if ( ( *it )->state == Thread::Sleeping && ( *it )->ring( ) )
			//	( *it )->run( );		// Wake up from Sleeping. Will run in the next schedule.
			{
//uart1.put( '-' );	//;;;;
				( *it )->run( );		// Wake up from Sleeping. Will run in the next schedule.
			}
		}
		while ( ++it != begin );
}

template< uint8_t MaxTasks >
inline void	Scheduler< MaxTasks >::context_load( )
{
//CriticalEnter( );
	//	*curr_thread
//CriticalExit( );
    // SP = *g_pCurrentContext
    // pop everything from stack
    // restore SREG according to new context
    asm volatile(
        "lds	r26, g_pCurrentContext	\n\t"
        "lds	r27, g_pCurrentContext+1\n\t"
        "ld	r28, x+			\n\t"
        "out	__SP_L__, r28		\n\t"
        "ld	r29, x+			\n\t"
        "out	__SP_H__, r29		\n\t"
        "pop	r31			\n\t"
        "pop	r30			\n\t"
        "pop	r29			\n\t"
        "pop	r28			\n\t"
        "pop	r27			\n\t"
        "pop	r26			\n\t"
        "pop	r25			\n\t"
        "pop	r24			\n\t"
        "pop	r23			\n\t"
        "pop	r22			\n\t"
        "pop	r21			\n\t"
        "pop	r20			\n\t"
        "pop	r19			\n\t"
        "pop	r18			\n\t"
        "pop	r17			\n\t"
        "pop	r16			\n\t"
        "pop	r15			\n\t"
        "pop	r14			\n\t"
        "pop	r13			\n\t"
        "pop	r12			\n\t"
        "pop	r11			\n\t"
        "pop	r10			\n\t"
        "pop	r9			\n\t"
        "pop	r8			\n\t"
        "pop	r7			\n\t"
        "pop	r6			\n\t"
        "pop	r5			\n\t"
        "pop	r4			\n\t"
        "pop	r3			\n\t"
        "pop	r2			\n\t"
        "pop	r1			\n\t"
        "pop	r0			\n\t"
        "out	__SREG__, r0		\n\t"
        "pop	r0			\n\t":: );
}

template< uint8_t MaxTasks >
void	Scheduler< MaxTasks >::context_save( )
{
//CriticalEnter( );
//CriticalExit( );
    // push all registers to stack
    // *g_pCurrentContext = SP (stack pointer)
    // leave interrupts disabled
    asm volatile(
        "push	r0			\n\t"
        "in	r0, __SREG__		\n\t"
        "cli				\n\t"
        "push	r0			\n\t"
        "push	r1			\n\t"
        "clr	r1			\n\t"
        "push	r2			\n\t"
        "push	r3			\n\t"
        "push	r4			\n\t"
        "push	r5			\n\t"
        "push	r6			\n\t"
        "push	r7			\n\t"
        "push	r8			\n\t"
        "push	r9			\n\t"
        "push	r10			\n\t"
        "push	r11			\n\t"
        "push	r12			\n\t"
        "push	r13			\n\t"
        "push	r14			\n\t"
        "push	r15			\n\t"
        "push	r16			\n\t"
        "push	r17			\n\t"
        "push	r18			\n\t"
        "push	r19			\n\t"
        "push	r20			\n\t"
        "push	r21			\n\t"
        "push	r22			\n\t"
        "push	r23			\n\t"
        "push	r24			\n\t"
        "push	r25			\n\t"
        "push	r26			\n\t"
        "push	r27			\n\t"
        "push	r28			\n\t"
        "push	r29			\n\t"
        "push	r30			\n\t"
        "push	r31			\n\t"
        "lds	r26, g_pCurrentContext	\n\t"
        "lds	r27, g_pCurrentContext+1\n\t"
        "in	r0, __SP_L__		\n\t"
        "st	x+, r0			\n\t"
        "in	r0, __SP_H__		\n\t"
        "st	x+, r0			\n\t":: );
}

template< uint8_t MaxTasks >
void	Scheduler< MaxTasks >::context_switch( )
{
	if ( !threads.count( ) )	// No tasks!!!
		return;

	if ( curr_thread == threads.end( ) )
		curr_thread = threads.begin( );

	//context_load( );
	// *curr_thread will never be NULL as we insert tasks by reference.
	if ( ( *curr_thread )->state == Thread::Running )
	{
//uart1.put( '|' );	//;;;;
		( *curr_thread )->exec( );
	}

	//context_save( );

	++curr_thread;		// Next thread.
}

template< uint8_t MaxTasks >
void	Scheduler< MaxTasks >::yield( )
{
	context_save( );
	context_switch( );
	context_load( );

	asm volatile (
		"sei	\n\t"
		"ret	\n\t":: );	// yield is "naked", we must return to force the task stack load.
}

template< uint8_t MaxTasks >
inline void Scheduler< MaxTasks >::critical_enter( )
{
	asm volatile(
		"in	__tmp_reg__, __SREG__       \n\t"
		"cli                                \n\t"
		"push   __tmp_reg__                 \n\t"::);
}

template< uint8_t MaxTasks >
inline void Scheduler< MaxTasks >::critical_exit( )
{
	asm volatile(
		"pop	__tmp_reg__                \n\t"
		"out     __SREG__, __tmp_reg__     \n\t":: );
}


#endif		// _SCHEDULER_CPP
