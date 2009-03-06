#ifndef _PAN_H
	#define _PAN_H


#include	"types.h"
#include	"lib/queue.h"

#include	<drivers/ports.h>
#include	<drivers/pan_frame.h>
#include	<drivers/pan_helper.h>


template< uint8_t BitNo, uint8_t RxBufferSize = 8, uint8_t TxBufferSize = 8 >
class PAN	:	public	PAN_Base< BitNo >
{
public:
	PAN( );

	bool	get( PAN_data &data )	{	return	rx.pop( data );	}
	bool	put( PAN_data data );

	void	key( uint8_t k )		{	_key = k;	}
	uint8_t	key( )					{	return	_key;	}

protected:
	uint8_t	_key;

	Queue< PAN_data, uint8_t, RxBufferSize >	rx;
	Queue< PAN_data, uint8_t, TxBufferSize >	tx;

	bool	push( PAN_data item )	{	return	rx.push( item );	}
	bool	pop( PAN_data &item )	{	return	tx.pop( item );	}
};


#include	<../drivers/pan.cpp>


#endif // _PAN_H
