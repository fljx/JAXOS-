#ifndef __LIST_H
	#define __LIST_H


#include	<stdlib.h>
#include	"types.h"
#include	<lib/allocators.h>


/**	Abstract base class for STL-like lists.	*/
template< typename T, uint16_t MaxSize = 16, template < typename T, uint16_t MaxSize > class Allocator = StaticAllocator >
class BaseList
{
public:		class NodeIterator;

protected:
	/// List element node.
	struct Node
	{
	private:
		friend class NodeIterator;

		T		item;
		Node	*_next,
				*_prev;

	public:
		Node( )
		:	_next( 0 ), _prev( 0 )
		{}

		Node( const T &item, Node *next, Node *prev )
		:	item( item ), _next( next ), _prev( prev )
		{}

		void	clear( )
		{	_next = _prev = NULL;	}

		operator	T &( )
		{	return	item;	}

		Node *&	next( )//	const
		{	return	_next;	}

		Node *&	prev( )//	const
		{	return	_prev;	}	};

public:
	/// STL-like iterator.
	class NodeIterator
	{
		Node	*node;
	public:
		NodeIterator( Node *n = 0 )
		:	node( n )
		{	}

		T&	operator*( )	{	return	node->item;	}
		T*	operator->( )	{	return	&*( BaseList< T >::NodeIterator )*this;	}

		NodeIterator & operator ++( )
		{	if ( node != NULL )	node = node->next( ); return	*this;	}

		NodeIterator & operator ++( int )
		{	NodeIterator tmp( *this ); ++( *this ); return	tmp;	}

		NodeIterator & operator --( )
		{	if ( node != NULL )	node = node->prev( ); return	*this;	}

		bool operator ==( const NodeIterator &ni ) const
		{
			if ( !node && !ni.node )	return	true;	//	Both NULL -> true.
			if ( !node || !ni.node )	return	false;	//	One of them NULL -> false.
			if ( node == ni.node )		return	true;	//	Both point the same data -> true.
			return	node->item == ni.node->item;		//	None NULL nor equal -> compare contents.
		}

		bool operator !=( const NodeIterator &ni ) const
		{	return	!( *this == ni );	}

		NodeIterator& operator =( const NodeIterator& ni )
		{	node = ni.node; return *this;	}

		operator	Node *( )	const
		{	return	node;	}
	};

	typedef	NodeIterator		iterator;
	typedef	const NodeIterator	const_iterator;
	typedef T&					reference;
    typedef const T&			const_reference;

	BaseList( )
	:	_count( 0 ), head( NULL )
	{
	}

	virtual bool	push_front( const T &item ) = 0;
	virtual void	pop_front( T &item ) = 0;
	virtual bool	push_back( const T &item ) = 0;
	virtual void	pop_back( T &item ) = 0;

	virtual bool	erase( const T &item ) = 0;
	virtual void	clear( ) = 0;

	iterator		begin( )		{	return	head;	}
	const_iterator	begin( ) const	{	return	head;	}
	iterator		end( )			{	return	iterator( NULL );	}
	const_iterator	end( ) const	{	return	iterator( NULL );	}
	reference front( )				{	return	*head;	}
	const_reference front( ) const	{	return	*head;	}
	virtual reference back( )				= 0;
	virtual const_reference back( ) const	= 0;

	int		count( )	const
	{	return	_count;	}

protected:
	int			_count;
	iterator	head;

	Allocator< Node, MaxSize >	allocator;
};


/**	Doubly-linked list.	*/
template< typename T, uint16_t MaxSize = 16, template < typename T, uint16_t MaxSize > class Allocator = StaticAllocator >
class List	:	public	BaseList< T, MaxSize, Allocator >
{
public:
	typedef typename BaseList< T, MaxSize, Allocator >::Node			Node;
	typedef typename BaseList< T, MaxSize, Allocator >::iterator		iterator;
	typedef typename BaseList< T, MaxSize, Allocator >::const_iterator	const_iterator;
	typedef typename BaseList< T, MaxSize, Allocator >::reference		reference;
	typedef typename BaseList< T, MaxSize, Allocator >::const_reference	const_reference;

	using typename BaseList< T, MaxSize, Allocator >::begin;
	using typename BaseList< T, MaxSize, Allocator >::end;

	List( )
	:	BaseList< T, MaxSize, Allocator >( ), tail( NULL )
	{
	}

	bool	push_front( const T &item )
	{
		Node	*n = new( this->allocator ) Node( item, head, NULL );

		if ( n == NULL )
			return	false;		// No free node found.

		if ( head == this->end( ) )	// Empty list has no head or tail.
			head = tail = iterator( n );

		head.prev( ) = n;
		head = n;
		++_count;

		return	true;
	}

	void	pop_front( T &item )
	{
		item = *head;
		this->allocator.deallocate( *head );
		++head;				// Move head forward.
		--_count;
	}

	bool	push_back( const T &item )
	{
		Node	*n = new( this->allocator ) Node( item, NULL, tail );

		if ( n == NULL )
			return	false;		// No free node found.

		if ( head == this->end( ) )	// Empty list has no head or tail.
			head = tail = iterator( n );

		n = Node( item, NULL, tail );
		tail->next = n;
		tail = n;
		++_count;

		return	true;
	}

	void	pop_back( T &item )
	{
		item = *tail;
		this->allocator.deallocate( *tail );
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
				this->allocator.deallocate( *it );
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
			this->allocator.deallocate( head );

		tail = end( );
		_count = 0;
	}

	typename BaseList< T, MaxSize, Allocator >::reference back( )				{	return	*tail;	}
	typename BaseList< T, MaxSize, Allocator >::const_reference back( ) const	{	return	*tail;	}

protected:
	iterator	head,
				tail;

	int		_count;
};


/**	Circular doubly-linked list.	*/
template< typename T, uint16_t MaxSize = 16, template < typename T, uint16_t MaxSize > class Allocator = StaticAllocator >
class CircularList	:	public	BaseList< T, MaxSize, Allocator >
{
public:
	typedef typename BaseList< T, MaxSize, Allocator >::Node			Node;
	typedef typename BaseList< T, MaxSize, Allocator >::iterator		iterator;
	typedef typename BaseList< T, MaxSize, Allocator >::const_iterator	const_iterator;
	typedef typename BaseList< T, MaxSize, Allocator >::reference		reference;
	typedef typename BaseList< T, MaxSize, Allocator >::const_reference	const_reference;

	using typename BaseList< T, MaxSize, Allocator >::begin;
	using typename BaseList< T, MaxSize, Allocator >::end;

	CircularList( )
	:	BaseList< T, MaxSize, Allocator >( )
	{
	}

	///	Insert an element in the begin of the list (moving the head to it).
	bool	push_front( const T &item )
	{
		if ( !push_back( item ) )
			return	false;		// Failed inserting item.

		this->head = ( ( Node * )this->head )->prev( );		// Move head to left, the inserted item.

		return	true;
	}

	///	Remove and return an element from the begin of the list.
	void	pop_front( T &item )
	{
		item = *( this->head );

		iterator	tail = ( ( Node * )this->head )->prev( );
		++this->head;					// Advance head.
		this->allocator.deallocate( *( ( Node * )this->head )->prev( ) );
		( ( Node * )tail )->next( ) = ( ( Node * )this->head );	// Reconnect the pointers.
		( ( Node * )this->head )->prev( ) = tail;

		--this->_count;
	}

	///	Insert an element in the end of the list.
	bool	push_back( const T &item )
	{
		Node	*n = new( this->allocator ) Node( item, this->head, ( ( Node * )this->head )->prev( ) );

		if ( n == NULL )
			return	false;			// No free node found.

		if ( this->head == end( ) )	// Empty list has no head or tail.
			this->head = n;

		iterator	tail = ( ( Node * )this->head )->prev( );
		( ( Node * )this->head )->prev( ) = n;		// Tie to the left.
		( ( Node * )tail )->next( ) = n;			// Tie to the right.
		++this->_count;

		return	true;
	}

	///	Remove and return an element from the end of the list.
	void	pop_back( T &item )
	{
		iterator	tail = ( ( Node * )this->head )->prev( );
		item = *tail;
		this->allocator.deallocate( *( ( ( Node * )this->head )->prev( ) ) );
		( ( Node * )tail )->prev( )->next( ) = this->head;
		( ( Node * )this->head )->prev( ) = ( ( Node * )tail )->prev( );
		--this->_count;
	}

	bool	erase( const T &item )
	{
		iterator it = begin( );

		do
		{
			if ( *it == item )
			{
				Node *n = ( Node * )it;
				n->prev( )->next( ) = n->next( );
				n->next( )->prev( ) = n->prev( );
				this->allocator.deallocate( *n );
				--this->_count;

				if ( this->_count == 0 )
					this->head = end( );

				return	true;
			}

			++it;
		}
		while (  it != end( ) );

		return	false;				// Not found.
	}

	void	clear( )
	{
		for ( ; this->head != end( ); ++this->head )
			this->allocator.deallocate( *( Node * )this->head );

		this->_count = 0;
	}

	reference back( )				{	return	*( ( ( Node * )this->head )->prev( ) );	}
	const_reference back( ) const	{	return	*( ( ( Node * )this->head )->prev( ) );	}

protected:
};


#endif // __LIST_H
