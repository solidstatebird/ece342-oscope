
#include <Arduino.h>
#include <cmath>
#include "SPI.h"
//#include "ILI9341_t3.h"
#include "ILI9341_t3n.h"
#include "ILI9341_t3n_font_Arial.h"
//#include "font_Arial.h"


#define TFT_DC  9
#define TFT_CS 10
#define TFT_RST 6
#define TFT_SCK 13
#define TFT_MISO 12
#define TFT_MOSI 11
#define TOUCH_CS  8

volatile uint16_t mode = 1;             // mode: 1 - 5
volatile uint16_t trigger = 2;         // trigger: 1 - 50
volatile uint16_t hscale = 95;          // hscale: 1 - 150
volatile uint16_t runStop = 0;          // runStop: 0 - 1
volatile uint16_t testarray[32000];
volatile uint16_t testarray2[32000];      
volatile uint16_t frequency;
volatile uint16_t vscale1 = 2;          // vscale1: 1 - 20
volatile uint16_t vscale2 = 4;          // vscale2: 1 - 20
volatile uint16_t trigPoint1 = 0;
volatile uint16_t trigPoint2 = 0;

//ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);
//ILI9341_t3n tft = ILI9341_t3n(TFT_CS, TFT_DC);
ILI9341_t3n tft = ILI9341_t3n(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCK, TFT_MISO);

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
  
  tft.drawFastHLine(10, 4, 304, ILI9341_WHITE);
  tft.drawFastHLine(10, 30, 304, CL(50, 50, 50));
  tft.drawFastHLine(10, 56, 304, CL(50, 50, 50));
  tft.drawFastHLine(10, 82, 304, CL(50, 50, 50));
  tft.drawFastHLine(10, 108, 304, CL(100, 100, 100));
  tft.drawFastHLine(10, 134, 304, CL(50, 50, 50));
  tft.drawFastHLine(10, 160, 304, CL(50, 50, 50));
  tft.drawFastHLine(10, 186, 304, CL(50, 50, 50));
  tft.drawFastHLine(10, 212, 304, ILI9341_WHITE);

  // vertical lines
  tft.drawFastVLine(10, 4, 208, ILI9341_WHITE);
  tft.drawFastVLine(48, 5, 207, CL(50, 50, 50));
  tft.drawFastVLine(86, 5, 207, CL(50, 50, 50));
  tft.drawFastVLine(124, 5, 207, CL(50, 50, 50));
  tft.drawFastVLine(162, 5, 207, CL(100, 100, 100));
  tft.drawFastVLine(200, 5, 207, CL(50, 50, 50));
  tft.drawFastVLine(238, 5, 207, CL(50, 50, 50));
  tft.drawFastVLine(276, 5, 207, CL(50, 50, 50));
  tft.drawFastVLine(314, 4, 208, ILI9341_WHITE);
}

double calculateHscale() {
  double hdiv;
  if (hscale < 99){
    hdiv = (((100.0 - hscale) / 1300000.0) * 19.0);
  }
  else {
    hdiv = ((1.0 / 1300000.0) * ((hscale - 97) / 19.0));
  }
  return hdiv;
}

void displayHscale(double hdiv) {
  tft.setCursor(188, 222);
  tft.setTextColor(ILI9341_WHITE); 
  tft.print("Time: ");
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
  delay(5);
  tft.updateScreen();
}

void setup() {
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);

  frequency = 8000;
  sinewave(frequency);
  sinewave2((frequency));

  tft.useFrameBuffer(true);

  Serial.begin(9600);

  tft.setRotation(1);
}

void loop() {
  
  while (trigger < 40) {
    trigger++;
    updateScreen2();
  }
  while (trigger > 2) {
    trigger--;
    updateScreen2();
  }
  
  /*
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
  */
}

