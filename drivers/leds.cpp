#include	"drivers/leds.h"
#include	"drivers/ports.h"
#include	<avr/io.h>


#define ledsALL_BITS_OUTPUT			( ( unsigned char ) LED_YELLOW | LED_RED | LED_GREEN )
#define ledsALL_OUTPUTS_OFF			( ( unsigned char ) ledsALL_BITS_OUTPUT )	// Complementar.


LEDs leds;

//------------- LEDs ----------------------------------------------------------

/**	Configure all LED pins.
	LED Amarelo:	0x10
	LED Vermelho:	0x20
	LED Verde:		0x40
*/
LEDs::LEDs( )
:	green( 10000 ),
	yellow( 100 ),
	red( 10000 )
{
	// Set port B direction to outputs.  Start with all output off.
	IOPort< Base_PORTC >::DDR( ) = ledsALL_BITS_OUTPUT;
	IOPort< Base_PORTC >::PORT( ) = ledsALL_OUTPUTS_OFF;
}


void	LEDs::on( uint8_t led )
{
	switch ( led )
	{
		case LED_GREEN:
			green.on( );
			break;
		case LED_YELLOW:
			yellow.on( );
			break;
		default:
			red.on( );
			break;
	}
}


void	LEDs::off( uint8_t led )
{
	switch ( led )
	{
		case LED_GREEN:
			green.off( );
			break;
		case LED_YELLOW:
			yellow.off( );
			break;
		default:
			red.off( );
			break;
	}
}
