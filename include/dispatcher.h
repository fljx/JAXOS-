#ifndef DISPATCH_H
	#define DISPATCH_H


#include	"types.h"


class Dispatcher
{
public:
	Dispatcher( );

	static bool	init( );
	static void	dispatch( );	//	Repasse de dados do PC para o Rastreador e vice-versa.
};


#endif

