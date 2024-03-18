#define F_CPU 7372800UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#define BAUD_ESP 115200
#define BAUDRATE_ESP8266 ((F_CPU)/(BAUD_ESP*16UL)-1)
#define BAUD_RSA 9600
#define BAUDRATE_RSA ((F_CPU)/(BAUD_RSA*16UL)-1)
#define ESP8266_RST PB0
#define BUFFER_SIZE 400



void reset_esp8266() {
	DDRB |= (1 << ESP8266_RST);
	PORTB &= ~(1 << ESP8266_RST);
	_delay_ms(10);
	
	PORTB |= (1 << ESP8266_RST);
	_delay_ms(1000);
}


void uart_esp_init() {
	UBRRH = (uint8_t)(BAUDRATE_ESP8266 >> 8);
	UBRRL = (uint8_t)(BAUDRATE_ESP8266);
	UCSRB = (1 << RXEN)|(1 << TXEN);
	UCSRC = (1 << URSEL)|(3 << UCSZ0);
	UCSRB |= (1 << RXCIE);
	sei();
}

void uart_transmit(unsigned char data) {
	while (!(UCSRA & (1<<UDRE)));
	UDR = data;
}

void send_string(char* string) {
	while (*string) {
		uart_transmit(*string++);
		_delay_ms(50);
	}
}

void uart_send_buffer(const char *buffer) {
	while (*buffer) {
		// Wait for empty transmit buffer
		while (!(UCSRA & (1<<UDRE)));
		// Put data into buffer, sends the data
		UDR = *buffer++;
		_delay_ms(100);
	}
}

void set_same_baud_rate() {
	_delay_ms(2000);
	send_string("AT+UART_CUR=9600,8,1,0,0\r\n");
	PORTA = 0x18;
	_delay_ms(4000);
	// uart_pc_init section
	UBRRH = (uint8_t)(BAUDRATE_RSA>>8);
	UBRRL = (uint8_t)(BAUDRATE_RSA);
}

void disable_receive_complete_interrupt() {
	UCSRB &= ~(1 << RXCIE);
}
void enable_receive_complete_interrupt() {
	UCSRB |= (1 << RXCIE);
}
