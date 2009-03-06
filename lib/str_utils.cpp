#include	"lib/str_utils.h"
#include	"types.h"

#include	<stdlib.h>
#include	<string.h>


// ---- Constructors. ----
string::string( )
{
	data = NULL;
	_lenght = _alloc = 0;
	allocated = false;
}

string::string( int16_t size )
{
	alloc( size );
}

string::string( const string &s )
{
	*this = s;
}

/**	Constructor with no memory allocation.
*/
string::string( char *s )
{
	if ( s )
	{
		data = s;
		_lenght = _alloc = strlen( s ) + 1;	// Just fitting!
	}
	else
	{
		data = NULL;
		_lenght = _alloc = 0;
	}

	allocated = false;
}

string::~string( )
{
	if ( allocated )
		free( );
}


// ---- Management. ----

/**	Allocates memory for the string data.
	\note	It is a destructive method! Previous content will be lost.
*/
void	string::alloc( uint16_t size )
{
	if ( data )
		delete[] data;

	if ( size )
	{
		data = new char[ size ];
		_alloc = size;
		_lenght = 0;
		data[ 0 ] = '\0';
		allocated = true;
	}
	else
	{
		data = NULL;
		_lenght = _alloc = 0;
		allocated = false;
	}
}

void	string::free( )
{
	delete[]	data;
	data = NULL;
	_lenght = _alloc = 0;
	allocated = false;
}

/** Realocates string data, copying it contents.

	\note	If the new size is smaller than previous, exceeding data will be lost.

	\param	size	New lenght for the allocated (!= used) memory space.
*/
void	string::realloc( uint16_t size )
{
	char		*old = data;
	uint16_t	i;

	data = new char[ size ];

	for ( i = 0; i < size - 1; ++i )
		data[ i ] = old[ i ];

	_alloc = size;
	data[ i ] = '\0';

	delete[] old;
}

/** Copies the string contents do a char buffer.

	\note	buffer \b must have enough space!
*/
void	string::copy( char *buffer ) const
{
	for ( char	*pdata1 = data; pdata1 < data + _lenght; ++pdata1, ++buffer )
		*buffer = *pdata1;
	*buffer = '\0';
}

/** Explicitly makes a copy of the string.	*/
string	string::clone( ) const
{
	string	draft( _alloc );

	copy( draft.data );

	draft._lenght = _lenght;

	return	draft;
}

/**	Sets a new lenght for the string.

	If new_len is equal to the current lenght, this method does nothing.
	If new_len is lesser than current lenght, string is truncated, but memory is not reallocated.
	If new_len is greater than current lenght, string is reallocated.

	\param	new_len	New string lenght in characters.
*/
void	string::length( uint16_t new_len )
{
	if ( new_len == _lenght )
		return;

	if ( new_len < _lenght )
	{
		_lenght = new_len;
		data[ _lenght ] = '\0';
		return;
	}

	realloc( new_len );		// Not enough room for new lenght, needs reallocation.
}

// ---- Handling. ----
/**	Find character in string.	*/
string::size_type	string::find( char c, size_type pos ) const
{
	if ( !data || pos >= length( ) )
		return	string::npos;

	char	*pdata = data + pos;

	for ( ; *pdata; ++pdata )
		if ( *pdata == c )
			return	( size_type )( pdata - data );

	return	string::npos;
}

/**	Find content in string.	*/
string::size_type	string::find( const string &str, size_type pos ) const
{
	if ( !data || !str.data || pos >= length( ) || str.length( ) + pos > length( ) )
		return	string::npos;

	char	*pdata = data + pos,
			*pstr = str.data;

	for ( ; *pdata && *pstr; ++pdata )
		if ( *pdata == *pstr )
			++pstr;

	if ( !*pstr )	// pstr == str.data + str.length( ) )
		return	( size_type )( pdata - data );

	return	string::npos;
}

string::size_type	string::find_first_of( char c, size_type pos ) const
{
	if ( !data || pos >= length( ) )
		return	string::npos;

	for ( char	*pdata = data + pos; *pdata; ++pdata )
		if ( *pdata == c )
			return	( size_type )( pdata - data );

	return	string::npos;
}

string::size_type	string::find_first_of( const string chars, size_type pos ) const
{
	if ( !data || !chars.data || pos >= length( ) || !chars.length( ) )
		return	string::npos;

	for ( char	*pdata = data + pos; *pdata; ++pdata )
		for ( char	*pdel = chars.data; *pdel; ++pdel )
			if ( *pdata == *pdel )
				return	( size_type )( pdata - data );

	return	string::npos;
}

string::size_type	string::find_first_not_of( char c, size_type pos ) const
{
	if ( !data || pos >= length( ) )
		return	string::npos;

	for ( char	*pdata = data + pos; *pdata; ++pdata )
		if ( *pdata != c )
			return	( size_type )( pdata - data );

	return	string::npos;
}

string::size_type	string::find_first_not_of( const string chars, size_type pos ) const
{
	if ( !data || !chars.data || pos >= length( ) || !chars.length( ) )
		return	string::npos;

	for ( char	*pdata = data + pos; *pdata; ++pdata )
	{
		char	*pdel = chars.data;
		for ( ; *pdel && *pdata != *pdel; ++pdel )
			;

		if ( !*pdel ) // pdel == chars.data + chars.length( ) )
			return	( size_type )( pdata - data );
	}

	return	string::npos;
}

string::size_type	string::find_last_of( char c, size_type pos ) const
{
	if ( !data || pos >= length( ) )
		return	string::npos;

	for ( char	*pdata = data + pos; pdata >= data; --pdata )
		if ( *pdata == c )
			return	( size_type )( pdata - data );

	return	string::npos;
}

string::size_type	string::find_last_of( const string chars, size_type pos ) const
{
	if ( !data || !chars.data || pos >= length( ) || !chars.length( ) )
		return	string::npos;

	for ( char	*pdata = data + pos; pdata >= data; --pdata )
		for ( char	*pdel = chars.data; *pdel; ++pdel )
			if ( *pdata == *pdel )
				return	( size_type )( pdata - data );

	return	string::npos;
}

string::size_type	string::find_last_not_of( char c, size_type pos ) const
{
	if ( !data || pos >= length( ) )
		return	string::npos;

	for ( char	*pdata = data + pos; pdata >= data; --pdata )
		if ( *pdata != c )
			return	( size_type )( pdata - data );

	return	string::npos;
}

string::size_type	string::find_last_not_of( const string chars, size_type pos ) const
{
	if ( !data || !chars.data || pos >= length( ) || !chars.length( ) )
		return	string::npos;

	for ( char	*pdata = data + pos; pdata >= data; --pdata )
	{
		char	*pdel = chars.data;
		for ( ; *pdel && *pdata != *pdel; ++pdel )
			;

		if ( !*pdel ) // pdel == chars.data + chars.length( ) )
			return	( size_type )( pdata - data );
	}

	return	string::npos;
}

string	string::substr( size_type pos, size_type n ) const
{
	if ( pos >= length( ) )
		return	string( );

	if ( n == string::npos )
		n = length( ) - pos;

	string	draft( n + 4 );
	char	*pdata1 = data + pos,
			*pdata2 = draft.data;

	for ( ; pdata1 < data + pos + n; ++pdata1, ++pdata2 )
		*pdata2 = *pdata1;
	*pdata2 = '\0';

	draft.length( n );

	return	draft;
}

// ---- Operators. ----

/** Concatenates a single character to the string.

	\note	Returns a new concatated copy of the original string!
*/
const string & string::operator +( char c ) const
{
	return	this->clone( ) + c;
}

/** Concatenates two strings.

	\note	Returns a new concatated copy of the original string!
*/
const string & string::operator +( const string &s ) const
{
	string	draft( _lenght + s._lenght + 8 );	// Avoid reallocation.

	return	draft + *this + s;
}

string & string::operator +=( char c )
{
	if ( !data || _lenght + 1 >= _alloc )
		realloc( _lenght + 8 );		// Make room for concatenating.

	data[ _lenght ] = c;
	_lenght++;
	data[ _lenght ] = '\0';

	return	*this;
}

/**
	\todo	This method changes the object contents, so it should make a clone to preserve the original references.
*/
string & string::operator +=( const string &s )
{
	if ( !data || ( _lenght + s._lenght + 1 ) >= _alloc )
		realloc( _lenght + s._lenght + 8 );		// Make room for concatenating plus some extra bytes.

	uint16_t	j;

	for ( j = 0; j < s._lenght; ++j, ++_lenght )
		data[ _lenght ] = s[ j ];

	data[ ++_lenght ] = '\0';

	return	*this;
}

string & string::operator =( const string &s )
{
	if ( this != &s )      // Same object?
	{
		data = s.data;
		_alloc = s._alloc;
		_lenght = s._lenght;
	}

	return *this;
}

bool string::operator ==( const string &s ) const
{
	if ( _lenght != s._lenght )
		return	false;

	return	*this == s.data;
}

bool string::operator ==( const char *s ) const
{
	if ( data == s )
		return	true;

	if ( !data || !s )
		return	false;

	char	*pdata = data;

	for ( ; *pdata && *s; ++pdata, ++s )
		if ( *pdata != *s )
			return	false;

	return	true;
}

/**
	\note	This operator is "read-only", since the string data can be shared across instances.
*/
//char string::operator []( uint16_t i ) const
//{
//	if ( i < _lenght )
//		return data[ i ];
//
//	return	'\0';
//}

// ---- Conversion Operadores. ----

string::operator int16_t( ) const
{
	return	( int16_t )atoi( data );
}

string::operator uint16_t( ) const
{
	return	( uint16_t )atoi( data );
}

string::operator int32_t( ) const
{
	return	( int32_t )atol( data );
}

string::operator uint32_t( ) const
{
	return	( uint32_t )atol( data );
}

string::operator float( ) const
{
	return	( float )atof( data );
}

string::operator double( ) const
{
	return	atof( data );
}

uint32_t	string::parseHex( ) const
{
	return	( uint32_t )strtoul( data, NULL, 16 );
}


// ----------------------------------------------------------------------------
// ----------------- Utility --------------------------------------------------
// ----------------------------------------------------------------------------

string	Tokenizer::token( const string &str, const string &delimiters )
{
	start = str.find_first_not_of( delimiters, end );

	if ( start == string::npos )
	{
		end = start = 0;
		return	string( );
	}

	end = str.find_first_of( delimiters, start );

	if ( end == string::npos || end <= start )
	{
		end = str.length( );
		return	str.substr( start );
	}

	return	str.substr( start, end - start );
}
