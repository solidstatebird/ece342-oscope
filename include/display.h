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
    Display(uint16_t *, uint16_t *, ILI9341_t3n *);
    void update();

    uint16_t mode = 1;           // mode: 1 - 6
    uint16_t vscale1 = 1;        // Mode 1: 1 - 20
    uint16_t vscale2 = 1;        // Mode 2: 1 - 20
    uint16_t hscale = 75;        // Mode 3: 1 - 150
    uint16_t runStop = 0;        // Mode 4: 0 - 1
    uint16_t triggerChannel = 0; // Mode 5: 0 - 1
    uint16_t trigger = 10;       // Mode 6: 1 - 50

private:
    int newTrigger();
    int midTrigger();
    int vertBoundCheck(int vertin);
    void drawIn(int, int, uint16_t);
    void drawTrigger();
    void drawGrid();
    double calculateHscale();
    void displayHscale(double hdiv);
    void displayRunStop();
    void displayTrigChannel();
    void displayData();
    void displayMode();

    uint16_t *data[2];

    uint16_t trigPoint = 0;
    uint16_t trigPoint1 = 0;
    uint16_t trigPoint2 = 0;

    const float SAMPLE_RATE = 1176470.58824;

    ILI9341_t3n *tft;
};