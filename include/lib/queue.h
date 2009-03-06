#ifndef _QUEUE_H
	#define _QUEUE_H


#include	<stdlib.h>


template< typename T, typename IndexType, uint16_t Size >
class Queue
{
public:
	Queue( )
	{
		iIn = iOut = 0;
	}

	~Queue( )
	{
		iIn = iOut = 0;
	}

	bool	push( T item )
	{
		IndexType	next = NEXT_i( iIn );

		if ( next == iOut )				// Queue cheia.
			return	false;

		buffer[ iIn ] = item;
		iIn = next;

		return	true;
	}

	bool	pop( T &item )
	{
		if ( iIn == iOut )
			return	false;							// Queue vazia.

		item = buffer[ iOut ];
		iOut = NEXT_i( iOut );

		return	true;
	}

	void	flush( )
	{
		iIn = iOut = 0;
	}

	IndexType	count( )
	{
		return	( iOut - ( iIn + 1 ) ) % Size;
	}

protected:
	IndexType	iIn,
				iOut;

	T			buffer[ Size ];

	IndexType	NEXT_i( IndexType index )
	{
		//return	( ( index + 1 ) & ( size - 1 ) );		// Funciona apenas para potências de 2.
		return	( ( index + 1 ) % Size );					// Funciona apenas qualquer tamanho, mas é mais lento.
	}

	bool	indexOverflow( IndexType index )
	{
		return ( index >= Size );	// IndexType é unsigned porque Size o é.
	}
};


#endif // _QUEUE_H
