#ifndef _UART_H
	#define _UART_H


#include	"types.h"
#include	"iomxx0_1.hpp"
#include	<avr/io.h>
#include	<drivers/ports.h>
#include	<drivers/devices.h>
#include	<lib/queue.h>


#define uartBAUD_DOUBLE		( U2X1 )
#define uartBAUD_DIV		( ( uint32_t )( 16 >> ( uartBAUD_DOUBLE >> 1 ) ) )

/// Constants for writing to UCSRB.
#define serRX_INT_ENABLE		( ( unsigned char )_BV( RXCIE1 ) )	// 0x80
#define serRX_ENABLE			( ( unsigned char )_BV( RXEN1 ) )	// 0x10
#define serTX_ENABLE			( ( unsigned char )_BV( TXEN1 ) )	// 0x08
#define serTX_INT_ENABLE		( ( unsigned char )_BV( UDRIE1 ) )	// 0x20

/// Constants for writing to UCSRC.
#define serUCSRC_SELECT			( ( unsigned char )_BV( UMSEL11 ) )	// 0x80
#define serEIGHT_DATA_BITS		( ( unsigned char )_BV( UCSZ11 ) | _BV( UCSZ10 ) )	// 0x06


enum	UART_BaseAddr
{
	Base_UART0 = ooregs::oo_UCSR0A,		//	0xC0.
	Base_UART1 = ooregs::oo_UCSR1A,		//	0xC8.
	Base_UART2 = ooregs::oo_UCSR2A,		//	0xD0.
	Base_UART3 = ooregs::oo_UCSR3A		//	0x130.
};


template< UART_BaseAddr base, uint8_t RxBufferSize = 64, uint8_t TxBufferSize = 64 >
class	UART	:	public CharDev
{
public:
	bool	get( char &c )
	{	return	rx.pop( c );	}

	bool	put( char c )
	{
		if( !tx.push( c ) )
			return	false;

		startXmit( );

		return	true;
	}

	int		puts( const char *s )
	{
		//taskENTER_CRITICAL( );
		for ( ; *s; ++s )
			if ( !put( *s ) )
				return	0;			// EOF
		//taskEXIT_CRITICAL( );

		return	( int )s;
	}

	enum	TX_RX_flags
	{
		TX = _BV( TXEN0 ),
		RX = _BV( RXEN0 )
	};

	inline void enable( TX_RX_flags tx_rx )	const
	{	Regs::UCSRB( ) |= tx_rx;	}

	inline void disable( TX_RX_flags tx_rx )	const
	{	Regs::UCSRB( ) &= ~( tx_rx );	}

	enum	IRQ_flags
	{
		irq_TX = _BV( TXCIE0 ),
		irq_UDRE = _BV( UDRIE0 ),
		irq_RX = _BV( RXCIE0 )
	};

	inline void enable_irq( IRQ_flags tx_rx )	const
	{	Regs::UCSRB( ) |= tx_rx;	}

	inline void disable_irq( IRQ_flags tx_rx )	const
	{	Regs::UCSRB( ) &= ~( tx_rx );	}

	bool	open( );
	void	close( );
	bool	isOpen( );

	int32_t	baud( )	{	return _baud;	}
	void	baud( int32_t b )
	{
		if ( isOpen( ) )
			close( );

		_baud = b;
	}

protected:
	int32_t			_baud;

	Queue< char, uint8_t, RxBufferSize >	rx;
	Queue< char, uint8_t, TxBufferSize >	tx;

	uint16_t baudToCounter( uint32_t b )	const
	{	return ( uint16_t )( ( uint32_t )F_CPU / ( uartBAUD_DIV * b ) ) - 1;	}

	void startXmit( );
	void stopXmit( );

	/// USART Registers.
	class Regs
	{
	public:
		static Register_reference	UCSRA( )
		{	return	*( Register_pointer )base;	}

		static Register_reference	UCSRB( )
		{	return	*( Register_pointer )( base + 1 );	}

		static Register_reference	UCSRC( )
		{	return	*( Register_pointer )( base + 2 );	}

		static Register16_reference	UBRR( )
		{	return	*( Register16_pointer )( base + 4 );	}

		static Register_reference	UBRRL( )
		{	return	*( Register_pointer )( base + 4 );	}

		static Register_reference	UBRRH( )
		{	return	*( Register_pointer )( base + 5 );	}

		static Register_reference	UDR( )
//		{	return	*( Register_pointer )( base + 6 );	}
		{	return	( *( volatile uint8_t * )( base + 6 ) );	}
	};

public:
	void isr_rx( );
	void isr_txe( );
};


template< UART_BaseAddr base, uint8_t RxBufferSize, uint8_t TxBufferSize >
bool	UART< base, RxBufferSize, TxBufferSize >::open( )
{
	if ( !_baud )
		return false;

	Regs::UCSRB( ) = 0x00;			// Disable while configuring.
	Regs::UBRR( ) = baudToCounter( _baud );
	Regs::UCSRA( ) = uartBAUD_DOUBLE;
	Regs::UCSRC( ) = ( serUCSRC_SELECT | serEIGHT_DATA_BITS );				// Set the data bits to 8.
	Regs::UCSRB( ) = ( serRX_INT_ENABLE | serRX_ENABLE | serTX_ENABLE );	// Enable the Rx interrupt.

	return true;
}


template< UART_BaseAddr base, uint8_t RxBufferSize, uint8_t TxBufferSize >
void	UART< base, RxBufferSize, TxBufferSize >::close( )
{
	stopXmit( );
}


template< UART_BaseAddr base, uint8_t RxBufferSize, uint8_t TxBufferSize >
bool	UART< base, RxBufferSize, TxBufferSize >::isOpen( )
{
	return	Regs::UCSRB( ) & serRX_INT_ENABLE;
}


template< UART_BaseAddr base, uint8_t RxBufferSize, uint8_t TxBufferSize >
void	UART< base, RxBufferSize, TxBufferSize >::startXmit( )
{
	Regs::UCSRB( ) |= serTX_INT_ENABLE;
}


template< UART_BaseAddr base, uint8_t RxBufferSize, uint8_t TxBufferSize >
void	UART< base, RxBufferSize, TxBufferSize >::stopXmit( )
{
	Regs::UCSRB( ) &= ~serTX_INT_ENABLE;
}

//-----------------------------------------------------------

template< UART_BaseAddr base, uint8_t RxBufferSize, uint8_t TxBufferSize >
inline void	UART< base, RxBufferSize, TxBufferSize >::isr_rx( )
{
	char	udr = Regs::UDR( );

	rx.push( udr );
}


template< UART_BaseAddr base, uint8_t RxBufferSize, uint8_t TxBufferSize >
inline void	UART< base, RxBufferSize, TxBufferSize >::isr_txe( )
{
	char udr;

	if( tx.pop( udr ) )	// Send the next character queued for Tx.
		Regs::UDR( ) = udr;
	else							// Queue empty, nothing to send.
		stopXmit( );
}


extern	UART< Base_UART0 >	uart0;
extern	UART< Base_UART1 >	uart1;
extern	UART< Base_UART2 >	uart2;
extern	UART< Base_UART3 >	uart3;


#endif
