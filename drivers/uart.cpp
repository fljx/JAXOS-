#include	<drivers/uart.h>
#include	<avr/interrupt.h>


UART< Base_UART0 >	uart0;
UART< Base_UART1 >	uart1;


ISR( USART0_RX_vect )
{
	uart0.isr_rx( );
}


ISR( USART0_UDRE_vect )
{
	uart0.isr_txe( );
}


ISR( USART1_RX_vect )
{
	uart1.isr_rx( );
}


ISR( USART1_UDRE_vect )
{
	uart1.isr_txe( );
}


#if defined(__ATmegaxx0__)

UART< Base_UART2 >	uart2;
UART< Base_UART3 >	uart3;


ISR( USART2_RX_vect )
{
	uart2.isr_rx( );
}


ISR( USART2_UDRE_vect )
{
	uart2.isr_txe( );
}


ISR( USART3_RX_vect )
{
	uart3.isr_rx( );
}


ISR( USART3_UDRE_vect )
{
	uart3.isr_txe( );
}

#endif
