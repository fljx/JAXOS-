#ifndef _PAN_HELPER_H
	#define _PAN_HELPER_H


#include	<drivers/devices.h>
#include	<drivers/pan_frame.h>

template< uint8_t BitNo >	class PAN_Base;
template< uint8_t BitNo >	class PAN_machine_Helper;


/**	PAN send/receive state machine.	*/
template< uint8_t BitNo >
class PAN_machine
{
public:
	PAN_machine( );

	void	exec( );
	bool	listen( );
	bool	next( );
	void	start_bit( );

	void	startXmit( );
	void	stopXmit( );

protected:
	enum PAN_state
	{
		PAN_Listening,
		PAN_WaitIdle,
		PAN_OutBusy,
		PAN_InBusy
	};

	friend class	PAN_Base< BitNo >;
	PAN_Base< BitNo >	*pan;

	uint8_t		state;
	volatile bool		start_bit_detected;

	PAN_frame	curr_frame;
	uint8_t		curr_bit_count;

	uint16_t	timeout;	// FIXME: volatile?
	uint8_t		idle_count;
	uint8_t		bit_phase;

	static const uint8_t	BitTime = 3;
	static const uint8_t	MaxIdleCount = 5 * BitTime;
	static const uint16_t	WaitIdleTimeout = 1000 * BitTime;

	void	send_bit( );
	void	recv_bit( );
	void	append_bit( bool bit );
	void	wait_idle( );

	typedef	IntPinIO< BitNo >	Pin;

	void	on( )	const		{	Pin::on( );	}
	void	off( )	const		{	Pin::off( );	}
	void	toggle( )	const	{	Pin::toggle( );	}
	bool	in( )	const		{	return	Pin::val( );	}

	void	input( )	const	{	return	Pin::input( );	}
	void	output( )	const	{	return	Pin::output( );	}

	void	interruptOn( )	const	{	Pin::interruptOn( );	}
	void	interruptOff( )	const	{	Pin::interruptOff( );	}
};


template< uint8_t BitNo >
class PAN_machine_Helper
{
	friend class	PAN_machine< BitNo >;
	static PAN_machine< BitNo >	mchn;

public:
	static PAN_machine< BitNo > *	machine( );
	static void		try_exec( );
	static void		try_start_bit( );
};


template< uint8_t BitNo >
class PAN_Base	:	public	IO< PAN_data >
{
public:
	void	setup( );

protected:
	PAN_Base( );

	friend class	PAN_machine< BitNo >;
	PAN_machine< BitNo >	*machine;

	virtual bool	push( PAN_data item )	= 0;
	virtual bool	pop( PAN_data &item )	= 0;

	void	startXmit( )	{	machine->startXmit( );	}
	void	stopXmit( )		{	machine->stopXmit( );	}
};


#include	<../drivers/pan_helper.cpp>


#endif // _PAN_HELPER_H
