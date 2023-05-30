#pragma once
#include <Arduino.h>
#include <cmath>
#include "SPI.h"
#include "ILI9341_t3n.h"
#include "ILI9341_t3n_font_Arial.h"
#include "display.h"

#define TFT_DC 9
#define TFT_CS 10
#define TFT_RST 6
#define TFT_SCK 13
#define TFT_MISO 12
#define TFT_MOSI 11
#define TOUCH_CS 8

class Display
{
public:
    Display(int16_t *, int16_t *, ILI9341_t3n *);
    void update();

    uint16_t mode = 1;           // mode: 1 - 6
    uint16_t vscale1 = 1;        // Mode 1: 0 - 10
    uint16_t vscale2 = 1;        // Mode 2: 0 - 10
    uint16_t hscale = 5;        // Mode 3: 0 - 13
    uint16_t runStop = 0;        // Mode 4: 0 - 1
    uint16_t triggerChannel = 0; // Mode 5: 0 - 1
    uint16_t trigger = 30;       // Mode 6: 0 - 30

    double Vdiv [11] = {2.0, 1.0, 0.5, 0.2, 0.1, 0.05, 0.02, 0.01, 0.005, 0.002, 0.001};
    double Hdiv [14] = {0.002, 0.001, 0.0005, 0.0002, 0.0001, 0.00005, 0.00002, 0.00001, 0.000005, 0.000002, 0.000001, 0.0000005, 0.0000002, 0.0000001};

private:
    int newTrigger();
    int vertBoundCheck(int vertin);
    void drawInmV(int, int, uint16_t);
    void drawTrigger();
    void drawGrid();
    void displayHscale();
    void displayRunStop();
    void displayTrigChannel();
    void displayData();
    void displayMode();

    int16_t *data[2];

    uint16_t trigPoint = 0;
    uint16_t trigPoint1 = 0;
    uint16_t trigPoint2 = 0;

    const float SAMPLE_RATE = 1176470.58824;

    ILI9341_t3n *tft;
};