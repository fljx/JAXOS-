#ifndef _TICK_H
	#define _TICK_H


#include	"types.h"


class Tick
{
public:
	bool			init( unsigned long tick_rate );
	unsigned long	getTick( void );
	bool			tickChanged( void );
	void			delay( unsigned ms );

	static unsigned long	ms2ticks( unsigned ms );
	static unsigned int		ticks2ms( unsigned long tick );
};


extern Tick tick;


#endif // _TICK_H
