#ifndef _DEVICES_H
	#define _DEVICES_H


/// Abstract base class for all devices that can be writen and read.
template< typename T >
class	IO
{
public:
	virtual bool	get( T &c ) = 0;
	virtual bool	put( T c ) = 0;
};


/// Abstract base class for all devices that can write and read characters.
class	CharDev	:	public IO< char >
{
public:
	virtual int		puts( const char *s ) = 0;

protected:
	virtual void startXmit( ) = 0;
	virtual void stopXmit( ) = 0;
};


/// Abstract base class for all devices that can be turned ON or OFF.
class	OnOffDev
{
public:
	virtual void	on( ) = 0;
	virtual void	off( ) = 0;
};


#endif // _DEVICES_H
