#include	<lib/nmea.h>
#include	<stdlib.h>


// ----------------------------------------------------------------------------
// ---------------- Transmition -----------------------------------------------
// ----------------------------------------------------------------------------

NMEA::Begin		NMEA::begin;
NMEA::End		NMEA::end;
NMEA::IntBase	NMEA::int_base;


///	Outputs a string, updating the current checksum.
NMEA&	NMEA::operator << ( const char *s )
{
	for ( ; *s; ++s )			// Send sentence contents, including header.
		*this << *s;			// Sends a char and updates the checksum.

	return	*this;
}

///	Outputs an unsigned int, according to the currently configured base and updating the current checksum.
NMEA&	NMEA::operator << ( uint16_t u )
{
	char	u_ascii[ 6 ];

	utoa( u, u_ascii, NMEA::int_base.base );

	*this << u_ascii;

	return	*this;
}

///	Outputs a byte as uppercase hexadecimal with 2 digits, zero filled.
NMEA&	NMEA::operator << ( uint8_t u )
{
	*this << XLAT[ ( u >> 4 ) ];
	*this << XLAT[ u & 0x0f ];

	return	*this;
}

///	Outputs a single character, updating the current checksum.
NMEA&	NMEA::operator << ( char c )
{
	io.put( c );			// The sentence may go corrupted if io.put fails!
	curr_cs ^= c;

	return	*this;
}

///	Prepares a new sentence transmition.
NMEA&	NMEA::operator << ( const Begin & )
{
	curr_cs = 0;

	io.put( '$' );				// Send sentence prefix.

	return	*this;
}

///	Terminates sentence transmition.
NMEA&	NMEA::operator << ( const End & )
{
	io.put( '*' );				// Send checksum separator.
	*this << curr_cs;			// Send checksum.
	io.put( '\r' );				// Send End Of Sentence.
	io.put( '\n' );

	return	*this;
}

///	Configures the integer base for printing numbers.
NMEA&	NMEA::operator << ( const IntBase & )
{
	return	*this;
}

// ----------------------------------------------------------------------------
// ---------------- Parser engine (reception) ---------------------------------
// ----------------------------------------------------------------------------

/**	Recebe stream de dados e os decodifica quando completar mensagem.

	\param	c	Novo dado recebidos.

	\return	NMEAState_parsed somente quando uma mensagem completa foi decodificada com sucesso.
*/
NMEA::ParserState	NMEA::parse( char c )
{
	// Se encontrar marcador de início de mensagem, reinicia a máquina, ainda que isso
	//	cause a perda da mensagem atual (incompleta).
	if ( !checkStart( c ) )
	{
		switch ( state )
		{
			case NMEAState_matchHeader:
				matchHeader( c );
				break;
			case NMEAState_getChar:
				if ( getChar( c ) )
					return state;
				break;
			case NMEAState_waitCheckSum:
				if ( waitCheckSum( c ) )	// Recebendo checksum, não atualiza mais o calculado.
					return state;
				break;
			case NMEAState_getCheckSum:
				getCheckSum( c );
				return state;
			case NMEAState_checkEnd:
				if ( checkEnd( c ) )
					return	NMEAState_parsed;
				return	state;
			//case NMEAState_error:	case NMEAState_parsed: case NMEAState_waitStart:
			default:
				return state;
		}

		check_sum ^= c;
	}

	return state;
}

/// Descarta processamento pendente e reinicia o parser.
void	NMEA::reset( )
{
	state = NMEAState_waitStart;
	//current.free( );
	//current.length( 0 );
}

/// Verifica o início de uma sentença NMEA.
bool	NMEA::checkStart( char c )
{
	if ( c == '$' )
	{
		//current.free( );
		current.length( 0 );
		check_sum = 0;
		state = NMEAState_matchHeader;

		return	true;
	}

	return	false;
}

/** Procura os cabeçalhos utilizados na sentença sendo recebida.

	Quando um desses cabeçalhos é identificado, o decodificador adequado é selecionado
	e a máquina de estados avança para a recepção dos campos de dados.

	\param	c	Caractere recebido.
*/
void	NMEA::matchHeader( char c )
{
	uint8_t	i;

	if ( c == ',' )
	{
		for ( i = 0; i < decoder_count; ++i )
		{
			if ( current == _decoders[ i ]->header( ) )
			{
				decoder = _decoders[ i ];
				decoder->flush( );
				state = NMEAState_getChar;	// Começa a decodificar campos.
				return;
			}
		}

		// Nenhuma mensagem de interesse, esperar a próxima.
		state = NMEAState_waitStart;
	}
	else
		current += c;
}

///	Decodes received character.
bool	NMEA::getChar( char c )
{
	switch ( ( *decoder )( c ) )
	{
		case Decoder::DecodingFinished:		// If finished, wait checksum.
			if ( waitCheckSum( c ) )		// Checksum may come right after decoding finish.
				return	true;

		case Decoder::DecoderIdle:
		case Decoder::DecodingFailed:
				state = NMEAState_waitCheckSum;
			break;

		case Decoder::Decoding:
		default:
			return	false;
	}

	return	false;
}


bool	NMEA::waitCheckSum( char c )
{
	if ( c == '*' )
	{
		state = NMEAState_getCheckSum;
		//current.free( );
		current.length( 0 );
		return true;
	}

	return false;
}


void	NMEA::getCheckSum( char c )
{
	current += c;

	if ( current.length( ) == 2 )	// Segundo caractere do checksum.
	{
		if ( check_sum == ( uint16_t )current.parseHex( ) )
		{
			state = NMEAState_checkEnd;
			eos_count = 0;
		}
		else
			state = NMEAState_waitStart;
	}
}


bool	NMEA::checkEnd( char c )
{
	switch ( eos_count )
	{
		case 0:								// Final de mensagem, o CR do par [CR][LF].
			if ( c == '\r' )
				++eos_count;
			return false;
		case 1:								// Final de mensagem OK.
			if ( c == '\n' )
			{
				decoder->OnNMEAReceived( );

				return true;
			}

			break;
	}

	// Erro no final da mensagem, aborta recepção.
	state = NMEAState_waitStart;

	return false;
}

// ----------------------------------------------------------------------------
// ----------------- Utility --------------------------------------------------
// ----------------------------------------------------------------------------

uint8_t	checksum( const char *data )
{
	uint8_t	cs = 0;

	for ( ; *data; ++data )
		cs ^= *data;

	return	cs;
}
