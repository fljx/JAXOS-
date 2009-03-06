#include	"crc.h"


// Crc para 2 bytes apenas!
data16	crc_calc_int16( data16 data )
{
	static data16	tmp;		//;;; Static apenas para simplificar a depuração.

	tmp.high = ( data.low ^ 0xFF ) + 1;
	tmp.low = (  data.high ^ 0xFF );

	if( !tmp.high )
		tmp.low++;

	return	tmp;
}
