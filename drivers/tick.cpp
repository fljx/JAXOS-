#include	<drivers/tick.h>
#include	<kernel/scheduler.h>

#include	<avr/io.h>
#include	<avr/interrupt.h>


#define T1_CLOCK_PRESCALER				( ( unsigned long ) 64 )
#define T1_PRESCALE_64					( ( unsigned char ) 0x03 )
#define T1_CLEAR_COUNTER_ON_MATCH		( ( unsigned char ) 0x08 )
#define T1_COMPARE_MATCH_A_IE			_BV( OCIE1A )	//( ( unsigned char ) 0x10 )

#define	TICK_TCCRA	TCCR1A
#define	TICK_TCCRB	TCCR1B

#if defined (__AVR_ATmega128__)
	#define	TICK_OCRA	OCR1A
	#define	TICK_TCNT	TCNT1
	#define	TICK_TIMSK	TIMSK
	#define	TICK_OCIE	OCIE1
#else		// Defaults to ATmega1281
	#define	TICK_OCRA	OCR1A
	#define	TICK_TCNT	TCNT1
	#define	TICK_TIMSK	TIMSK1
	#define	TICK_OCIE	OCIE1A
#endif


Tick tick;


static unsigned long	count;
static unsigned long	tick_rate_Hz;
static bool				tick_changed;


bool	Tick::init( unsigned long tick_rate )
{
	if ( !tick_rate )
		return	false;

	tick_rate_Hz = tick_rate;
	tick_changed = false;

	//	Setup compare match value for compare match A.
	//  Interrupts are disabled before this is called so we need not worry here.
	TICK_OCRA = ( F_CPU / tick_rate / T1_CLOCK_PRESCALER ) - 1;

	// Setup clock source and compare match behaviour.
	TICK_TCCRB = T1_CLEAR_COUNTER_ON_MATCH | T1_PRESCALE_64;

	// Enable the interrupt.
	TICK_TIMSK |= T1_COMPARE_MATCH_A_IE;

	return	true;
}


unsigned long	Tick::getTick( void )
{
	return	count;
}


bool	Tick::tickChanged( void )
{
	bool	old_tickChanged = tick_changed;

	tick_changed = false;

	return	old_tickChanged;
}


unsigned long	Tick::ms2ticks( unsigned ms )
{
	return	( unsigned long )ms * tick_rate_Hz / 1000;
}


unsigned int	Tick::ticks2ms( unsigned long tick )
{
	return	( unsigned int )( tick * 1000 / tick_rate_Hz );
}

/**	Busy wait, in milliseconds.	*/
void	Tick::delay( unsigned ms )
{
	unsigned long	final_tick = Tick::getTick( ) + Tick::ms2ticks( ms );

	while ( Tick::getTick( ) != final_tick )
		;
}


ISR( TIMER1_COMPA_vect )
{
	++count;
	tick_changed = true;

	Scheduler<>::tick( );		// Notify sleeping threads.
}
