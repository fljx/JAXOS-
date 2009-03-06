#ifndef _NMEA_DECODERS_GENERIC
	#define _NMEA_DECODERS_GENERIC


#include	"lib/nmea.h"
#include	"lib/list.h"


class	Decoder_Generic	:	public	Decoder
{
public:
	struct	Data	:	public	DecoderData
	{
		List< string * >	data;
	};

	typedef	void ( *GenericReceived )( const Data &data );	///<	Message reception eve	.

	Decoder_Generic( GenericReceived calback, const char *header )
	:	Decoder( ( NMEAReceived )calback, header )
	{
	}

	void	OnNMEAReceived( );
	DecoderStatus	operator()( char c );
	void	flush( );

protected:
	Data	data;
};


#endif // _NMEA_DECODERS_GENERIC
