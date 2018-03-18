/*
 * Atmeg32a_Master_controller.cpp
 *
 * Created: 26-01-2018 22:54:00
 * Author : Puneet Shrivas
 */ 

#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

uint8_t data; //variable for exchange

void SPI_init_Master(void)  //initialization as master board
{
	DDRB|=(1<<PINB7)|(1<<PINB5); //DDRB&=(0<<PINB6);//sets MOSI and SCK as output
	SPCR|=(1<<SPE)|(1<<MSTR)|(1<<SPIE)|(1<<SPR0); // Enabled SPI, SPI interrupt, device is master, Prescaler Used :		frequency of SCK= Fosc/16 
	sei(); //global interrupt
}

	void UART_init(void)
	{
		UBRRH = (uint8_t)(BAUD_PRESCALLER>>8);
		UBRRL = (uint8_t)BAUD_PRESCALLER;
		UCSRB = (1<<RXEN)|(1<<TXEN); 
		UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0); // Set frame format: 8data, 2stop bit 
	}
 
/*	 uint8_t UART_receive(void)
	 {
 		while(!(UCSRA & (1<<RXC)));
 		return UDR;
	 }
*/ 
	 void UART_send(uint8_t data)
	 {
 		while(!(UCSRA & (1<<UDRE)));
 		UDR=data;
		PORTA^=(1<<PINA0); 
	}

	void led_color (int color)
	{
		if(color==0)  // turns LED red
		{
			PORTA=0b00000000; //reset LED
			PORTA|=(1<<PINA0);     
		}
		else if(color==1) // turns LED green
		{
			PORTA=0b00000000; //reset LED
			PORTA|=(1<<PINA1);
		}
	}

void SPI_transfer(uint8_t data)
{
	TCNT0=0;
	SPDR=data;
	while(!(SPSR&(1<<SPIF))) {;/*wait for data transfer and recieving*/}
	UART_send(TCNT0);
}

	
int main(void)
{
	SPI_init_Master();
	UART_init();
	DDRA|=1<<PINA0|1<<PINA1|1<<PINA2; //PINA0 : Red(0), PINA1 : Green(1)
	PORTA=0b00000000; //set PINA0 and PINA1 as output and low	
	TCCR0|=(1<<CS00);
	TCCR1B|=1<<CS11|1<<WGM12;
	TIMSK|=1<<OCIE1A;
	OCR1A=979;		// required frequency =532us ,prescaler=1, OCR1A= (16Mhz*532us/1)-1 = 8511, OCR1A= 4255 with prescaler 2 and Fosc=16Mhz
    while (1) {}
}

ISR (TIMER1_COMPA_vect) //timing the data transfer
{
	PORTA^=(1<<PINA1);
	
	SPI_transfer(1);
	PORTA^=(1<<PINA2);
	PORTA^=(1<<PINA2);
	PORTA^=(1<<PINA2);
	
}
