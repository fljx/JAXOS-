#ifndef _CONTEXT_H
	#define _CONTEXT_H


/// CRTP base class avoiding inheritance in derived Context types.
template< class ContextType >
class ThreadContext
{
public:
	static inline void	load( )	__attribute__(( always_inline ));
	static inline void	save( )	__attribute__(( always_inline ));
};

/// Full Context save and restore handling.
class FullContext	:	ThreadContext< FullContext >
{
public:
	FullContext( );

	static inline void	load( )	__attribute__(( always_inline ));
	static inline void	save( )	__attribute__(( always_inline ));
};

/// Empty Context save and restore handling.
class VoidContext	:	ThreadContext< VoidContext >
{
public:
	VoidContext( );

	static inline void	load( )	__attribute__(( always_inline ));
	static inline void	save( )	__attribute__(( always_inline ));
};

	#include	<../kernel/context.cpp>


#endif // _CONTEXT_H
