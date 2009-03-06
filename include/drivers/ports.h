#ifndef _PORTS_H
	#define _PORTS_H


#include	<avr/io.h>
#include	"iomxx0_1.hpp"


/*	From "avr/io.h"

	"__SFR_OFFSET must be 0 or 0x20"
	#define _SFR_IO8(io_addr) ( *( volatile uint8_t * )((io_addr) + __SFR_OFFSET) )
	#define PINx	_SFR_IO8(PORT_BASE + 0X00)
	#define DDRx	_SFR_IO8(PORT_BASE + 0X01)
	#define PORTx   _SFR_IO8(PORT_BASE + 0X02)

	PORT_BASE:
		PORT A base address at (0x00 + __SFR_OFFSET).
		PORT B base address at (0x03 + __SFR_OFFSET).
		PORT C base address at (0x06 + __SFR_OFFSET).
		PORT D base address at (0x09 + __SFR_OFFSET).
		PORT E base address at (0x0c + __SFR_OFFSET).
		PORT F base address at (0x0f + __SFR_OFFSET).
		PORT G base address at (0x12 + __SFR_OFFSET).
*/

//-------------------------------------------------------------
//------------- Register Access -------------------------------
//-------------------------------------------------------------

typedef	volatile uint8_t	Register;
typedef	Register *			Register_pointer;
typedef	Register &			Register_reference;

typedef	volatile uint16_t	Register16;
typedef	Register16 *		Register16_pointer;
typedef	Register16 &		Register16_reference;


///	IO Ports base addresses.
enum	Port_BaseAddr
{
	Base_PORTA = ooregs::oo_PINA,
	Base_PORTB = ooregs::oo_PINB,
	Base_PORTC = ooregs::oo_PINC,
	Base_PORTD = ooregs::oo_PIND,
	Base_PORTE = ooregs::oo_PINE,
	Base_PORTF = ooregs::oo_PINF,
	Base_PORTG = ooregs::oo_PING,
	Base_PORTH = ooregs::oo_PINH
};

namespace	IOPortOffsets
{
//	enum	IOPortOffsets
//	{
//		DDR = Base_PORTA - ooregs::oo_DDRA,		/// Port Direction offset (from pin change base port).
//		PIN = Base_PORTA - ooregs::oo_PINA,		/// Port Input offset (from pin change base port).
//		POUT = Base_PORTA - ooregs::oo_PORTA	/// Port Output offset (from pin change base port).
//	};
	static const uint8_t	DDR = ooregs::oo_DDRA - Base_PORTA;		/// Port Direction offset (from pin change base port).
	static const uint8_t	PIN = ooregs::oo_PINA - Base_PORTA;		/// Port Input offset (from pin change base port).
	static const uint8_t	POUT = ooregs::oo_PORTA - Base_PORTA;	/// Port Output offset (from pin change base port).
};


///	Register bit handling.
template< uint8_t reg >
class RegBit
{
public:
	static inline void bitOn( uint8_t bitNo )
	{	( *( volatile uint8_t * )reg ) |= _BV( bitNo );	}

	static inline void bitOff( uint8_t bitNo )
	{	( *( volatile uint8_t * )reg ) &= ~_BV( bitNo );	}

	static inline void bitToggle( uint8_t bitNo )
	{	( *( volatile uint8_t * )reg ) ^= _BV( bitNo );	}

	static inline bool bitVal( uint8_t bitNo )
	{	return	( *( volatile uint8_t * ) reg ) & _BV( bitNo );	}
};


/// IO Port Registers Access.
template< uint8_t RegNo >
class IOPort
{
public:
	static Register_reference	PIN( )
	{	return	*( Register_pointer )( RegNo + IOPortOffsets::PIN );	}
	//static Register PIN = static_cast< Register >( RegNo );

	static Register_reference	DDR( )
	{	return	*( Register_pointer )( RegNo + IOPortOffsets::DDR );	}

	static Register_reference	PORT( )
	{	return	*( Register_pointer )( RegNo + IOPortOffsets::POUT );	}

	static inline void bitOn( uint8_t bitNo )
	{	( *( Register_pointer )( RegNo + IOPortOffsets::POUT ) ) |= _BV( bitNo );	}		// Write to PORTx.

	static inline void bitOff( uint8_t bitNo )
	{	( *( Register_pointer )( RegNo + IOPortOffsets::POUT ) ) &= ~_BV( bitNo );	}	// Write to PORTx.

	static inline void bitToggle( uint8_t bitNo )
	{	( *( Register_pointer )( RegNo + IOPortOffsets::POUT ) ) ^= _BV( bitNo );	}		// Write to PORTx.

	static inline bool bitVal( uint8_t bitNo )
	{	return	( *( Register_pointer )RegNo ) & _BV( bitNo );	}		// Read from PINx.
};


///	Simple Port IO pin.
template< uint8_t BitNo >
class IOPin
{
public:
	static inline void output( )
	{	RegBit< reg + IOPortOffsets::DDR >::bitOn( bit );	}

	static inline void input( )
	{	RegBit< reg + IOPortOffsets::DDR >::bitOff( bit );	}

	static inline void on( )
	{	RegBit< reg + IOPortOffsets::POUT >::bitOn( bit );	}

	static inline void off( )
	{	RegBit< reg + IOPortOffsets::POUT >::bitOff( bit );	}

	static inline void toggle( )
	{	RegBit< reg + IOPortOffsets::POUT >::bitToggle( bit );	}

	/// Return the specific bit value.
	static inline bool val( )
	{	return	RegBit< reg + IOPortOffsets::PIN >::bitVal( bit );	}

protected:
	/// Port Base Register address.
	static const uint8_t	reg = Base_PORTA + ( BitNo / 8 ) * ( Base_PORTB - Base_PORTA );

	///	In-register bit number.
	static const uint8_t	bit = BitNo % 8;
};


/** Pin Change Interrupt Capable Pins (PCINT).

	Pin Change begin at PORTB, BitNo is a linear count from PB0 across all other ports.
*/
template< uint8_t BitNo >
class PinChangeIO
{
public:
	///	PCMSK0 == 0x6b, PCMSK1 == 0x6c, PCMSK2 == 0x6d.
	static inline void interruptOn( )
	{
		RegBit< ooregs::oo_PCMSK0 + ( BitNo / 8 ) >::bitOn( bit );	// Enable bit.
		RegBit< ooregs::oo_PCICR >::bitOn( BitNo / 8 );				// Enable group.
	}

	static inline void interruptOff( )
	{
		RegBit< ooregs::oo_PCMSK0 + reg >::bitOff( bit );
	}

	// FIXME: Support pin change > 7.
	static inline void output( )
	{	RegBit< reg + IOPortOffsets::DDR >::bitOn( bit );	}

	static inline void input( )
	{	RegBit< reg + IOPortOffsets::DDR >::bitOff( bit );	}

	static inline void on( )
	{	RegBit< reg + IOPortOffsets::POUT >::bitOn( bit );	}

	static inline void off( )
	{	RegBit< reg + IOPortOffsets::POUT >::bitOff( bit );	}

	/// Return the IO pin value.
	static inline bool val( )
	{	return	RegBit< reg + IOPortOffsets::PIN >::bitVal( bit );	}

protected:

	/// Register offset (from pin change base port).
	static const uint8_t	reg = Base_PORTB + ( BitNo / 8 ) * ( Base_PORTB - Base_PORTA );

	///	In-register bit number.
	static const uint8_t	bit = BitNo % 8;
};


/// Interrupt Capable Pins (INT).
template< uint8_t BitNo >
class IntPinIO
{
public:
	///	PCMSK0 == 0x6b, PCMSK1 == 0x6c, PCMSK2 == 0x6d.
	static inline void interruptOn( )
	{
		( *( Register16_pointer )ooregs::oo_EICRA ) |= ( 3 << ( bit << 1 ) );	// Rising edge.
		RegBit< ooregs::oo_EIMSK >::bitOn( bit );
	}

	static inline void interruptOff( )
	{
		RegBit< ooregs::oo_EIMSK >::bitOff( bit );
	}

	static inline void output( )
	{	RegBit< reg + IOPortOffsets::DDR >::bitOn( bit );	}

	static inline void input( )
	{	RegBit< reg + IOPortOffsets::DDR >::bitOff( bit );	}

	static inline void on( )
	{	RegBit< reg + IOPortOffsets::POUT >::bitOn( bit );	}

	static inline void off( )
	{	RegBit< reg + IOPortOffsets::POUT >::bitOff( bit );	}

	static inline void toggle( )
	{	RegBit< reg + IOPortOffsets::POUT >::bitToggle( bit );	}

	/// Return the specific bit value.
	static inline bool val( )
	{	return	RegBit< reg + IOPortOffsets::PIN >::bitVal( bit );	}

protected:
	/// Port Base Register address.
	static const uint8_t	reg = Base_PORTD + ( BitNo / 4 ) * ( Base_PORTB - Base_PORTA );

	///	In-register bit number.
	static const uint8_t	bit = BitNo % 8;
};


#endif // _PORTS_H
