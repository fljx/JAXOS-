#ifndef _CPP_SUPPORT_H
	#define _CPP_SUPPORT_H

/// http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&t=59453
///	-fno-exceptions

#include <stdlib.h>

void	* operator new( size_t size );
void	operator delete( void * ptr );

void	* operator new[]( size_t size );
void	operator delete[]( void * ptr );

__extension__ typedef int __guard __attribute__(( mode(__DI__) ));

extern "C"
{
	int		__cxa_guard_acquire( __guard * );
	void	__cxa_guard_release( __guard * );
	void	__cxa_guard_abort( __guard * );
	void	__cxa_pure_virtual( void );
	void	atexit( void );
}

#endif // _CPP_SUPPORT_H
