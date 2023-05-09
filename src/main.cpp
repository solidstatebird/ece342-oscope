
#include <Arduino.h>
#include <cmath>
#include "SPI.h"
#include "ILI9341_t3.h"
#include "font_Arial.h"

#define TFT_DC  9
#define TFT_CS 10

volatile uint16_t trigger = 40000;
volatile uint16_t hscale = 97;
volatile uint16_t testarray[32000];
volatile uint16_t frequency;
//volatile uint16_t vscale1 = 10;

ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);

void sinewave(long freq) {
  //uint16_t testarray[32000];
  long sineval;
  for (int i = 0; i < 32000; i++) {
    // sine wave for given freq  (max input amplitude = 32767), shifted half way up of max uint16 value (65535)
    sineval = (16000 * sin(((freq * i) / 1500000.0) * 2.0 * PI));
    testarray[i] = (sineval + 32767.5); 
  }
  //return testarray;
}

int triggerStart(int trig) {
  int i = 0;
  while (testarray[i] < trig && i < 32000) {
    i++;
  }
  return i;
}

void drawWave() {
  //uint16_t testarray[] = sinewave(1000);
  //tft.fillScreen(ILI9341_BLACK);

  int y1, y2;
  int j = 1;
  int start = triggerStart(trigger);
  //y1 = (240 * (testarray[start] / 65535.0));
  y1 = (211 - (206.0 * (testarray[start] / 65535.0)));
  //Serial.println(start);

  //tft.setCursor(0, 0);
  //tft.setTextColor(ILI9341_WHITE);  
  //tft.setFont(Arial_14);
  //tft.println("Oscilloscope!");
  //tft.setFont(Arial_10);
  //tft.println("Look at that sexy sine wave");
  if (start > 30000) {
    
    return;
  }
  else if (hscale < 99) {
    //y1 = (240 * (testarray[start] / 65535.0));
    for (int i = 10; i < 313; i+=2) {
      //y1 = (240 * (testarray[start + (i * (100-hscale))] / 65535.0));
      //y2 = (240 * (testarray[start + (j * (100 - hscale))] / 65535.0));
      y2 = (211 - (206.0 * (testarray[start + (j * (100 - hscale))] / 65535.0)));
      //y2 = ((120 * sin(i))+120);
      tft.drawLine(i, y1, (i+2), y2, ILI9341_WHITE);
      y1 = y2;
      j++;
    }
  }
  else {
    //y1 = (240 * (testarray[start] / 65535.0));
    for (int i = 10; i < 313; i+=(hscale - 97)) {
      //y1 = (240 * (testarray[start + j] / 65535.0));
      //y2 = (240 * (testarray[start + j] / 65535.0));
      y2 = (211 - (206.0 * (testarray[start + j] / 65535.0)));
      //y2 = ((120 * sin(i))+120);
      tft.drawLine(i, y1, (i + (hscale - 98)), y2, ILI9341_WHITE);
      y1 = y2;
      j++;
    }
  }
}

void testFill() {
  tft.fillScreen(ILI9341_BLACK);
  delay(2000);
  tft.fillScreen(ILI9341_WHITE);
  delay(2000);
}

void setup() {
  tft.begin();
  //tft.setClock(60000000);
  tft.fillScreen(ILI9341_BLACK);
  //tft.setTextColor(ILI9341_WHITE);
  //tft.setTextSize(2);
  frequency = 10000;
  sinewave(frequency);
  
  tft.println("Waiting for Arduino Serial Monitor...");

  Serial.begin(9600);
  while (!Serial) ; // wait for Arduino Serial Monitor
  Serial.println("ILI9341 Test!"); 
  tft.setRotation(1);
  Serial.println(tft.width()),
  Serial.println(tft.height());
  for (int i = 0; i < 2000; i++) {
    Serial.println(testarray[i]);
  }
  
}

void drawTrigger() {
  int trig = (211 - (206 * trigger / 65535.0));
  tft.fillTriangle(1, (trig + 4), 1, (trig-4), 8, trig, ILI9341_WHITE);
}

void updateScreen() {
  tft.fillScreen(ILI9341_BLACK);
  tft.drawRect(10, 4, 305, 212, ILI9341_WHITE);
  tft.drawLine(10, 108, 313, 108, ILI9341_WHITE);
  drawWave();
  drawTrigger();
  tft.setCursor(10, 220);
  tft.setTextColor(ILI9341_WHITE);  
  tft.setFont(Arial_12);
  tft.println("Control: Horizontal Scale");
  //tft.setFont(Arial_10);
  //tft.println("Look at that sexy sine wave");
  delay(4);

}

void loop() {
  while(hscale < 102) { 
    while(trigger < 44000) {
      updateScreen();
      trigger+=800;
    }
    while(trigger > 36000) {
      updateScreen();
      trigger-=800;
    }
    hscale++;
  }
  while(hscale > 93) {
    while(trigger < 44000) {
      updateScreen();
      trigger+=800;
    }
    while(trigger > 36000) {
      updateScreen();
      trigger-=800;
    }
    hscale--;
  }
  
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