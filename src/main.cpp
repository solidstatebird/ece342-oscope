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

volatile int position = 0;
volatile int newPosition;
volatile int mode;

//   avoid using pins with LEDs attached

void rotate() {
   /*
   if (digitalRead(B)) {
      position++;
      Serial.print(position);
      Serial.println();
   }
   else {
      position--;
      Serial.print(position);
      Serial.println();
   }
   */
   
   int knobValue = knob.read();
   newPosition = (knobValue / 4);
   
   if (newPosition != position) {
      Serial.print("Knob: ");
      Serial.print(newPosition);
      Serial.println();
      position = newPosition;
   }
   
}

void modeChange() {
   if (mode == 3) {
      mode = 0;
   }
   else {
      mode++;
   }
   Serial.print("mode: ");
   Serial.print(mode);
   Serial.println();
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

   newPosition = knob.read();
   mode = 0;

}

void loop() {
  
}

