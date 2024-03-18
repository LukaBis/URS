#ifndef WIFI_MODULE_H
#define WIFI_MODULE_H

#include <stdint.h>

#define BAUD_ESP 115200
#define BAUDRATE_ESP8266 ((F_CPU)/(BAUD_ESP*16UL)-1)
#define BAUD_RSA 9600
#define BAUDRATE_RSA ((F_CPU)/(BAUD_RSA*16UL)-1)
#define ESP8266_RST PB0
#define BUFFER_SIZE 400

void reset_esp8266();
void uart_esp_init();
void uart_transmit(unsigned char data);
void send_string(char* string);
void uart_send_buffer(const char *buffer);
void set_same_baud_rate();
void disable_receive_complete_interrupt();
void enable_receive_complete_interrupt();

#endif /* WIFI_MODULE_H */

