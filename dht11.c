#define F_CPU 7372800UL
#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#define DHT11_PIN 6

uint8_t c = 0;

// initiate communication with dht11 sensor
void request_dht11()
{
	DDRD |= _BV(DHT11_PIN);
	PORTD |= _BV(DHT11_PIN);
	PORTD &= ~_BV(DHT11_PIN);
	_delay_ms(20);
	PORTD |= _BV(DHT11_PIN);
}
// detect response signal from dht11 sensor, after this you can read data like temperature etc.
void response_from_dht11()
{
	DDRD &= ~_BV(DHT11_PIN);
	while(PIND & _BV(DHT11_PIN));
	while((PIND & _BV(DHT11_PIN)) == 0);
	while(PIND & _BV(DHT11_PIN));
}

uint8_t receive_dht11_data()
{
	for (uint8_t q=0; q<8; q++) {
		while((PIND &  _BV(DHT11_PIN)) == 0);
		_delay_us(60);
		if(PIND & _BV(DHT11_PIN)) {
			c = (c << 1) | (0x01);
			} else {
			c = (c << 1);
		}
		while(PIND & _BV(DHT11_PIN));
	}
	return c;
}
