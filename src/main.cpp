#include <Arduino.h>

#include "sampler.h"

const float user_scale_1 = 1.0;
const float user_scale_2 = 0.5;

const int channel_1_pin = A4;
const int channel_2_pin = A5;


void setup()
{
    while (!Serial && millis() < 5000)
        ; // Wait 5 seconds for serial connection

    setupADC(channel_1_pin, channel_2_pin);
}

void loop()
{
    static bool data_ready = false;
    static int16_t channel_1_data[BUFFER_SIZE];
    static int16_t channel_2_data[BUFFER_SIZE];

    if (DMA_completed())
    {
        // Serial.println();
        // Serial.print("Sample rate: ");
        // Serial.print(1 / ((float)(micros() - timer) / BUFFER_SIZE));
        // Serial.println(" Msps");

        processBuffers(channel_1_data, channel_2_data, user_scale_1, user_scale_2);
        data_ready = true;
    }

    if (data_ready)
    {
        for (uint16_t i = 0; i < BUFFER_SIZE; i++)
        {
            Serial.print(channel_1_data[i]);
            Serial.print(',');
            Serial.println(channel_2_data[i]);
        }
        data_ready = false;
    }
}