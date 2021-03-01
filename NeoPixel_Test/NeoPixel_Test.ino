#include "NDPixel.h"

NDPixel pixels(8, 16);

#define Button 2
boolean toggleState = false;

void setup() {
  OCR0A = 0xAF; //Set Timer 0 Counter Register Value
  TIMSK0 |= _BV(OCIE0A);

  pinMode(Button, INPUT_PULLUP);
  pixels.setBrightness(255);
}

//INTERRUPT HANDLING
SIGNAL(TIMER0_COMPA_vect) { //Timer 0 Counter Comparison 
  pixels.updateLEDs();
}

void loop() {
  //pixels.demoChase(0, 255, 0, 4);
  if(digitalRead(Button) == LOW) {
    toggleState = !toggleState;
    if(!toggleState) {
      for(int i = 0; i < 16; i++) {
        pixels.setPixel(i, 255, 255, 255);
      }
    }
    else {
      for(int i = 0; i < 16; i++) {
        pixels.setPixel(i, 0, 0, 0);
      }
    }
    delay(300);
  }
}
