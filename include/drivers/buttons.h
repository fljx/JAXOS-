#ifndef _BUTTONS_H
	#define _BUTTONS_H


#include	<kernel/thread.h>
#include	<drivers/devices.h>
#include	<drivers/ports.h>
#include	<command.h>
#include	<types.h>


/**	Generic button device.

	\note	Actual transition sensing is done in a hardware interrupt not contained in this class.
	\param	ButtonBit	Hardware bit number where the button is sensed.
*/
template< uint8_t ButtonBit >
class Button
{
public:
	Button( );

	uint8_t	bitNo( ) const	{	return	ButtonBit;	}

    inline bool status( ) const
	{	return	Bit::val( );	}

protected:
	typedef	PinChangeIO< ButtonBit >	Bit;

};


/**	Button capable of notifying its state change.

	\par	Message format:
	\par	Byte 0:		MessageHeader
	\par	Byte 1:		b7: <ON|OFF>, b6: -; b5-b0: ButtonBit

	\note	Debouncing is done in Thread ring event.	*/
template< uint8_t ButtonBit, uint8_t MessageHeader, uint16_t DebounceTime = 10 >
class NotifyingButton	:	public Button< ButtonBit >,	public OneShotThread
{
public:
	NotifyingButton( INotifier< data16 > &notifier );

	void	change( );
	void	exec( );

protected:
	INotifier< data16 > &notifier;
	bool	button_state;	//!< Last state of the button.
};


/**	Bimanual push button control.
*/
template< uint8_t ButtonBit0, uint8_t ButtonBit1, uint8_t MessageHeader >
class Bimanual	:	public OneShotThread, public INotifier< data16 >
{
public:
	/// Default constructor.
	Bimanual( INotifier< data16 > &notifier );

	void	change( uint8_t changed );
	void	exec( );

	uint8_t	bitNo0( ) const	{	return	ButtonBit0;	}
	uint8_t	bitNo1( ) const	{	return	ButtonBit1;	}

	void	notify( const data16 &msg );

protected:
	INotifier< data16 > &notifier;
	volatile uint8_t	button_state;	///< Last state for both buttons (may change in an ISR, so volatile).

//	NotifyingButton< ButtonBit0, 0 >	button0;
//	NotifyingButton< ButtonBit1, 1 >	button1;
	NotifyingButton< ButtonBit0, MessageHeader >	button0;
	NotifyingButton< ButtonBit1, MessageHeader >	button1;

	typedef	PinChangeIO< ButtonBit0 >	bit0;
	typedef	PinChangeIO< ButtonBit1 >	bit1;
};


#ifndef _BUTTONS_CPP
	#include	"../drivers/buttons.cpp"
#endif // _BUTTONS_CPP


#endif // _BUTTONS_H
