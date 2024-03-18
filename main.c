#define F_CPU 7372800UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#include "wifi_module.h"
#include "dht11.h"
#define BUFFER_SIZE 400
volatile char global_buffer[BUFFER_SIZE];
volatile uint8_t global_buffer_pos = 0;

uint8_t integral_rh, decimal_rh, integral_temp, decimal_temp, checksum;

// Interrupt service routine for USART Receive Complete
ISR(USART_RXC_vect) {
	char receivedByte = UDR;
	// Add the received byte to the buffer, if there's space
	if (global_buffer_pos < (BUFFER_SIZE - 1)) {
		cli();
		global_buffer[global_buffer_pos] = receivedByte;
		global_buffer_pos++;
		sei();
	}
}

// read wifi-module responses - debugging purposes
void show_at_command_responses() {
	cli();
	
	_delay_ms(2000);
	uart_send_buffer("\r\n \r\nRESPONSES \r\n \r\n");
	_delay_ms(2000);
	uart_send_buffer(global_buffer);
	
	sei();
}
int main(void) {
	
	global_buffer[BUFFER_SIZE];
	memset(global_buffer, 0, sizeof(global_buffer));
	global_buffer_pos = 0;
	
	uart_esp_init();
	reset_esp8266();
	set_same_baud_rate();
	
	disable_receive_complete_interrupt();
	
	_delay_ms(2000);
	send_string("AT+CWMODE_CUR=1\r\n"); // station mode
	
	_delay_ms(2000);
	send_string("ATE1\r\n"); // enable echo back
	
	_delay_ms(4000);
	send_string("AT+CIPMUX=0\r\n"); // single connection

	_delay_ms(20000);
	send_string("AT+CWJAP_CUR=\"HUAWEI\",\"password\"\r\n"); // connect to AP
	
	enable_receive_complete_interrupt();
	
	// wait until connection is stable
	_delay_ms(30000);
	send_string("AT+CIPSTART=\"TCP\",\"io.adafruit.com\",80\r\n"); // establish connection

	while(1)
	{
		request_dht11();
		response_from_dht11();
			
		integral_rh = receive_dht11_data();
		decimal_rh = receive_dht11_data();
		integral_temp = receive_dht11_data();
		decimal_temp = receive_dht11_data();
		checksum = receive_dht11_data();
			
		// check for no error
		if ((integral_rh + decimal_rh + integral_temp + decimal_temp) == checksum) {
			char postRequest[203];
			sprintf(postRequest,"POST /api/v2/lukabis/feeds/my-first-feed/data HTTP/1.1\r\nHost: io.adafruit.com\r\nX-AIO-Key: aio_QhPo58UnyKHlLIUX3N9t1wzVkQ7P\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 8\r\n\r\nvalue=%d", integral_temp);
				
			_delay_ms(10000);
			send_string("AT+CIPSEND=202\r\n"); // length of the request
				
				
			_delay_ms(2000);
			send_string(postRequest); // send request
		}
		
		_delay_ms(5000);
	}
}

