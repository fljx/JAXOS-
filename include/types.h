#ifndef _TYPES_H
	#define _TYPES_H


#include	<inttypes.h>


union	data32
{
	char	chars[ 4 ];
	uint8_t	bytes[ 4 ];

	struct
	{
		uint16_t	low,
					high;
	};

	uint32_t	data;

	uint8_t &	operator[]( uint8_t i )
	{	return	bytes[ i ];	}

	data32	operator=( uint32_t val )	{	data = val;	return	*this;	}

	data32( )	{}
	data32( uint16_t low, uint16_t high )
	:	low( low ), high( high )
	{}
	data32( uint32_t val )	{	data = val;	}
	data32( const uint8_t src[ 4 ] )
	{
		bytes[ 0 ] = src[ 0 ];
		bytes[ 1 ] = src[ 1 ];
		bytes[ 2 ] = src[ 2 ];
		bytes[ 3 ] = src[ 3 ];
	}
};


union	data16
{
	char	chars[ 2 ];
	uint8_t	bytes[ 2 ];

	struct
	{
		uint8_t	low,
				high;
	};

	uint16_t	data;

	uint8_t &	operator[]( uint8_t i )
	{	return	bytes[ i ];	}

	data16	operator=( uint16_t val )	{	data = val;	return	*this;	}
	operator	uint16_t( )	const		{	return	data;	}

	data16( )	{}
	data16( uint8_t low, uint8_t high )
	:	low( low ), high( high )
	{}
};


union	bitfield8
{
	char	ascii;
	uint8_t	byte;

	bitfield8( )
	{}

	bitfield8( uint8_t val )
	:	byte( val )
	{}

	bool operator[]( uint8_t i )	const
	{
		if ( i > 0x07 )
			return	false;

		return	byte & ( 1 << i );
	}

	operator	uint8_t&( )	{	return	byte;	}
};


/// Split ISR definition from prototype declaration and body implementation (took from "avr/interrupt.h").
#ifdef __cplusplus
	#define ISR_DECL(vector, ...)            \
		extern "C" void vector( void ) __attribute__ ((signal,__INTR_ATTRS)) __VA_ARGS__
	#define ISR_BODY(vector, ...)		void vector( void )
#else
	#define ISR_DECL(vector, ...)            \
		void vector (void) __attribute__ ((signal,__INTR_ATTRS)) __VA_ARGS__
	#define ISR_BODY(vector, ...)		void vector( void )
#endif

/// Define an interrupt vector as a class static member function.
#define CLASS_IRQ(name, vector) \
    static void name(void) asm(__STRINGIFY(vector)) __attribute__ ((signal, __INTR_ATTRS))


const char	xlat[] = "0123456789abcdef";
const char	XLAT[] = "0123456789ABCDEF";

// To dump a byte 'b':
//uart1.put( '[' );
//uart1.put( xlat[ ( b >> 4 ) ] );
//uart1.put( xlat[ b & 0x0f ] );
//uart1.put( ']' );

#endif // _TYPES_H
