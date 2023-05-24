#include <display.h>

Display::Display(uint16_t *data1, uint16_t *data2, ILI9341_t3n *screen)
    : tft(screen)
{
    data[0] = data1;
    data[1] = data2;
    tft->begin();
    tft->fillScreen(ILI9341_BLACK);

    tft->useFrameBuffer(true);

    tft->setRotation(1);
}

int Display::newTrigger()
{
    int scaling, start, prevPoint;
    int trig = ((trigger / 50.0) * 32767.0) + 32767;

    if (triggerChannel == 0)
    {
        scaling = vscale1;
    }
    else
    {
        scaling = vscale2;
    }

    if (hscale < 99)
    {
        start = ((100 - hscale) * 151);
        trigPoint = start;
    }
    else
    {
        start = 76;
        trigPoint = start;
    }
    prevPoint = trigPoint - 1;
    /*
    while ((((data[triggerChannel][trigPoint] - 32767.0) * scaling) + 32767.0) < trig && trigPoint < 31000)
    {
        trigPoint++;
        prevPoint++;
    }
    */
    while (!(((((data[triggerChannel][trigPoint] - 32767.0) * scaling) + 32767.0) > trig) &&
             ((((data[triggerChannel][prevPoint] - 32767.0) * scaling) + 32767.0) < trig)) &&
           !(trigPoint > 31000))
    {
        trigPoint++;
        prevPoint++;
    }

    if (trigPoint < 30000)
    {
        return trigPoint;
    }
    else
    {
        return start;
    }
}

int Display::midTrigger()
{
    int trig = ((trigger / 50.0) * 32767.0) + 32767;
    if (hscale < 99)
    {
        trigPoint1 = ((100 - hscale) * 151);
    }
    else
    {
        trigPoint1 = 76;
    }
    trigPoint2 = trigPoint1;
    while ((((data[0][trigPoint1] - 32767.0) * vscale1) + 32767.0) < trig && trigPoint1 < 31000)
    {
        trigPoint1++;
    }
    while ((((data[1][trigPoint2] - 32767.0) * vscale2) + 32767.0) < trig && trigPoint2 < 31000)
    {
        trigPoint2++;
    }
    if (trigPoint1 > trigPoint2 && trigPoint1 < 30000)
    {
        return trigPoint1;
    }
    else if (trigPoint2 > trigPoint1 && trigPoint2 < 30000)
    {
        return trigPoint2;
    }
    else if (trigPoint1 < 30000)
    {
        return trigPoint1;
    }
    else if (trigPoint2 < 30000)
    {
        return trigPoint2;
    }
    else
    {
        return 32000;
    }
}

int Display::vertBoundCheck(int vertin)
{
    if (vertin > 211)
    {
        vertin = 211;
    }
    else if (vertin < 5)
    {
        vertin = 5;
    }
    return vertin;
}

void Display::drawIn(int wave, int start, uint16_t color)
{
    int y1, y2, scaling;
    int j = -76;

    if (wave == 0)
    {
        scaling = vscale1;
    }
    else
    {
        scaling = vscale2;
    }

    if (hscale < 99)
    {
        y1 = (108 - (104.0 * (((data[wave][start + (j * (100 - hscale))] - 32767.0) * scaling) / 32767.0)));
        y1 = vertBoundCheck(y1);
        j++;
        for (int i = 11; i < 312; i += 2)
        {
            y2 = (108 - (104.0 * (((data[wave][start + (j * (100 - hscale))] - 32767.0) * scaling) / 32767.0)));
            y2 = vertBoundCheck(y2);

            tft->drawLine(i, y1, (i + 2), y2, color);

            y1 = y2;
            j++;
        }
    }
    else
    {
        j = (-151 / (hscale - 97));
        y1 = (108 - (104.0 * (((data[wave][start + j] - 32767.0) * scaling) / 32767.0)));
        y1 = vertBoundCheck(y1);
        j++;
        for (int i = 11; i < (411 - hscale); i += (hscale - 97))
        {
            y2 = (108 - (104.0 * (((data[wave][start + j] - 32767.0) * scaling) / 32767.0)));
            y2 = vertBoundCheck(y2);

            tft->drawLine(i, y1, (i + (hscale - 97)), y2, color);

            y1 = y2;
            j++;
        }
    }
}

void Display::drawTrigger()
{
    int trig = ((trigger / 50.0) * 32767.0) + 32767;
    int trigPointer = (211 - (206 * trig / 65535.0));
    if (mode == 5)
    {
        tft->fillTriangle(2, (trigPointer + 6), 2, (trigPointer - 6), 8, trigPointer, ILI9341_WHITE);
    }
    else
    {
        tft->drawTriangle(2, (trigPointer + 6), 2, (trigPointer - 6), 8, trigPointer, ILI9341_WHITE);
    }
}

void Display::drawGrid()
{
    // horizontal lines

    tft->drawFastHLine(10, 4, 304, ILI9341_WHITE);
    tft->drawFastHLine(10, 30, 304, CL(50, 50, 50));
    tft->drawFastHLine(10, 56, 304, CL(50, 50, 50));
    tft->drawFastHLine(10, 82, 304, CL(50, 50, 50));
    tft->drawFastHLine(10, 108, 304, CL(100, 100, 100));
    tft->drawFastHLine(10, 134, 304, CL(50, 50, 50));
    tft->drawFastHLine(10, 160, 304, CL(50, 50, 50));
    tft->drawFastHLine(10, 186, 304, CL(50, 50, 50));
    tft->drawFastHLine(10, 212, 304, ILI9341_WHITE);

    // vertical lines
    tft->drawFastVLine(10, 4, 208, ILI9341_WHITE);
    tft->drawFastVLine(48, 5, 207, CL(50, 50, 50));
    tft->drawFastVLine(86, 5, 207, CL(50, 50, 50));
    tft->drawFastVLine(124, 5, 207, CL(50, 50, 50));
    tft->drawFastVLine(162, 5, 207, CL(100, 100, 100));
    tft->drawFastVLine(200, 5, 207, CL(50, 50, 50));
    tft->drawFastVLine(238, 5, 207, CL(50, 50, 50));
    tft->drawFastVLine(276, 5, 207, CL(50, 50, 50));
    tft->drawFastVLine(314, 4, 208, ILI9341_WHITE);
}

double Display::calculateHscale()
{
    double hdiv;
    if (hscale < 99)
    {
        hdiv = (((100.0 - hscale) / SAMPLE_RATE) * 19.0);
    }
    else
    {
        hdiv = ((1.0 / SAMPLE_RATE) * ((hscale - 97) / 19.0));
    }
    return hdiv;
}

void Display::displayHscale(double hdiv)
{
    tft->setFont(Arial_9);
    tft->setCursor(199, 222);
    tft->setTextColor(ILI9341_WHITE);
    tft->print("Time: ");
    if (hdiv >= 1)
    {
        tft->print(hdiv, 1);
        tft->print(" s");
    }
    else if (hdiv < 1 && hdiv >= 0.001)
    {
        tft->print((hdiv * 1000.0), 1);
        tft->print(" ms");
    }
    else if (hdiv < 0.001 && hdiv >= 0.000001)
    {
        tft->print((hdiv * 1000000.0), 1);
        tft->print(" us");
    }
    else if (hdiv < 0.000001 && hdiv >= 0.000000001)
    {
        tft->print((hdiv * 1000000000.0), 1);
        tft->print(" ns");
    }
    else
    {
        tft->print((hdiv * 1000000000000.0), 1);
        tft->print(" ps");
    }
}

void Display::displayRunStop()
{
    tft->setFont(Arial_8);
    if (runStop == 0)
    {
        tft->fillRoundRect(288, 216, 27, 10, 2, ILI9341_WHITE);
        tft->setCursor(292, 217);
        tft->setTextColor(ILI9341_BLACK);
        tft->print("Run");
        tft->setCursor(291, 228);
        tft->setTextColor(ILI9341_WHITE);
        tft->print("Stop");
    }
    else
    {
        tft->setCursor(292, 217);
        tft->setTextColor(ILI9341_WHITE);
        tft->print("Run");
        tft->fillRoundRect(288, 227, 27, 10, 2, ILI9341_WHITE);
        tft->setCursor(291, 228);
        tft->setTextColor(ILI9341_BLACK);
        tft->print("Stop");
    }
}

void Display::displayTrigChannel()
{
    tft->setFont(Arial_8);
    tft->setCursor(8, 217);
    tft->setTextColor(ILI9341_WHITE);
    tft->print("Trig:");

    if (triggerChannel == 0)
    {
        tft->fillRoundRect(6, 227, 23, 10, 2, CL(224, 204, 27));
        tft->setCursor(9, 228);
        tft->setTextColor(ILI9341_BLACK);
        tft->print("CH1");
    }
    else
    {
        tft->fillRoundRect(6, 227, 23, 10, 2, CL(52, 214, 201));
        tft->setCursor(8, 228);
        tft->setTextColor(ILI9341_BLACK);
        tft->print("CH2");
    }
}

void Display::displayData()
{
    float div1 = 1.5 / vscale1;
    float div2 = 1.5 / vscale2;
    double hdiv = calculateHscale();
    tft->setFont(Arial_9);

    tft->setCursor(33, 222);
    tft->setTextColor(CL(224, 204, 27));
    tft->print("CH1: ");

    if (div1 > 1)
    {
        tft->print(div1, 2);
        tft->print(" V");
    }
    else
    {
        tft->print((div1 * 1000), 0);
        tft->print(" mV");
    }

    tft->setCursor(116, 222);
    tft->setTextColor(CL(52, 214, 201));
    tft->print("CH2: ");

    if (div2 > 1)
    {
        tft->print(div2, 2);
        tft->print(" V");
    }
    else
    {
        tft->print((div2 * 1000), 0);
        tft->print(" mV");
    }

    displayTrigChannel();
    displayHscale(hdiv);
    displayRunStop();
}

void Display::displayMode()
{
    if (mode == 1)
    {
        tft->drawRoundRect(31, 217, 83, 20, 2, ILI9341_WHITE);
    }
    else if (mode == 2)
    {
        tft->drawRoundRect(114, 217, 83, 20, 2, ILI9341_WHITE);
    }
    else if (mode == 3)
    {
        tft->drawRoundRect(197, 217, 89, 20, 2, ILI9341_WHITE);
    }
    else if (mode == 4)
    {
        tft->drawRoundRect(286, 214, 31, 25, 2, ILI9341_WHITE);
    }
    else if (mode == 6)
    {
        tft->drawRoundRect(4, 214, 27, 25, 2, ILI9341_WHITE);
    }
}

void Display::update()
{
    // int start = midTrigger();
    int start = newTrigger();
    tft->fillScreen(ILI9341_BLACK);
    drawGrid();
    drawIn(0, start, CL(224, 204, 27));
    drawIn(1, start, CL(52, 214, 201));
    drawTrigger();
    displayData();
    displayMode();
    // delay(1);
    tft->updateScreen();
}