// PICO_Serial.cpp

#include <Arduino.h>
//#include "hardware/uart.h"

#include "PICO_Serial.h"

//UART Serial2(8, 9, NC, NC);

void SerialSetup()
{
	Serial2.begin(115200);
	while (!Serial2 && millis() < 5000);
	Serial2.println("Serial started.");
}