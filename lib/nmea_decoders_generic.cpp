#include	"lib/nmea_decoders_generic.h"


void	Decoder_Generic::OnNMEAReceived( )
{
	NMEAReceived_calback( data );
}

/** Generic NMEA message Decoder.

	Splits colon separated fields of an NMEA message.	*/
Decoder::DecoderStatus	Decoder_Generic::operator()( char c )
{
	if ( c == ',' )
	{
		data.data.push_back( current );
	}
	else if ( c == '*' )
	{
		data.data.push_back( current );

		return DecodingFinished;
	}

	current += c;

	return Decoding;
}

void	Decoder_Generic::flush( )
{
	current.length( 0 );
	data.data.clear( );
}
