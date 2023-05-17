
#include <Arduino.h>
#include <display.h>

uint16_t a[32000];
uint16_t b[32000];


ILI9341_t3n tft = ILI9341_t3n(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCK, TFT_MISO);
Display *display;

void setup() {
    delay(2000);
    Serial.println("enter setup");
    display = new Display(a, b, &tft);
   long sineval;
  
  for (int i = 0; i < 32000; i++) {
    // sine wave for given freq  (max input amplitude = 32767), shifted half way up of max uint16 value (65535)
    sineval = (10000.0 * sin(((10000 * i) / 1300000.0) * 2.0 * PI));
    a[i] = (sineval + 32767); 
    b[i] = (-0.5* sineval + 32767);
  }
}

void loop() {
    //display->update();
    while (display->trigger < 40) {
    display->trigger++;
    display->update();
  }
  while (display->trigger > 2) {
    display->trigger--;
    display->update();
  }
}