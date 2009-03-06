#ifndef __ALLOCATORS_H
	#define __ALLOCATORS_H


template< typename T, uint16_t MaxSize >
class BaseAllocator
{
public:
	virtual T *		allocate( )	= 0;
	virtual void	deallocate( T &item )	= 0;
};


/**	(Hopefully) STL-like memory allocator with no dynamic memory allocation.

	The items are copied into a statically allocated draft array to avoid problems with malloc/free in AVRlibc.
	\param	T			Type of the data to be stored.
	\param	MaxSize		Size of the draft array and therefore the maximum number of items storable.
	*/
template< typename T, uint16_t MaxSize >
class StaticAllocator	:	public	BaseAllocator< T, MaxSize >
{
public:
	StaticAllocator( )
	{
		for ( uint8_t c = 0; c < mask_length; ++c )
			busy_mask[ 0 ] = 0;
	}

	T *		allocate( );
	void	deallocate( T &item );

protected:
	T			elements[ MaxSize ];

	typedef uint8_t			index_t;
	static const uint8_t	mask_length = MaxSize / sizeof( index_t ) / 8;
	index_t					busy_mask[ mask_length ];
};


template< typename T, uint16_t MaxSize >
T *		StaticAllocator< T, MaxSize >::allocate( )
{
	for ( uint8_t i = 0; i < mask_length; ++i )
		for ( index_t j = 0; j < sizeof( index_t ) * 8; ++j )
			if ( !( busy_mask[ i ] & ( 1 << j ) ) )	// Found a free element.
			{
				busy_mask[ i ] |= ( 1 << j );
				return	&elements[ i * sizeof( index_t ) * 8 + j ];
			}

	return	NULL;		// End of buffer (cannot allocate more).
}

template< typename T, uint16_t MaxSize >
void	StaticAllocator< T, MaxSize >::deallocate( T &item )
{
	for ( uint16_t i = 0; i < mask_length; ++i )
		for ( uint16_t j = 0; j < 16; ++j )
			if ( &item == &elements[ i * 16 + j ] )	// Found the element.
				busy_mask[ i ] &= ~( 1 << j );		// Clear busy flag.
}


template< typename T, uint16_t MaxSize >
class DynamicAllocator
{
public:
	T *		allocate( )				{	return ( T * )malloc( sizeof( T ) );	}
	void	deallocate( T &item )	{	free( &item );	}
};


template< typename T, uint16_t MaxSize, template < typename T, uint16_t MaxSize > class Allocator >
void *	operator new( size_t /*size*/, Allocator< T, MaxSize > &alloc )
{	return	alloc.allocate( );	}

template< typename T, uint16_t MaxSize, template < typename T, uint16_t MaxSize > class Allocator >
void	operator delete( void *item, Allocator< T, MaxSize > &alloc )
{	alloc.deallocate( *( T * )item );	}


#endif // __ALLOCATORS_H
