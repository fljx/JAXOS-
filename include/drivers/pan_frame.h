#ifndef _PAN_FRAME_H
	#define _PAN_FRAME_H


#include	"types.h"


class	PAN_frame_proxy;		// Forward definition.
//class	PAN_data;				// Forward definition.

struct	PAN_data
{
	uint8_t	id,
			data,
			key;

	PAN_data( )	{}
	PAN_data( uint8_t id, uint8_t data, uint8_t key )
	:	id( id ), data( data ), key( key )
	{}
};


class PAN_frame
{
public:
	PAN_frame( )	{}
	PAN_frame( const PAN_data &data );
	PAN_frame( bitfield8 id, bitfield8 data, bitfield8 key )
	{	init( id, data, key );	}

	bool	at( uint8_t i )	const;
	bool	operator[]( uint8_t i )	const	{	return at( i );	}
	PAN_frame_proxy	operator[]( uint8_t i );

	bool		decode( PAN_data &data );
	bitfield8	check( const PAN_data &data );
	bitfield8	check( uint8_t id, uint8_t data, uint8_t key );
	void		compute( uint8_t byte, uint8_t &acc );

	static const uint8_t	length = 40;

	friend class	PAN_frame_proxy;

private:
	bitfield8	bytes[ 5 ];

	void	init( bitfield8 id, bitfield8 data, bitfield8 key );
};

/**	Proxy class to handle PAN_frame bits.	*/
class	PAN_frame_proxy
{
public:
	PAN_frame_proxy( PAN_frame &f, uint8_t i )
	:	frame( f ), index( i )
	{}

	PAN_frame&	operator=( bool b )
	{
		if ( PAN_frame::length >= index )
		{
			if ( b )
				frame.bytes[ index >> 3 ] |= 1 << ( index & 0x07 );
			else
				frame.bytes[ index >> 3 ] &= ~( 1 << ( index & 0x07 ) );
		}

		return frame;
	}

	operator	bool( )	{	return	frame.at( index );	}

private:
	PAN_frame	&frame;
	uint8_t		index;
};


#endif // _PAN_FRAME_H
