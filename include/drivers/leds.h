#ifndef _LEDS_H
	#define _LEDS_H


#include	"drivers/timedio.h"


enum	LED_IDs
{
	LED_YELLOW	=	0x10,
	LED_RED		=	0x20,
	LED_GREEN	=	0x40
};


class	LEDs
{
public:
	LEDs( );

	void	on( uint8_t led );
	void	off( uint8_t led );

private:
	OneShotIO< LED_GREEN, Base_PORTC >	green;
	ToggleIO< LED_YELLOW, Base_PORTC >	yellow;
	OneShotIO< LED_RED, Base_PORTC >	red;
};


extern LEDs leds;


#endif

