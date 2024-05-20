// DAC_MCP4725.cpp


#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_MCP4725.h>

#include "DAC_MCP4725.h"
#include "PICO_Serial.h"

static Adafruit_MCP4725 dac;

#define DAC_RESOLUTION    (9)   // Set this value to 9, 8, 7, 6 or 5 to adjust the resolution

DAC_MCP4725* DAC_MCP4725::instance = nullptr;

DAC_MCP4725::DAC_MCP4725()
{
    Wire1.setSDA(2);
    Wire1.setSCL(3);
    Wire1.setClock(400000);

    Serial.println("MCP4725 Test");

    if (dac.begin(0x60, &Wire1))        //could be 0x60 or 0x62
    {
        Serial.println("MCP4725 Initialized Successfully.");
        digitalWrite(LED_BUILTIN, HIGH);
    }
    else
    {
        Serial.println("Failed to Initialize MCP4725.");

        digitalWrite(LED_BUILTIN, LOW);
    }

}


bool DAC_MCP4725::SetBuffer(int16_t* buffer, uint32_t size)
{
    return false;
}

bool DAC_MCP4725::PlayBuffer() const
{
    return false;
}

DAC_MCP4725* DAC_MCP4725::GetInstance()
{
    if (instance == nullptr)
        instance = new DAC_MCP4725();
    return instance;
}

void DAC_MCP4725::DCTest10Sec() const
{
    dac.setVoltage((1 * 4095) / 5, false);    //Set voltage to 1V
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);
    dac.setVoltage((2 * 4095) / 5, false);    //Set voltage to 2V
    delay(2000);
    digitalWrite(LED_BUILTIN, LOW);
    dac.setVoltage((3 * 4095) / 5, false);    //Set voltage to 3V
    delay(2000);
    digitalWrite(LED_BUILTIN, HIGH);
    dac.setVoltage((4 * 4095) / 5, false);    //Set voltage to 4V
    delay(2000);
    digitalWrite(LED_BUILTIN, LOW);
    dac.setVoltage(4095, false);              //Set voltage to 5V or (Vcc)
    delay(2000);
    digitalWrite(LED_BUILTIN, HIGH);
}

#define   M_PI 3.14159265358979323846 /* pi */
#define   SAMPLE_WINDOW_SIZE 1024

static int sampleBuffer[SAMPLE_WINDOW_SIZE];
static bool generateSample = true;

void DAC_MCP4725::GenerateSin()
{
    int sampleNr = SAMPLE_WINDOW_SIZE;
    for (int i = 0; i < sampleNr; ++i) {
        sampleBuffer[i] = (int)((sin(M_PI * 2.0 / sampleNr * i) + 1) * 2047.0);
        dac.setVoltage(sampleBuffer[i], false);
    }
}

void DAC_MCP4725::PlaySin()
{
    static int index = 0;
    static uint32_t lastUs = 0;
    static uint32_t SampleRate = 22;

    if (generateSample) {
        GenerateSin();
        generateSample = false;
    }

    for (int i = 0; i < SAMPLE_WINDOW_SIZE; ++i)
    {

        uint32_t nowUs = micros();
        if (nowUs - lastUs >= SampleRate) {
            lastUs = nowUs;
            dac.setVoltage(sampleBuffer[index++], false);
            if (index >= SAMPLE_WINDOW_SIZE)
                index = 0;
        }
    }

}


void DAC_MCP4725::PlaySinDirect()
{

    if (generateSample) {
        GenerateSin();
        generateSample = false;
    }

    for (int i = 0; i < SAMPLE_WINDOW_SIZE; ++i)
    {

        dac.setVoltage(sampleBuffer[i++], false);
    }

}

void DAC_MCP4725::Loop()
{
    PlaySinDirect();
}

