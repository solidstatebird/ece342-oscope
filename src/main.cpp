#include <Arduino.h>


/* Encoder Library - TwoKnobs Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */

// This optional setting causes Encoder to use more optimized code,
// It must be defined before Encoder.h is included.
#define ENCODER_OPTIMIZE_INTERRUPTS

#include <Encoder.h>

// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability

//Encoder knobLeft(5, 6);
//Encoder knobRight(7, 8);
Encoder knob(1, 2 );

volatile int oldPosition;
volatile int newPosition;

volatile int position0;
volatile int position1;
volatile int position2;
volatile int position3;

volatile int mode;

//   avoid using pins with LEDs attached

void displayMode() {
   Serial.print("Mode: ");
   if (mode == 0) {
      Serial.print("horizontal scale");
   }
   else if (mode == 1) {
      Serial.print("vertical scale 1");
   }
   else if (mode == 2) {
      Serial.print("vertical scale 2");
   }
   else if (mode == 3) {
      Serial.print("trigger");
   }
   else {}
   Serial.println();
}

void displayPosition() {
   Serial.print("horz: ");
   Serial.print(position0);
   Serial.print("\t vert1: ");
   Serial.print(position1);
   Serial.print("\t vert2: ");
   Serial.print(position2);
   Serial.print("\t trig: ");
   Serial.println(position3);
}

void rotate() {
   int knobValue = knob.read();
   //newPosition = (knobValue / 4);
   newPosition = knobValue;
   int diff = newPosition - oldPosition;
   if (diff != 0) {
      if (mode == 0) {
         position0 = position0 + diff;
         if (position0 < 1) {
            position0 = 0;
         }
         else if (position0 > 999) {
            position0 = 1000;
         }
      }
      else if (mode == 1) {
         position1 = position1 + diff;
         if (position1 < 1) {
            position1 = 0;
         }
         else if (position1 > 999) {
            position1 = 1000;
         }
      }
      else if (mode == 2) {
         position2 = position2 + diff;
         if (position2 < 1) {
            position2 = 0;
         }
         else if (position2 > 999) {
            position2 = 1000;
         }
      }
      else if (mode == 3) {
         position3 = position3 + diff;
         if (position3 < 1) {
            position3 = 0;
         }
         else if (position3 > 999) {
            position3 = 1000;
         }
      }
      else {
      }
      oldPosition = newPosition;
      displayPosition();
   }
   
   
}

void modeChange() {
   if (mode == 3) {
      mode = 0;
   }
   else {
      mode++;
   }
   displayMode();
   delayMicroseconds(50);
}

void setup() {
   Serial.begin(9600);
   
   Serial.println("Encoder Test:");

   pinMode(0, INPUT_PULLUP);
   pinMode(1, INPUT_PULLUP);
   pinMode(2, INPUT_PULLUP);
   
   attachInterrupt(digitalPinToInterrupt(2), rotate, FALLING);

   attachInterrupt(digitalPinToInterrupt(0), modeChange, FALLING);

   oldPosition = knob.read();
   newPosition = oldPosition;

   mode = 0;
   position0 = 500;
   position1 = 500;
   position2 = 500;
   position3 = 500;

   displayMode();
   displayPosition();
}

void loop() {
  
}

