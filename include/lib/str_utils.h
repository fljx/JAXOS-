#ifndef _STR_UTILS_H
	#define _STR_UTILS_H


#include	<inttypes.h>


/**
	\note Using "copy on change" to preserve memory.
*/
class	string
{
public:

	// ---- Types and Constants. ----
	typedef	uint16_t		size_type;
	static const size_type	npos = -1;

	// ---- Constructors. ----
	string( );
	string( int16_t size );
	string( const string &s );
	string( char *s );
	~string( );

	// ---- Management. ----
	void	alloc( uint16_t size );
	void	free( );
	void	realloc( uint16_t size );

	void	copy( char *buffer ) const;
	string	clone( ) const;

	uint16_t	length( ) const	{	return _lenght;	}
	void		length( uint16_t new_len );

	// ---- Handling. ----
	size_type	find( char c, size_type pos = 0 ) const;
	size_type	find( const string &str, size_type pos = 0 ) const;
	size_type	find_first_of( char c, size_type pos = 0 ) const;
	size_type	find_first_of( const string chars, size_type pos = 0 ) const;
	size_type	find_first_not_of( char c, size_type pos = 0 ) const;
	size_type	find_first_not_of( const string chars, size_type pos = 0 ) const;
	size_type	find_last_of( char c, size_type pos = 0 ) const;
	size_type	find_last_of( const string chars, size_type pos = 0 ) const;
	size_type	find_last_not_of( char c, size_type pos = 0 ) const;
	size_type	find_last_not_of( const string chars, size_type pos = 0 ) const;

	string	substr( size_type pos = 0, size_type n = npos ) const;

	// ---- Operators. ----
	const string & operator +( char c ) const;
	const string & operator +( const string &s ) const;

	string & operator +=( char c );
	string & operator +=( const string &s );

	string & operator =( const string &s );

	bool operator ==( const string &s ) const;
	bool operator ==( const char *s ) const;

	//char operator []( uint16_t i ) const;

	// ---- Conversion Operators. ----
	operator const char *( ) const { return data; }
	operator int16_t( ) const;
	operator uint16_t( ) const;
	operator int32_t( ) const;
	operator uint32_t( ) const;

	operator float( ) const;
	operator double( ) const;

	uint32_t	parseHex( ) const;

protected:
	char		*data;			///<	Pointer to memory data.
	uint16_t	_lenght;		///<	Current lenght of the string.
	uint16_t	_alloc;			///<	Amount of allocated memory.
	bool		allocated;		///<	Indicates if dynamic memory was allocated by the instance.
};


class	Tokenizer
{
public:
	Tokenizer( )
	:	start( 0 ), end( 0 )
	{	}

	string	token( const string &str, const string &delimiters = " \t\r\n" );

protected:
	string::size_type start;
	string::size_type end;
};


#endif // _STR_UTILS_H
