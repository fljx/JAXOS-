#ifndef __LIST_H
	#define __LIST_H


#include	<stdlib.h>
#include	"types.h"
#include	"lib/allocators.h"


/**	Abstract base class for STL-like lists.	*/
//template< typename T, template < typename T, uint16_t MaxSize = 16 > class Allocator = StaticAllocator< T, MaxSize > >
template< typename T, uint16_t MaxSize = 16,
	template < typename, uint16_t > class Allocator = StaticAllocator >
class BaseList
{
protected:
	/// List element node.
	struct Node
	{
		T		item;
		Node	*_next,
				*_prev;

		Node( )
		:	_next( 0 ), _prev( 0 )
		{}

		Node( const T &item, Node *next, Node *prev )
		:	item( item ), _next( next ), _prev( prev )
		{}

		void	clear( )
		{	_next = _prev = NULL;	}
	};

public:
	/// STL-like iterator.
	class NodeIterator
	{
		Node	*node;
	public:
		T&	operator*( )	{	return	node->item;	}
		T*	operator->( )	{	return	&*( /*BaseListList< T >::*/NodeIterator )*this;	}

		NodeIterator & operator ++( )
		{	if ( node != NULL )	node = node->_next; return	*this;	}

		NodeIterator & operator ++( int )
		{	NodeIterator tmp( *this ); ++( *this ); return	tmp;	}

		NodeIterator & operator --( )
		{	if ( node != NULL )	node = node->_prev; return	*this;	}

		bool operator ==( const NodeIterator &ni ) const
		{	return	node == &( ni.node );	}

		bool operator !=( const NodeIterator &ni ) const
		{	return	node != &( ni.node );	}

		NodeIterator& operator =( const NodeIterator& ni )
		{	node = ni.node; return *this;	}
	};

	typedef	NodeIterator		iterator;
	typedef	const NodeIterator	const_iterator;
	typedef T&					reference;
    typedef const T&			const_reference;


	BaseList( )
	:	_count( 0 )
	{
	}

	virtual bool	push_front( const T &item ) = 0;
	virtual void	pop_front( T &item ) = 0;
	virtual bool	push_back( const T &item ) = 0;
	virtual void	pop_back( T &item ) = 0;

	virtual bool	erase( const T &item ) = 0;
	virtual void	clear( ) = 0;

	int		count( )	const
	{	return	_count;	}

protected:
	int	_count;

	Allocator< T, MaxSize >	allocator;
};


/**	Doubly-linked list.	*/
template< typename T, uint16_t MaxSize, template < typename T, uint16_t MaxSize > class Allocator >
class List	:	public	BaseList< T, MaxSize, Allocator >
{
public:

	List( )
	:	BaseList< T, MaxSize, Allocator >( )
	{
		head = tail = end( );
	}

	bool	push_front( const T &item )
	{
		typename BaseList< T, MaxSize, Allocator >::Node &n =
			BaseList< T, MaxSize, Allocator >::allocator.allocate( );

		if ( n == NULL )
			return	false;		// No free node found.

		if ( head == end( ) )	// Empty list has no head or tail.
			head = tail = iterator( n );

		n = BaseList< T, MaxSize, Allocator >::Node( item, head, NULL );
		head->prev = &n;
		head = &n;
		++_count;

		return	true;
	}

	void	pop_front( T &item )
	{
		item = *head;
		BaseList< T, MaxSize, Allocator >::allocator.deallocate( *head );
		++head;				// Move head forward.
		--_count;
	}

	bool	push_back( const T &item )
	{
		typename BaseList< T, MaxSize, Allocator >::Node &n =
			BaseList< T, MaxSize, Allocator >::allocator.allocate( );

		if ( n == NULL )
			return	false;		// No free node found.

		if ( head == end( ) )	// Empty list has no head or tail.
			head = tail = iterator( n );

		n = BaseList< T, MaxSize, Allocator >::Node( item, NULL, tail );
		tail->next = &n;
		tail = &n;
		++_count;

		return	true;
	}

	void	pop_back( T &item )
	{
		item = *tail;
		BaseList< T, MaxSize, Allocator >::allocator.deallocate( *tail );
		--tail;				// Move tail backward.
		--_count;
	}

	bool	erase( const T &item )
	{
		for ( iterator it = begin( ); it != end( ); ++it )
			if ( it == item )
			{
				*it.prev->next = *it.next;
				*it.next->prev = *it.prev;
				BaseList< T, MaxSize, Allocator >::allocator.deallocate( *it );
				--_count;

				if ( _count == 0 )
					head = tail = end( );

				return	true;
			}

		return	false;				// Not found.
	}

	void	clear( )
	{
		for ( ; head != end( ); ++head )
			BaseList< T, MaxSize, Allocator >::allocator.deallocate( head );

		tail = end( );
		_count = 0;
	}

	typedef	typename BaseList<T, MaxSize, Allocator>::iterator			iterator;
	typedef	typename BaseList<T, MaxSize, Allocator>::const_iterator	const_iterator;
	typedef	typename BaseList<T, MaxSize, Allocator>::reference			reference;
	typedef	typename BaseList<T, MaxSize, Allocator>::const_reference	const_reference;

	iterator		begin( )		{	return	head;	}
	const_iterator	begin( ) const	{	return	head;	}
	iterator		end( )			{	return	iterator( NULL );	}
	const_iterator	end( ) const	{	return	iterator( NULL );	}
	reference front( )				{	return	*head;	}
	const_reference front( ) const	{	return	*head;	}
	reference back( )				{	return	*tail;	}
	const_reference back( ) const	{	return	*tail;	}

protected:
	iterator	head,
				tail;

	int		_count;
};


/**	Circular doubly-linked list.	*/
template< typename T, uint16_t MaxSize, template < typename T, uint16_t MaxSize > class Allocator >
class CircularList	:	public	BaseList< T, MaxSize, Allocator >
{
public:

	CircularList( )
	:	BaseList< T, MaxSize, Allocator >( )
	{
		head = end( );
	}

	///	Insert an element in the begin of the list (moving the head to it).
	bool	push_front( const T &item )
	{
		if ( !push_back( item ) )
			return	false;		// Failed inserting item.

		head = head->prev;		// Move head to left, the inserted item.

		return	true;
	}

	///	Remove and return an element from the begin of the list.
	void	pop_front( T &item )
	{
		item = *head;

		iterator	tail = head->prev;
		BaseList< T, MaxSize, Allocator >::allocator.deallocate( *head );
		++head;
		tail->next = head;		// Reconnect the pointers leaving behind the previous head
		head->prev = tail;		//	(no memory deallocation, as said above).

		--_count;
	}

	///	Insert an element in the end of the list.
	bool	push_back( const T &item )
	{
		typename BaseList< T, MaxSize, Allocator >::Node &n =
			BaseList< T, MaxSize, Allocator >::allocator.allocate( );

		if ( n == NULL )
			return	false;		// No free node found.

		if ( head == end( ) )	// Empty list has no head or tail.
			head = iterator( n );

		iterator	tail = head->prev;
		head->prev = &n;		// Tie on left.
		tail->next = &n;		// Tie on right.
		n = BaseList< T, MaxSize, Allocator >::Node( item, head, tail );
		++_count;

		return	true;
	}

	///	Remove and return an element from the end of the list.
	void	pop_back( T &item )
	{
		iterator	tail = head->prev;
		item = *tail;
		BaseList< T, MaxSize, Allocator >::allocator.deallocate( *head->prev );
		tail->prev->next = head;
		head->prev = tail->prev;
		--_count;
	}

	bool	erase( const T &item )
	{
		iterator it = begin( );

		do
		{
			if ( it == item )
			{
				*it.prev->next = *it.next;
				*it.next->prev = *it.prev;
				BaseList< T, MaxSize, Allocator >::allocator.deallocate( *it );
				--_count;

				if ( _count == 0 )
					head = end( );

				return	true;
			}

			++it;
		}
		while (  it != end( ) );

		return	false;				// Not found.
	}

	void	clear( )
	{
		for ( ; head != end( ); ++head )
			BaseList< T, MaxSize, Allocator >::allocator.deallocate( head );

		_count = 0;
	}

	typedef	typename BaseList<T, MaxSize, Allocator>::iterator			iterator;
	typedef	typename BaseList<T, MaxSize, Allocator>::const_iterator	const_iterator;
	typedef	typename BaseList<T, MaxSize, Allocator>::reference			reference;
	typedef	typename BaseList<T, MaxSize, Allocator>::const_reference	const_reference;

	iterator		begin( )		{	return	head;	}
	const_iterator	begin( ) const	{	return	head;	}
	iterator		end( )			{	return	iterator( NULL );	}
	const_iterator	end( ) const	{	return	iterator( NULL );	}
	reference front( )				{	return	*head;	}
	const_reference front( ) const	{	return	*head;	}
	reference back( )				{	return	*head->prev;	}
	const_reference back( ) const	{	return	*head->prev;	}

protected:
	iterator	head;

	int		_count;
};


#endif // __LIST_H
