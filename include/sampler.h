#pragma once

#include <Arduino.h>
#include <ADC.h>
#include <AnalogBufferDMA.h>

#define BUFFER_SIZE 32000 // 32767; //breaks if this is more than 1/2 the limit (32,767) also fails if too small - 1600 seems to be around the minimum

void setupADC(uint8_t, uint8_t);
void processBuffers(int16_t *, int16_t *);
bool DMA_completed();