#include <Arduino.h>
#include "display.h"
#include "sampler.h"

const float user_scale_1 = 1.0;
const float user_scale_2 = 1.0;

const int channel_1_pin = A4;
const int channel_2_pin = A5;

int16_t channel_1_data[BUFFER_SIZE];
int16_t channel_2_data[BUFFER_SIZE];
uint16_t screen_channel_1_data[BUFFER_SIZE];
uint16_t screen_channel_2_data[BUFFER_SIZE];

ILI9341_t3n tft = ILI9341_t3n(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCK, TFT_MISO);
Display *display;

void setup()
{
    while (!Serial && millis() < 5000)
        ; // Wait 5 seconds for serial connection
    display = new Display(screen_channel_1_data, screen_channel_2_data, &tft);

    setupADC(channel_1_pin, channel_2_pin);
}

void loop()
{

    if (DMA_completed())
    {
        processBuffers(channel_1_data, channel_2_data, user_scale_1, user_scale_2);
        
        //temporary: convert from mV to full 16 bit range (6V = )
        for(int i = 0; i < BUFFER_SIZE; i++) {
            screen_channel_1_data[i] = channel_1_data[i] * 5.46133333333 + 32767.0;
            screen_channel_2_data[i] = channel_2_data[i] * 5.46133333333 + 32767.0;
        }

        display->update();
    }
}