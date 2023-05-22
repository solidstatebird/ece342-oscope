#pragma once
#include <Arduino.h>
#include <Encoder.h>
#include "display.h"

// This optional setting causes Encoder to use more optimized code,
// It must be defined before Encoder.h is included.
#define ENCODER_OPTIMIZE_INTERRUPTS

class KnobIn {
    public:
    int oldPosition;
    int newPosition;

    void rotate(Display * display, Encoder knob);
    void modeChange(Display * display);

    
    private:
    int knobRange(int position, int min, int max);
};