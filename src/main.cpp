#include <Arduino.h>



// This optional setting causes Encoder to use more optimized code,
// It must be defined before Encoder.h is included.
#define ENCODER_OPTIMIZE_INTERRUPTS

#include <Encoder.h>

Encoder knob(1, 2);


// The following variables are volatile so they may be accessed in all functions
volatile int oldPosition;
volatile int newPosition;

volatile int position0;
volatile int position1;
volatile int position2;
volatile int position3;

volatile int mode;

// Display mode on serial monitor when switched
// For block 1 checkoff
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

int knobRange(int position) {
   if (position < 1) {
      position = 0;
   }
   else if (position > 999) {
      position = 1000;
   }
   return position;
}

void rotate() {
   int knobValue = knob.read();
   newPosition = (knobValue / 4);
   //newPosition = knobValue;
   int diff = newPosition - oldPosition;
   if (diff != 0) {
      if (mode == 0) {
         position0 = knobRange(position0 + diff);
      }
      else if (mode == 1) {
         position1 = knobRange(position1 + diff);
      }
      else if (mode == 2) {
         position2 = knobRange(position2 + diff);
      }
      else if (mode == 3) {
         position3 = knobRange(position3 + diff);
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
   
   attachInterrupt(digitalPinToInterrupt(2), rotate, CHANGE);

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

