#ifndef DHT11_H
#define DHT11_H

#include <stdint.h> // For uint8_t definitions

// Define the CPU clock speed if not already defined
#ifndef F_CPU
#define F_CPU 7372800UL
#endif

// Define the pin used for DHT11 data
#define DHT11_PIN 6

// Function prototypes
void request_dht11(void);
void response_from_dht11(void);
uint8_t receive_dht11_data(void);

#endif // DHT11_H

