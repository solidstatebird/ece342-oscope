
#include <Arduino.h>
#include <cmath>
#include "SPI.h"
#include "ILI9341_t3.h"
#include "font_Arial.h"

#define TFT_DC  9
#define TFT_CS 10

volatile uint16_t mode = 1;             // mode: 1 - 5
volatile uint16_t trigger = 2;         // trigger: 1 - 50
volatile uint16_t hscale = 95;          // hscale: 1 - 150
volatile uint16_t runStop = 0;          // runStop: 0 - 1
volatile uint16_t testarray[32000];
volatile uint16_t testarray2[32000];      
volatile uint16_t frequency;
volatile uint16_t vscale1 = 1;          // vscale1: 1 - 20
volatile uint16_t vscale2 = 1;          // vscale2: 1 - 20
volatile uint16_t trigPoint1 = 0;
volatile uint16_t trigPoint2 = 0;

ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);

void sinewave(long freq) {
  long sineval;
  
  for (int i = 0; i < 32000; i++) {
    // sine wave for given freq  (max input amplitude = 32767), shifted half way up of max uint16 value (65535)
    sineval = (10000.0 * sin(((freq * i) / 1300000.0) * 2.0 * PI));
    testarray[i] = (sineval + 32767); 
  }
}

void sinewave2(long freq) {
  long sineval;
  
  for (int i = 0; i < 32000; i++) {
    // sine wave for given freq  (max input amplitude = 32767), shifted half way up of max uint16 value (65535)
    sineval = (6000.0 * sin(((freq * i) / 1300000.0) * 2.0 * PI + 1.0));
    testarray2[i] = (sineval + 32767); 
  }
}

int triggerStart() {
  int i = 0;
  int trig = ((trigger / 50.0) * 32767.0) + 32767;
  while ((((testarray[i] - 32767.0) * vscale1) + 32767.0) < trig && i < 32000) {
    i++;
  }
  return i;
}

int triggerStart2() {
  int i = 0;
  int trig = ((trigger / 50.0) * 32767.0) + 32767;
  while ((((testarray2[i] - 32767.0) * vscale2) + 32767.0) < trig && i < 32000) {
    i++;
  }
  return i;
}

int midTrigger() {
  int trig = ((trigger / 50.0) * 32767.0) + 32767;
  if (hscale < 99) {
    trigPoint1 = ((100 - hscale) * 151);
  }
  else {
    trigPoint1 = 76;
  }
  trigPoint2 = trigPoint1;
  while ((((testarray[trigPoint1] - 32767.0) * vscale1) + 32767.0) < trig && trigPoint1 < 31000) {
    trigPoint1++;
  }
  while ((((testarray2[trigPoint2] - 32767.0) * vscale2) + 32767.0) < trig && trigPoint2 < 31000) {
    trigPoint2++;
  }
  if (trigPoint1 > trigPoint2 && trigPoint1 < 30000) {
    return trigPoint1;
  }
  else if (trigPoint2 > trigPoint1 && trigPoint2 < 30000) {
    return trigPoint2;
  }
  else if (trigPoint1 < 30000) {
    return trigPoint1;
  }
  else if (trigPoint2 < 30000) {
    return trigPoint2;
  }
  else {
    return 32000;
  }
}

int vertBoundCheck(int vertin) {
  if (vertin > 211) {
    vertin = 211;
  }
  else if (vertin < 5) {
    vertin = 5;
  }
  return vertin;
}

void drawIn1(int start) {
  int y1, y2;
  int j = -76;
  //int start = midTrigger();

  if (start > 30000 || trigPoint1 > 30000) {  
    return;
  }
  else if (hscale < 99) { 
    y1 = (108 - (104.0 * (((testarray[start + (j * (100 - hscale))] - 32767.0) * vscale1)/ 32767.0)));
    y1 = vertBoundCheck(y1);
    j++;
    for (int i = 11; i < 312; i+=2) {
      y2 = (108 - (104.0 * (((testarray[start + (j * (100 - hscale))] - 32767.0) * vscale1)/ 32767.0)));
      y2 = vertBoundCheck(y2);
    
      tft.drawLine(i, y1, (i+2), y2, CL(224, 204, 27));
      
      y1 = y2;
      j++;
    }
  }
  else {
    j = (-151 / (hscale - 97));
    y1 = (108 - (104.0 * (((testarray[start + j] - 32767.0) * vscale1)/ 32767.0)));
    y1 = vertBoundCheck(y1);
    j++;
    for (int i = 11; i < (411 - hscale); i+=(hscale - 97)) {
      y2 = (108 - (104.0 * (((testarray[start + j] - 32767.0) * vscale1)/ 32767.0)));
      y2 = vertBoundCheck(y2);

      tft.drawLine(i, y1, (i + (hscale - 97)), y2, CL(224, 204, 27));
      
      y1 = y2;
      j++;
    }
  }
}

void drawIn2(int start) {
  int y1, y2;
  int j = -76;
  //int start = midTrigger();

  if (start > 30000 || trigPoint2 > 30000) {  
    return;
  }
  else if (hscale < 99) { 
    y1 = (108 - (104.0 * (((testarray2[start + (j * (100 - hscale))] - 32767.0) * vscale2)/ 32767.0)));
    y1 = vertBoundCheck(y1);
    j++;
    for (int i = 11; i < 312; i+=2) {
      y2 = (108 - (104.0 * (((testarray2[start + (j * (100 - hscale))] - 32767.0) * vscale2)/ 32767.0)));
      y2 = vertBoundCheck(y2);
    
      tft.drawLine(i, y1, (i+2), y2, CL(52, 214, 201));
      
      y1 = y2;
      j++;
    }
  }
  else {
    j = (-151 / (hscale - 97));
    y1 = (108 - (104.0 * (((testarray2[start + j] - 32767.0) * vscale2)/ 32767.0)));
    y1 = vertBoundCheck(y1);
    j++;
    for (int i = 11; i < (411 - hscale); i+=(hscale - 97)) {
      y2 = (108 - (104.0 * (((testarray2[start + j] - 32767.0) * vscale2)/ 32767.0)));
      y2 = vertBoundCheck(y2);

      tft.drawLine(i, y1, (i + (hscale - 97)), y2, CL(52, 214, 201));
      
      y1 = y2;
      j++;
    }
  }
}

void drawWave() {
  int y1, y2;
  int j = 1;
  int start = triggerStart();

  y1 = (108 - (104.0 * (((testarray[start] - 32767.0) * vscale1)/ 32767.0)));
  y1 = vertBoundCheck(y1);

  if (start > 30000) {  
    return;
  }
  else if (hscale < 99) { 
    for (int i = 12; i < 311; i+=2) {
      y2 = (108 - (104.0 * (((testarray[start + (j * (100 - hscale))] - 32767.0) * vscale1)/ 32767.0)));
      y2 = vertBoundCheck(y2);
    
      tft.drawLine(i, y1, (i+2), y2, CL(224, 204, 27));
      
      y1 = y2;
      j++;
    }
  }
  else {
    for (int i = 12; i < (410 - hscale); i+=(hscale - 97)) {
      y2 = (108 - (104.0 * (((testarray[start + j] - 32767.0) * vscale1)/ 32767.0)));
      y2 = vertBoundCheck(y2);

      tft.drawLine(i, y1, (i + (hscale - 97)), y2, CL(224, 204, 27));
      
      y1 = y2;
      j++;
    }
  }
}

void drawWave2() {
  int y1, y2;
  int j = 1;
  int start = triggerStart2();
  
  y1 = (108 - (104.0 * (((testarray2[start] - 32767.0) * vscale2)/ 32767.0)));
  y1 = vertBoundCheck(y1);

  if (start > 30000) {  
    return;
  }
  else if (hscale < 99) { 
    for (int i = 12; i < 311; i+=2) {
      y2 = (108 - (104.0 * (((testarray2[start + (j * (100 - hscale))] - 32767.0) * vscale2)/ 32767.0)));
      y2 = vertBoundCheck(y2);
    
      tft.drawLine(i, y1, (i+2), y2, CL(52, 214, 201));
      
      y1 = y2;
      j++;
    }
  }
  else {
    for (int i = 12; i < (410 - hscale); i+=(hscale - 97)) {
      y2 = (108 - (104.0 * (((testarray2[start + j] - 32767.0) * vscale2)/ 32767.0)));
      y2 = vertBoundCheck(y2);

      tft.drawLine(i, y1, (i + (hscale - 97)), y2, CL(52, 214, 201));
      
      y1 = y2;
      j++;
    }
  }
}

void drawTrigger() {
  int trig = ((trigger / 50.0) * 32767.0) + 32767;
  int trigPointer = (211 - (206 * trig / 65535.0));
  if (mode == 5) {
    tft.fillTriangle(2, (trigPointer + 6), 2, (trigPointer-6), 8, trigPointer, ILI9341_WHITE);
  }
  else {
    tft.drawTriangle(2, (trigPointer + 6), 2, (trigPointer-6), 8, trigPointer, ILI9341_WHITE);
  }
}

void drawGrid() {
  // horizontal lines
  //tft.drawFastHLine(10, 3, 304, ILI9341_WHITE);
  tft.drawFastHLine(10, 4, 304, ILI9341_WHITE);
  //tft.drawFastHLine(10, 5, 304, ILI9341_WHITE);
  tft.drawFastHLine(10, 30, 304, CL(50, 50, 50));
  tft.drawFastHLine(10, 56, 304, CL(50, 50, 50));
  tft.drawFastHLine(10, 82, 304, CL(50, 50, 50));
  tft.drawFastHLine(10, 108, 304, CL(100, 100, 100));
  tft.drawFastHLine(10, 134, 304, CL(50, 50, 50));
  tft.drawFastHLine(10, 160, 304, CL(50, 50, 50));
  tft.drawFastHLine(10, 186, 304, CL(50, 50, 50));
  //tft.drawFastHLine(10, 211, 304, ILI9341_WHITE);
  tft.drawFastHLine(10, 212, 304, ILI9341_WHITE);
  //tft.drawFastHLine(10, 213, 304, ILI9341_WHITE);
 

  // vertical lines
  //tft.drawFastVLine(9, 4, 208, ILI9341_WHITE);
  tft.drawFastVLine(10, 4, 208, ILI9341_WHITE);
  //tft.drawFastVLine(11, 4, 208, ILI9341_WHITE);
  tft.drawFastVLine(48, 5, 207, CL(50, 50, 50));
  tft.drawFastVLine(86, 5, 207, CL(50, 50, 50));
  tft.drawFastVLine(124, 5, 207, CL(50, 50, 50));
  tft.drawFastVLine(162, 5, 207, CL(100, 100, 100));
  tft.drawFastVLine(200, 5, 207, CL(50, 50, 50));
  tft.drawFastVLine(238, 5, 207, CL(50, 50, 50));
  tft.drawFastVLine(276, 5, 207, CL(50, 50, 50));
  //tft.drawFastVLine(313, 4, 208, ILI9341_WHITE);
  tft.drawFastVLine(314, 4, 208, ILI9341_WHITE);
  //tft.drawFastVLine(315, 4, 208, ILI9341_WHITE);
}

double calculateHscale() {
  double hdiv;
  if (hscale < 99){
    hdiv = (((100.0 - hscale) / 1300000.0) * 19.0);
  }
  else {
    hdiv = ((1.0 / 1300000.0) * ((hscale - 97) / 19.0));
  }
  //Serial.print(hdiv);
  return hdiv;
}

void displayHscale(double hdiv) {
  tft.setCursor(188, 222);
  tft.setTextColor(ILI9341_WHITE); 
  tft.print("Time: ");
  //tft.print(hdiv, 3);
  if (hdiv >= 1) {
    tft.print(hdiv, 1);
    tft.print(" s");
  }
  else if (hdiv < 1 && hdiv >= 0.001) {
    tft.print((hdiv * 1000.0), 1);
    tft.print(" ms");
  }
  else if (hdiv < 0.001 && hdiv >= 0.000001) {
    tft.print((hdiv * 1000000.0), 1);
    tft.print(" us");
  }
  else if (hdiv < 0.000001 && hdiv >= 0.000000001) {
    tft.print((hdiv * 1000000000.0), 1);
    tft.print(" ns");
  }
  else {
    tft.print((hdiv * 1000000000000.0), 1);
    tft.print(" ps");
  }
}

void displayRunStop() {
  tft.setFont(Arial_8);
  if (runStop == 0) {
    tft.fillRoundRect(284, 216, 27, 10, 2, ILI9341_WHITE);
    tft.setCursor(288, 217);
    tft.setTextColor(ILI9341_BLACK); 
    tft.print("Run");
    tft.setCursor(287, 228);
    tft.setTextColor(ILI9341_WHITE); 
    tft.print("Stop");
  }
  else {
    tft.setCursor(288, 217);
    tft.setTextColor(ILI9341_WHITE); 
    tft.print("Run");
    tft.fillRoundRect(284, 227, 27, 10, 2, ILI9341_WHITE);
    tft.setCursor(287, 228);
    tft.setTextColor(ILI9341_BLACK); 
    tft.print("Stop");
  }
}

void displayData() {
  float div1 = 1.5 / vscale1;
  float div2 = 1.5 / vscale2;
  double hdiv =calculateHscale();
  tft.setFont(Arial_9);
  
  tft.setCursor(10, 222);
  tft.setTextColor(CL(224, 204, 27));  
  tft.print("CH1: ");
  
  if (div1 > 1) {
    tft.print(div1, 2);
    tft.print(" V");
  }
  else {
    tft.print((div1 * 1000), 0);
    tft.print(" mV");
  }
  
  tft.setCursor(98, 222);
  tft.setTextColor(CL(52, 214, 201)); 
  tft.print("CH2: ");
  
  if (div2 > 1) {
    tft.print(div2, 2);
    tft.print(" V");
  }
  else {
    tft.print((div2 * 1000), 0);
    tft.print(" mV");
  }
  
  displayHscale(hdiv);
  displayRunStop();
}

void displayMode() {
  if (mode == 1) {
    tft.drawRoundRect(6, 217, 87, 20, 2, ILI9341_WHITE);
  }
  else if (mode == 2) {
    tft.drawRoundRect(94, 217, 87, 20, 2, ILI9341_WHITE);
  }
  else if (mode == 3) {
    tft.drawRoundRect(184, 217, 94, 20, 2, ILI9341_WHITE);
  }
  else if (mode == 4) {
    tft.drawRoundRect(282, 214, 31, 25, 2, ILI9341_WHITE);
  }
}

void updateScreen2() {
  int start = midTrigger();
  tft.fillScreen(ILI9341_BLACK);
  drawGrid();
  drawIn1(start);
  drawIn2(start);
  drawTrigger();
  displayData();
  displayMode();
  delay(10);
}

void updateScreen() {
  tft.fillScreen(ILI9341_BLACK);
  drawGrid();
  drawWave();
  drawWave2();
  drawTrigger();
  tft.setCursor(10, 220);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setFont(Arial_10);
  tft.println("Control: Horizontal Scale");
  //tft.setFont(Arial_10);
  //tft.println("Look at that sexy sine wave");
  delay(10);

}

void setup() {
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);

  frequency = 8000;
  sinewave(frequency);
  sinewave2((frequency));

  Serial.begin(9600);

  tft.setRotation(1);
}

void loop() {
  while (mode == 1) {
    updateScreen2();
      while (vscale1 < 5) {
        vscale1++;
        for (int i = 0; i < 15; i++) {
          updateScreen2();
        }
      }
      while (vscale1 > 1) {
        vscale1--;
        for (int i = 0; i < 15; i++) {
          updateScreen2();
        }
      }
    mode++;
  }
  while (mode == 2) {
    updateScreen2();
      while (vscale2 < 5) {
        vscale2++;
        for (int i = 0; i < 15; i++) {
          updateScreen2();
        }
      }
      while (vscale2 > 1) {
        vscale2--;
        for (int i = 0; i < 15; i++) {
          updateScreen2();
        }
      }
    mode++;
  }
  while (mode == 3) {
    updateScreen2();
      while (hscale < 100) {
        hscale++;
        for (int i = 0; i < 15; i++) {
          updateScreen2();
        }
      }
      while (hscale > 95) {
        hscale--;
        for (int i = 0; i < 15; i++) {
          updateScreen2();
        }
      }
    mode++;
  }
  while (mode == 4) {
    updateScreen2();
      while (runStop < 1) {
        runStop++;
        for (int i = 0; i < 15; i++) {
          updateScreen2();
        }
      }
      while (runStop > 0) {
        runStop--;
        for (int i = 0; i < 15; i++) {
          updateScreen2();
        }
      }
      while (runStop < 1) {
        runStop++;
        for (int i = 0; i < 15; i++) {
          updateScreen2();
        }
      }
      while (runStop > 0) {
        runStop--;
        for (int i = 0; i < 15; i++) {
          updateScreen2();
        }
      }
      while (runStop < 1) {
        runStop++;
        for (int i = 0; i < 15; i++) {
          updateScreen2();
        }
      }
      while (runStop > 0) {
        runStop--;
        for (int i = 0; i < 15; i++) {
          updateScreen2();
        }
      }
    mode++;
  }
  while (mode == 5) {
    updateScreen2();
      while (trigger < 15) {
        trigger++;
        for (int i = 0; i < 5; i++) {
          updateScreen2();
        }
      }
      while (trigger > 2) {
        trigger--;
        for (int i = 0; i < 5; i++) {
          updateScreen2();
        }
      }
    mode = 1;
  }
  /*
  while(hscale < 101) { 
    while(vscale1 < 4) {
      updateScreen2();
      vscale1++;
      frequency+=1000;
      //trigger++;
      sinewave(frequency);
    }
    while(vscale1 > 1) {
      updateScreen2();
      vscale1--;
      frequency-=1000;
      //trigger--;
      sinewave(frequency);
    }
    hscale++;
  }
  while(hscale > 96) {
    while(vscale1 < 4) {
      updateScreen2();
      vscale1++;
      frequency+=1000;
      //trigger++;
      sinewave(frequency);
    }
    while(vscale1 > 1) {
      updateScreen2();
      vscale1--;
      frequency-=1000;
      //trigger--;
      sinewave(frequency);
    }
    hscale--;
  }
  while(hscale < 101) { 
    while(vscale2 < 10) {
      updateScreen2();
      vscale2++;
      frequency+=1000;
      //trigger++;
      sinewave2(frequency);
    }
    while(vscale2 > 1) {
      updateScreen2();
      vscale2--;
      frequency-=1000;
      //trigger--;
      sinewave2(frequency);
    }
    hscale++;
  }
  while(hscale > 96) {
    while(vscale2 < 10) {
      updateScreen2();
      vscale2++;
      frequency+=1000;
      //trigger++;
      sinewave2(frequency);
    }
    while(vscale2 > 1) {
      updateScreen2();
      vscale2--;
      frequency-=1000;
      //trigger--;
      sinewave2(frequency);
    }
    hscale--;
  }
  */
}


/***************************************************
  This is our GFX example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

/*
#include "SPI.h"
#include "ILI9341_t3.h"
#include "font_Arial.h"

// For the Adafruit shield, these are the default.
#define TFT_DC  9
#define TFT_CS 10

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);



unsigned long testFillScreen() {
  unsigned long start = micros();
  tft.fillScreen(ILI9341_BLACK);
  tft.fillScreen(ILI9341_RED);
  tft.fillScreen(ILI9341_GREEN);
  tft.fillScreen(ILI9341_BLUE);
  tft.fillScreen(ILI9341_BLACK);
  return micros() - start;
}

unsigned long testText() {
  tft.fillScreen(ILI9341_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ILI9341_YELLOW); tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(ILI9341_RED);    tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(5);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("I implore thee,");
  tft.setTextSize(1);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
  return micros() - start;
}

unsigned long testProportionalText() {
  tft.fillScreen(ILI9341_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);  tft.setFont(Arial_10);
  tft.println("Hello World!");
  tft.setTextColor(ILI9341_YELLOW); tft.setFont(Arial_16);
  tft.println(1234.56);
  tft.setTextColor(ILI9341_RED);    tft.setFont(Arial_24);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(ILI9341_GREEN);
  tft.setFont(Arial_40);
  tft.println("Groop");
  tft.setFont(Arial_16);
  tft.println("I implore thee,");
  tft.setFont(Arial_10);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
  tft.setFontAdafruit();
  return micros() - start;
}

unsigned long testLines(uint16_t color) {
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = tft.width(),
                h = tft.height();

  tft.fillScreen(ILI9341_BLACK);

  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t     = micros() - start; // fillScreen doesn't count against timing

  tft.fillScreen(ILI9341_BLACK);

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  tft.fillScreen(ILI9341_BLACK);

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);
  t    += micros() - start;

  tft.fillScreen(ILI9341_BLACK);

  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, color);
  x2    = 0;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, color);

  return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2) {
  unsigned long start;
  int           x, y, w = tft.width(), h = tft.height();

  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for(y=0; y<h; y+=5) tft.drawFastHLine(0, y, w, color1);
  for(x=0; x<w; x+=5) tft.drawFastVLine(x, 0, h, color2);

  return micros() - start;
}

unsigned long testRects(uint16_t color) {
  unsigned long start;
  int           n, i, i2,
                cx = tft.width()  / 2,
                cy = tft.height() / 2;

  tft.fillScreen(ILI9341_BLACK);
  n     = min(tft.width(), tft.height());
  start = micros();
  for(i=2; i<n; i+=6) {
    i2 = i / 2;
    tft.drawRect(cx-i2, cy-i2, i, i, color);
  }

  return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
  unsigned long start, t = 0;
  int           n, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  n = min(tft.width(), tft.height()) - 1;
  for(i=n; i>0; i-=6) {
    i2    = i / 2;
    start = micros();
    tft.fillRect(cx-i2, cy-i2, i, i, color1);
    t    += micros() - start;
    // Outlines are not included in timing results
    tft.drawRect(cx-i2, cy-i2, i, i, color2);
  }

  return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for(x=radius; x<w; x+=r2) {
    for(y=radius; y<h; y+=r2) {
      tft.fillCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int           x, y, r2 = radius * 2,
                w = tft.width()  + radius,
                h = tft.height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros();
  for(x=0; x<w; x+=r2) {
    for(y=0; y<h; y+=r2) {
      tft.drawCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

unsigned long testTriangles() {
  unsigned long start;
  int           n, i, cx = tft.width()  / 2 - 1,
                      cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  n     = min(cx, cy);
  start = micros();
  for(i=0; i<n; i+=5) {
    tft.drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      tft.color565(0, 0, i));
  }

  return micros() - start;
}

unsigned long testFilledTriangles() {
  unsigned long start, t = 0;
  int           i, cx = tft.width()  / 2 - 1,
                   cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for(i=min(cx,cy); i>10; i-=5) {
    start = micros();
    tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color565(0, i, i));
    t += micros() - start;
    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      tft.color565(i, i, 0));
  }

  return t;
}

unsigned long testRoundRects() {
  unsigned long start;
  int           w, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  w     = min(tft.width(), tft.height()) - 1;
  start = micros();
  for(i=0; i<w; i+=6) {
    i2 = i / 2;
    tft.drawRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(i, 0, 0));
  }

  return micros() - start;
}

unsigned long testFilledRoundRects() {
  unsigned long start;
  int           i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(ILI9341_BLACK);
  start = micros();
  for(i=min(tft.width(), tft.height()) - 1; i>20; i-=6) {
    i2 = i / 2;
    tft.fillRoundRect(cx-i2, cy-i2, i, i, i/8, tft.color565(0, i, 0));
  }

  return micros() - start;
}

void setup() {
  tft.begin();
// Note: you can now set the SPI speed to any value
// the default value is 30Mhz, but most ILI9341 displays
// can handle at least 60Mhz and as much as 100Mhz
//  tft.setClock(60000000);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(2);
  tft.println("Waiting for Arduino Serial Monitor...");

  Serial.begin(9600);
  while (!Serial) ; // wait for Arduino Serial Monitor
  Serial.println("ILI9341 Test!"); 

  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("Image Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 
  
  Serial.println(F("Benchmark                Time (microseconds)"));

  Serial.print(F("Screen fill              "));
  Serial.println(testFillScreen());
  delay(200);

  Serial.print(F("Text                     "));
  Serial.println(testText());
  delay(600);

  Serial.print(F("Proportional Text        "));
  Serial.println(testProportionalText());
  delay(600);

  Serial.print(F("Lines                    "));
  Serial.println(testLines(ILI9341_CYAN));
  delay(200);

  Serial.print(F("Horiz/Vert Lines         "));
  Serial.println(testFastLines(ILI9341_RED, ILI9341_BLUE));
  delay(200);

  Serial.print(F("Rectangles (outline)     "));
  Serial.println(testRects(ILI9341_GREEN));
  delay(200);

  Serial.print(F("Rectangles (filled)      "));
  Serial.println(testFilledRects(ILI9341_YELLOW, ILI9341_MAGENTA));
  delay(200);

  Serial.print(F("Circles (filled)         "));
  Serial.println(testFilledCircles(10, ILI9341_MAGENTA));

  Serial.print(F("Circles (outline)        "));
  Serial.println(testCircles(10, ILI9341_WHITE));
  delay(200);

  Serial.print(F("Triangles (outline)      "));
  Serial.println(testTriangles());
  delay(200);

  Serial.print(F("Triangles (filled)       "));
  Serial.println(testFilledTriangles());
  delay(200);

  Serial.print(F("Rounded rects (outline)  "));
  Serial.println(testRoundRects());
  delay(200);

  Serial.print(F("Rounded rects (filled)   "));
  Serial.println(testFilledRoundRects());
  delay(200);

  Serial.println(F("Done!"));

}


void loop(void) {
  for(uint8_t rotation=0; rotation<4; rotation++) {
    tft.setRotation(rotation);
    testText();
    delay(1000);
  }
}
*/