#include	"cpp_support.h"

/// http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&t=59453

void * operator new( size_t size )
{
	return malloc( size );
}


void operator delete( void *ptr )
{
	free( ptr );
}

void * operator new[]( size_t size )
{
    return malloc( size );
}

void operator delete[]( void * ptr )
{
    free( ptr );
}

int		__cxa_guard_acquire( __guard *g )
{
	return !*( char * )( g );
}

void	__cxa_guard_release( __guard *g )
{
	*( char * )g = 1;
}

void	__cxa_guard_abort( __guard * )
{
}

void	__cxa_pure_virtual( void )
{
}

void	atexit( void ) { }

/** \brief Pure-virtual workaround.

 The avr-libc does not support a default implementation for handling
 possible pure-virtual calls. This is a short and empty workaround for this.

 http://www.mikrocontroller.net/topic/68813
*/
//extern "C"
//{
//	void __cxa_pure_virtual( )
//	{
//		// put error handling here
//	}
//}
