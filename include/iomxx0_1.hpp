#ifndef	_IOMxx0_1_HPP
	#define	_IOMxx0_1_HPP

// Header file:	WinAVR/avr/include/avr/iomxx0_1.h
#include	<avr/io.h>

namespace	ooregs
{

	enum	SFR_IO8
	{
		oo_PINA = 0X00 + __SFR_OFFSET,
		oo_DDRA = 0X01 + __SFR_OFFSET,
		oo_PORTA = 0X02 + __SFR_OFFSET,
		oo_PINB = 0X03 + __SFR_OFFSET,
		oo_DDRB = 0x04 + __SFR_OFFSET,
		oo_PORTB = 0x05 + __SFR_OFFSET,
		oo_PINC = 0x06 + __SFR_OFFSET,
		oo_DDRC = 0x07 + __SFR_OFFSET,
		oo_PORTC = 0x08 + __SFR_OFFSET,
		oo_PIND = 0x09 + __SFR_OFFSET,
		oo_DDRD = 0x0A + __SFR_OFFSET,
		oo_PORTD = 0x0B + __SFR_OFFSET,
		oo_PINE = 0x0C + __SFR_OFFSET,
		oo_DDRE = 0x0D + __SFR_OFFSET,
		oo_PORTE = 0x0E + __SFR_OFFSET,
		oo_PINF = 0x0F + __SFR_OFFSET,
		oo_DDRF = 0x10 + __SFR_OFFSET,
		oo_PORTF = 0x11 + __SFR_OFFSET,
		oo_PING = 0x12 + __SFR_OFFSET,
		oo_DDRG = 0x13 + __SFR_OFFSET,
		oo_PORTG = 0x14 + __SFR_OFFSET,
		oo_TIFR0 = 0x15 + __SFR_OFFSET,
		oo_TIFR1 = 0x16 + __SFR_OFFSET,
		oo_TIFR2 = 0x17 + __SFR_OFFSET,
		oo_TIFR3 = 0x18 + __SFR_OFFSET,
		oo_TIFR4 = 0x19 + __SFR_OFFSET,
		oo_TIFR5 = 0x1A + __SFR_OFFSET,
		oo_PCIFR = 0x1B + __SFR_OFFSET,
		oo_EIFR = 0x1C + __SFR_OFFSET,
		oo_EIMSK = 0x1D + __SFR_OFFSET,
		oo_GPIOR0 = 0x1E + __SFR_OFFSET,
		oo_EECR = 0x1F + __SFR_OFFSET,
		oo_EEDR = 0X20 + __SFR_OFFSET,
		oo_EEARL = 0x21 + __SFR_OFFSET,
		oo_EEARH = 0X22 + __SFR_OFFSET,
		oo_GTCCR = 0x23 + __SFR_OFFSET,
		oo_TCCR0A = 0x24 + __SFR_OFFSET,
		oo_TCCR0B = 0x25 + __SFR_OFFSET,
		oo_TCNT0 = 0X26 + __SFR_OFFSET,
		oo_OCR0A = 0X27 + __SFR_OFFSET,
		oo_OCR0B = 0X28 + __SFR_OFFSET,
		oo_GPIOR1 = 0x2A + __SFR_OFFSET,
		oo_GPIOR2 = 0x2B + __SFR_OFFSET,
		oo_SPCR = 0x2C + __SFR_OFFSET,
		oo_SPSR = 0x2D + __SFR_OFFSET,
		oo_SPDR = 0X2E + __SFR_OFFSET,
		oo_ACSR = 0x30 + __SFR_OFFSET,
		oo_MONDR = 0x31 + __SFR_OFFSET,
		oo_OCDR = 0x31 + __SFR_OFFSET,
		oo_SMCR = 0x33 + __SFR_OFFSET,
		oo_MCUSR = 0x34 + __SFR_OFFSET,
		oo_MCUCR = 0X35 + __SFR_OFFSET,
		oo_SPMCSR = 0x37 + __SFR_OFFSET,
		oo_RAMPZ = 0X3B + __SFR_OFFSET,
		oo_EIND = 0X3C + __SFR_OFFSET
	};

	enum	SFR_IO16
	{
		oo_EEAR = 0x21,
	};

	enum	SFR_MEM8
	{
		oo_WDTCSR = 0x60,
		oo_CLKPR = 0x61,
		oo_PRR0 = 0x64,
		oo_PRR1 = 0x65,
		oo_OSCCAL = 0x66,
		oo_PCICR = 0x68,
		oo_EICRA = 0x69,
		oo_EICRB = 0x6A,
		oo_PCMSK0 = 0x6B,
		oo_PCMSK1 = 0x6C,
		oo_PCMSK2 = 0x6D,
		oo_TIMSK0 = 0x6E,
		oo_TIMSK1 = 0x6F,
		oo_TIMSK2 = 0x70,
		oo_TIMSK3 = 0x71,
		oo_TIMSK4 = 0x72,
		oo_TIMSK5 = 0x73,
		oo_XMCRA = 0x74,
		oo_XMCRB = 0x75,
		oo_ADCL = 0x78,
		oo_ADCH = 0x79,
		oo_ADCSRA = 0x7A,
		oo_ADCSRB = 0x7B,
		oo_ADMUX = 0x7C,
		oo_DIDR2 = 0x7D,
		oo_DIDR0 = 0x7E,
		oo_DIDR1 = 0x7F,
		oo_TCCR1A = 0x80,
		oo_TCCR1B = 0x81,
		oo_TCCR1C = 0x82,
		oo_TCNT1L = 0x84,
		oo_TCNT1H = 0x85,
		oo_ICR1L = 0x86,
		oo_ICR1H = 0x87,
		oo_OCR1AL = 0x88,
		oo_OCR1AH = 0x89,
		oo_OCR1BL = 0x8A,
		oo_OCR1BH = 0x8B,
		oo_OCR1CL = 0x8C,
		oo_OCR1CH = 0x8D,
		oo_TCCR3A = 0x90,
		oo_TCCR3B = 0x91,
		oo_TCCR3C = 0x92,
		oo_TCNT3L = 0x94,
		oo_TCNT3H = 0x95,
		oo_ICR3L = 0x96,
		oo_ICR3H = 0x97,
		oo_OCR3AL = 0x98,
		oo_OCR3AH = 0x99,
		oo_OCR3BL = 0x9A,
		oo_OCR3BH = 0x9B,
		oo_OCR3CL = 0x9C,
		oo_OCR3CH = 0x9D,
		oo_TCCR4A = 0xA0,
		oo_TCCR4B = 0xA1,
		oo_TCCR4C = 0xA2,
		oo_TCNT4L = 0xA4,
		oo_TCNT4H = 0xA5,
		oo_ICR4L = 0xA6,
		oo_ICR4H = 0xA7,
		oo_OCR4AL = 0xA8,
		oo_OCR4AH = 0xA9,
		oo_OCR4BL = 0xAA,
		oo_OCR4BH = 0xAB,
		oo_OCR4CL = 0xAC,
		oo_OCR4CH = 0xAD,
		oo_TCCR2A = 0xB0,
		oo_TCCR2B = 0xB1,
		oo_TCNT2 = 0xB2,
		oo_OCR2A = 0xB3,
		oo_OCR2B = 0xB4,
		oo_ASSR = 0xB6,
		oo_TWBR = 0xB8,
		oo_TWSR = 0xB9,
		oo_TWAR = 0xBA,
		oo_TWDR = 0xBB,
		oo_TWCR = 0xBC,
		oo_TWAMR = 0xBD,
		oo_UCSR0A = 0xC0,
		oo_UCSR0B = 0XC1,
		oo_UCSR0C = 0xC2,
		oo_UBRR0L = 0xC4,
		oo_UBRR0H = 0xC5,
		oo_UDR0 = 0XC6,
		oo_UCSR1A = 0xC8,
		oo_UCSR1B = 0XC9,
		oo_UCSR1C = 0xCA,
		oo_UBRR1L = 0xCC,
		oo_UBRR1H = 0xCD,
		oo_UDR1 = 0XCE,
		oo_UCSR2A = 0xD0,
		oo_UCSR2B = 0XD1,
		oo_UCSR2C = 0xD2,
		oo_UBRR2L = 0xD4,
		oo_UBRR2H = 0xD5,
		oo_UDR2 = 0XD6,
		oo_PINH = 0x100,
		oo_DDRH = 0x101,
		oo_PORTH = 0x102,
		oo_PINJ = 0x103,
		oo_DDRJ = 0x104,
		oo_PORTJ = 0x105,
		oo_PINK = 0x106,
		oo_DDRK = 0x107,
		oo_PORTK = 0x108,
		oo_PINL = 0x109,
		oo_DDRL = 0x10A,
		oo_PORTL = 0x10B,
		oo_TCCR5A = 0x120,
		oo_TCCR5B = 0x121,
		oo_TCCR5C = 0x122,
		oo_TCNT5L = 0x124,
		oo_TCNT5H = 0x125,
		oo_ICR5L = 0x126,
		oo_ICR5H = 0x127,
		oo_OCR5AL = 0x128,
		oo_OCR5AH = 0x129,
		oo_OCR5BL = 0x12A,
		oo_OCR5BH = 0x12B,
		oo_OCR5CL = 0x12C,
		oo_OCR5CH = 0x12D,
		oo_UCSR3A = 0x130,
		oo_UCSR3B = 0X131,
		oo_UCSR3C = 0x132,
		oo_UBRR3L = 0x134,
		oo_UBRR3H = 0x135,
		oo_UDR3 = 0X136,
	};

	enum	SFR_MEM16
	{
		oo_ADC = 0x78,
		oo_ADCW = 0x78,
		oo_TCNT1 = 0x84,
		oo_ICR1 = 0x86,
		oo_OCR1A = 0x88,
		oo_OCR1B = 0x8A,
		oo_OCR1C = 0x8C,
		oo_TCNT3 = 0x94,
		oo_ICR3 = 0x96,
		oo_OCR3A = 0x98,
		oo_OCR3B = 0x9A,
		oo_OCR3C = 0x9C,
		oo_TCNT4 = 0xA4,
		oo_ICR4 = 0xA6,
		oo_OCR4A = 0xA8,
		oo_OCR4B = 0xAA,
		oo_OCR4C = 0xAC,
		oo_UBRR0 = 0xC4,
		oo_UBRR1 = 0xCC,
		oo_UBRR2 = 0xD4,
		oo_TCNT5 = 0x124,
		oo_ICR5 = 0x126,
		oo_OCR5A = 0x128,
		oo_OCR5B = 0x12A,
		oo_OCR5C = 0x12C,
		oo_UBRR3 = 0x134,
	};

	enum	ISR_VECT
	{
		oo_INT0_vect = 1,
		oo_INT1_vect = 2,
		oo_INT2_vect = 3,
		oo_INT3_vect = 4,
		oo_INT4_vect = 5,
		oo_INT5_vect = 6,
		oo_INT6_vect = 7,
		oo_INT7_vect = 8,
		oo_PCINT0_vect = 9,
		oo_PCINT1_vect = 10,
		oo_PCINT2_vect = 11,
		oo_WDT_vect = 12,
		oo_TIMER2_COMPA_vect = 13,
		oo_TIMER2_COMPB_vect = 14,
		oo_TIMER2_OVF_vect = 15,
		oo_TIMER1_CAPT_vect = 16,
		oo_TIMER1_COMPA_vect = 17,
		oo_TIMER1_COMPB_vect = 18,
		oo_TIMER1_COMPC_vect = 19,
		oo_TIMER1_OVF_vect = 20,
		oo_TIMER0_COMPA_vect = 21,
		oo_TIMER0_COMPB_vect = 22,
		oo_TIMER0_OVF_vect = 23,
		oo_SPI_STC_vect = 24,
		oo_USART0_RX_vect = 25,
		oo_USART0_UDRE_vect = 26,
		oo_USART0_TX_vect = 27,
		oo_ANALOG_COMP_vect = 28,
		oo_ADC_vect = 29,
		oo_EE_READY_vect = 30,
		oo_TIMER3_CAPT_vect = 31,
		oo_TIMER3_COMPA_vect = 32,
		oo_TIMER3_COMPB_vect = 33,
		oo_TIMER3_COMPC_vect = 34,
		oo_TIMER3_OVF_vect = 35,
		oo_USART1_RX_vect = 36,
		oo_USART1_UDRE_vect = 37,
		oo_USART1_TX_vect = 38,
		oo_TWI_vect = 39,
		oo_SPM_READY_vect = 40,
		oo_TIMER4_CAPT_vect = 41,
		oo_TIMER4_COMPA_vect = 42,
		oo_TIMER4_COMPB_vect = 43,
		oo_TIMER4_COMPC_vect = 44,
		oo_TIMER4_OVF_vect = 45,
		oo_TIMER5_CAPT_vect = 46,
		oo_TIMER5_COMPA_vect = 47,
		oo_TIMER5_COMPB_vect = 48,
		oo_TIMER5_COMPC_vect = 49,
		oo_TIMER5_OVF_vect = 50,
		oo_USART2_RX_vect = 51,
		oo_USART2_UDRE_vect = 52,
		oo_USART2_TX_vect = 53,
		oo_USART3_RX_vect = 54,
		oo_USART3_UDRE_vect = 55,
		oo_USART3_TX_vect = 56,
	};

};


#endif
