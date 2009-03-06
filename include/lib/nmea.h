#ifndef _NMEA_H
	#define _NMEA_H


#include	"types.h"
#include	"drivers/devices.h"
#include	"lib/str_utils.h"


class Decoder
{
public:
	struct	DecoderData
	{
		uint16_t	tag;
	};

	enum	DecoderStatus
	{
		DecoderIdle,
		Decoding,
		DecodingFinished,
		DecodingFailed
	};

	typedef	void ( *NMEAReceived )( const DecoderData &data );	///<	NMEA message reception event.

	Decoder( NMEAReceived calback, const char *h )
	:	NMEAReceived_calback( calback ), _header( h )
	{}

	virtual DecoderStatus	operator()( char c ) = 0;
	virtual void	flush( ) = 0;
	virtual void	OnNMEAReceived( ) = 0;

	const char		*header( ) const		{	return	_header;	}

protected:
	NMEAReceived	NMEAReceived_calback;
	const char		*_header;		///<	Message header.
	string			current;
};


class	NMEA
{
public:
	NMEA( CharDev &io )
	:	io( io )
	{}

	// ---- Envio de mensagens. ----
	class SentenceMarker
	{	};

	class Begin	:	public	SentenceMarker
	{
	public:
		Begin( )	{}
	};

	class End	:	public	SentenceMarker
	{
	public:
		End( )	{}
	};

	class IntBase	:	public	SentenceMarker
	{
	public:
		IntBase( )
		:	base( 10 )
		{}

		IntBase	operator()( uint8_t ib )	{	base = ib;	return	*this;	}

	private:
		uint8_t	base;

		friend class NMEA;
	};

	static	Begin	begin;
	static	End		end;
	static	IntBase	int_base;

	NMEA& operator << ( const char* );		/// Sending strings.
	NMEA& operator << ( uint16_t );			/// Sending unsigned integers.
	NMEA& operator << ( uint8_t );			/// Sending bytes.
	NMEA& operator << ( char );				/// Sending characters.
	NMEA& operator << ( const Begin & );	/// Start sentence.
	NMEA& operator << ( const End & );		/// Terminate sentence.
	NMEA& operator << ( const IntBase & );	/// Change integer printing base.

	// ---- Message Reception. ----
	void	setDecoders( Decoder *decoders[], uint8_t count )
	{	_decoders = decoders; decoder_count = count;	}

	enum	ParserState
	{
		NMEAState_waitStart,
		NMEAState_matchHeader,
		NMEAState_getChar,
		NMEAState_waitCheckSum,
		NMEAState_getCheckSum,
		NMEAState_checkEnd,
		NMEAState_parsed,
		NMEAState_error
	};

	ParserState	parse( char c );
	void		reset( );

protected:
	CharDev			&io;

	Decoder			**_decoders;	///<	Parser decoders list.
	uint8_t			decoder_count;	///<	Amount of Decoders in "decoder_info".

	Decoder			*decoder;		///<	Currently parsing message decoder.

	// ---- Message Transmition. ----
	uint8_t	curr_cs;

	// ---- Parser State Machine. ----
	ParserState		state;
	string			current;

	uint16_t		check_sum;
	int				eos_count;		///<	End Of Sentence.

	bool	checkStart( char c );
	void	matchHeader( char c );
	bool	getChar( char c );
	bool	waitCheckSum( char c );
	void	getCheckSum( char c );
	bool	checkEnd( char c );
};


uint8_t	checksum( const char *data );


#endif // _NMEA_H
