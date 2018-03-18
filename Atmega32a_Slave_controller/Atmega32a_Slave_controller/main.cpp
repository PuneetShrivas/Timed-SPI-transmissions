/*
 * Atmega32a_Slave_controller.cpp
 *
 * Created: 27-01-2018 20:45:11
 * Author : Puneet Shrivas
 */ 
#define F_CPU 14745600
#include <avr/io.h>
unsigned char data;
void SPI_init_Slave(void)  //initialization as slave board
{
	DDRB|=(1<<PINB6); //sets MISO as output
	SPCR|=(1<<SPE); // Enabled SPI, PRESCALINGLEFT
}

unsigned char SPI_transfer(unsigned char data)
{
	PORTA|=(1<<PINA2);
	SPDR=data;
	while(!(SPSR&(1<<SPIF))) {;/*wait for data transfer and recieving*/}
	return (SPDR);
}

int main(void)
{
	DDRA|=1<<PINA0|1<<PINA1|1<<PINA2; //PINA0 : Off(0), PINA1 : On(1)
	PORTA = 0x00000000;
	SPI_init_Slave();
    while (1) 
    {
	data=SPI_transfer(0);
    }
}

