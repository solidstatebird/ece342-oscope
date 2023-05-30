#include <display.h>

Display::Display(int16_t *data1, int16_t *data2, ILI9341_t3n *screen)
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
    int scaling, start;
    
    // calculates vertical position of trigger. Lower value is higher on screen
    int trig = 108.0 - (((trigger - 30) / 30.0) * 104.0);

    // "step" calculates number of samples per pixel horizontally in wave display 
    double step = (Hdiv[hscale] * SAMPLE_RATE / 38.0);

    if (triggerChannel == 0)
    {
        scaling = vscale1;
    }
    else
    {
        scaling = vscale2;
    }

    if (step >= 0.5)
    {
        // ensures there are enough samples to fill screen
        start = 304.0 * step;
        trigPoint = start;
    }
    else
    {
        // ensures there are enough samples to fill screen
        start = 80;
        trigPoint = start;
    }
  
    /*
    // increases trigPoint counter until a sample is reached that crosses trigger threshold
    // checks for samples ahead and behind for better stability
    while (!(((108.0 - (104.0 * (data[triggerChannel][trigPoint + 4] / (1000.0 * 4.0 * Vdiv[scaling])))) <= trig) &&
            ((108.0 - (104.0 * (data[triggerChannel][trigPoint] / (1000.0 * 4.0 * Vdiv[scaling])))) <= trig) && 
            ((108.0 - (104.0 * (data[triggerChannel][trigPoint - 2] / (1000.0 * 4.0 * Vdiv[scaling])))) > trig) &&
            ((108.0 - (104.0 * (data[triggerChannel][trigPoint - 4] / (1000.0 * 4.0 * Vdiv[scaling])))) > trig)) && 
            !(trigPoint > 31000)) 
    {
        trigPoint++;
    }
    */

    // multiply Vdiv * 4 to match amplitude for 50 ohm termination
    while (!(((108.0 + (104.0 * (data[triggerChannel][trigPoint] / (1000.0 * Vdiv[scaling])))) <= trig) && 
            ((108.0 + (104.0 * (data[triggerChannel][trigPoint - 4] / (1000.0 * Vdiv[scaling])))) > trig)) && 
            !(trigPoint > 31000)) 
    {
        trigPoint++;
    }

    if (trigPoint < 30000)
    {
        return trigPoint;
    }
    else
    {
        // returns starting point if trigger threshold not crossed
        return start;
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

// draws waves with trigger point locked to y-axis
// j = 0 at trigger point, negative values for left half of screen, positive for right
void Display::drawInmV(int channel, int start, uint16_t color)
{
    int y1, y2, scaling, sample;
    int j = -152;       // number of pixels to the left of center trigger sample

    // "step" calculates number of samples per pixel horizontally in wave display 
    // (sec/quarter / pixels/quarter) * samples/sec = samples/pixel
    double step = ((Hdiv[hscale] / 38.0) * SAMPLE_RATE);

    if (channel == 0)
    {
        scaling = vscale1;
    }
    else
    {
        scaling = vscale2;
    }

    // x value of line endpoints must be at least 2 pixels apart
    // step >= 0.5 ensures at least one sample every 2 pixels
    if (step >= 0.5)
    {
        sample = round(start + (j * step));
        
        // voltage magnitude matches oscilloscope with 50 ohm terminaltion
        // remove /4 to match 1 MOhm magnitude?
        // multiply Vdiv * 4 to match amplitude for 50 ohm termination
        y1 = (108 + (104.0 * (data[channel][sample] / (1000.0 * Vdiv[scaling]))));
        y1 = vertBoundCheck(y1);
        j+=2;
        for (int i = 11; i < 312; i += 2)
        {
            sample = round(start + (j * step));
            if (sample > 31999)
            {
                return;         // stops drawing wave if no samples left in array
            }
            y2 = (108 + (104.0 * (data[channel][sample] / (1000.0 * Vdiv[scaling]))));
            y2 = vertBoundCheck(y2);

            tft->drawLine(i, y1, (i + 2), y2, color);

            y1 = y2;
            j+=2;
        }
    }
    else        // less than 1 sample every two pixels, starts spreading out adjacent samples
    {
        j = round(Hdiv[hscale] * SAMPLE_RATE * -4);
        // voltage magnitude matches oscilloscope with 50 ohm terminaltion
        // remove /4 to match 1 MOhm magnitude?
        // multiply Vdiv * 4 to match amplitude for 50 ohm termination
        y1 = (108 + (104.0 * (data[channel][start + j] / (1000.0 * Vdiv[scaling]))));
        y1 = vertBoundCheck(y1);
        j++;
        for (float i = 11; i < 312; i += (1.0 / step))
        {
             if ((start + j) > 31999)
            {
                return;         // stops drawing wave if no samples left in array
            }
            y2 = (108 + (104.0 * (data[channel][start + j] / (1000.0 * Vdiv[scaling]))));
            y2 = vertBoundCheck(y2);

            tft->drawLine(i, y1, round(i + (1.0 / step)), y2, color);

            y1 = y2;
            j++;
        }
    }
}

// draws trigger triangle on left side of display window, triangle filled if adjusting trigger value
void Display::drawTrigger()
{
    int trig = (((trigger - 30) / 30.0) * 104.0) + 104;
    int trigPointer = (212 - trig);
    if (mode == 5)
    {
        tft->fillTriangle(2, (trigPointer + 6), 2, (trigPointer - 6), 8, trigPointer, ILI9341_WHITE);
    }
    else
    {
        tft->drawTriangle(2, (trigPointer + 6), 2, (trigPointer - 6), 8, trigPointer, ILI9341_WHITE);
    }
}

// draws gridlines for wave display window
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

// Displays time interval between vertical grid lines
void Display::displayHscale()
{
    int scaling = hscale;

    tft->setFont(Arial_9);
    tft->setCursor(199, 222);
    tft->setTextColor(ILI9341_WHITE);
    tft->print("Time: ");
    if (Hdiv[hscale] >= 1)
    {
        tft->print(Hdiv[hscale], 0);
        tft->print(" s");
    }
    else if (Hdiv[hscale] < 1 && Hdiv[hscale] >= 0.001)
    {
        tft->print((Hdiv[hscale] * 1000.0), 0);
        tft->print(" ms");
    }
    else if (Hdiv[hscale] < 0.001 && Hdiv[hscale] >= 0.000001)
    {
        tft->print((Hdiv[hscale] * 1000000.0), 0);
        tft->print(" us");
    }
    else if (Hdiv[hscale] < 0.000001 && Hdiv[hscale] >= 0.000000001)
    {
        tft->print((Hdiv[hscale] * 1000000000.0), 0);
        tft->print(" ns");
    }
    else
    {
        tft->print((Hdiv[hscale] * 1000000000000.0), 0);
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

// Displays user interface data on bottom of screen and vertical scaling
// Calls functions to display horizontal scaling, run/stop, and trigger channel
void Display::displayData()
{
    tft->setFont(Arial_9);

    tft->setCursor(33, 222);
    tft->setTextColor(CL(224, 204, 27));
    tft->print("CH1: ");

    if (Vdiv[vscale1] >= 1)
    {
        tft->print(Vdiv[vscale1], 2);
        tft->print(" V");
    }
    else
    {
        tft->print((Vdiv[vscale1] * 1000), 0);
        tft->print(" mV");
    }

    tft->setCursor(116, 222);
    tft->setTextColor(CL(52, 214, 201));
    tft->print("CH2: ");

    if (Vdiv[vscale2] >= 1)
    {
        tft->print(Vdiv[vscale2], 2);
        tft->print(" V");
    }
    else
    {
        tft->print((Vdiv[vscale2] * 1000), 0);
        tft->print(" mV");
    }

    displayTrigChannel();
    displayHscale();
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
    //Serial.println(" Received trigger start ");
    tft->fillScreen(ILI9341_BLACK);
    drawGrid();
    //drawIn(0, start, CL(224, 204, 27));
    //drawIn(1, start, CL(52, 214, 201));
    drawInmV(0, start, CL(224, 204, 27));
    drawInmV(1, start, CL(52, 214, 201));
    drawTrigger();
    displayData();
    displayMode();
    // delay(1);
    tft->updateScreen();
}