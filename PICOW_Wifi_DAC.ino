// PICO MCP4725 DAC

#include <Wire.h>
#include <Adafruit_MCP4725.h>

#include "DAC_MCP4725.h" 
#include "PICO_Serial.h"
#include "PICO_Wifi.h"

void setup()
{
    Serial.begin (115200);
  
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);

    Serial.begin(115200);
    for (int i = 0; i < 5; ++i) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(900);

    }
    WifiSetup ();

    DAC_MCP4725::GetInstance ()->GenerateSin ();
}

void loop()
{
  DAC_MCP4725::GetInstance ()->Loop ();
  WifiLoop();
}

