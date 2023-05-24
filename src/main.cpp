#include <Arduino.h>
#include <Encoder.h>
#include "display.h"
#include "sampler.h"
#include "input.h"

const int channel_1_pin = A4;
const int channel_2_pin = A5;

const int knob_in_1 = 34;
const int knob_in_2 = 35;
const int knob_push = 33;

int16_t channel_1_data[BUFFER_SIZE];
int16_t channel_2_data[BUFFER_SIZE];
uint16_t screen_channel_1_data[BUFFER_SIZE];
uint16_t screen_channel_2_data[BUFFER_SIZE];

ILI9341_t3n tft = ILI9341_t3n(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCK, TFT_MISO);
Display *display;

Encoder knob(knob_in_1, knob_in_2);
KnobIn knob_in;

int buttonState = 0;
int lastButtonState = 0;

int currentButtonState = 0;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void rotate()
{
    knob_in.rotate(display, knob);
}

void modeChange()
{
    knob_in.modeChange(display);
}

void setup()
{
    while (!Serial && millis() < 5000)
        ; // Wait 5 seconds for serial connection
    display = new Display(screen_channel_1_data, screen_channel_2_data, &tft);

    setupADC(channel_1_pin, channel_2_pin);

    pinMode(knob_in_1, INPUT_PULLUP);
    pinMode(knob_in_2, INPUT_PULLUP);
    pinMode(knob_push, INPUT_PULLUP);
    // attachInterrupt(digitalPinToInterrupt(knob_push), modeChange, FALLING);
    attachInterrupt(digitalPinToInterrupt(knob_in_2), rotate, CHANGE);

    knob_in.oldPosition = knob.read();
    knob_in.newPosition = knob_in.oldPosition;
}

void loop()
{
    if (DMA_completed())
    {
        processBuffers(channel_1_data, channel_2_data);

        // temporary: convert from mV to full 16 bit range (6V = )
        for (int i = 0; i < BUFFER_SIZE; i++)
        {
            screen_channel_1_data[i] = channel_1_data[i] * 5.46133333333 + 32767.0;
            screen_channel_2_data[i] = channel_2_data[i] * 5.46133333333 + 32767.0;
        }

        display->update();
    }

    currentButtonState = digitalRead(knob_push);

    if (currentButtonState != lastButtonState)
    {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay)
    {
        if (currentButtonState != buttonState)
        {
            buttonState = currentButtonState;
            if (buttonState == LOW)
            {
                modeChange();
            }
        }
    }
    lastButtonState = currentButtonState;
}