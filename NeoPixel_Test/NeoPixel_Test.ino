#include "NDPixel.h"

NDPixel pixels(8, 8);

void setup() {
  OCR0A = 0xAF; //Set Timer 0 Counter Register Value
  TIMSK0 |= _BV(OCIE0A);

  pixels.setBrightness(50);
}

//INTERRUPT HANDLING
SIGNAL(TIMER0_COMPA_vect) { //Timer 0 Counter Comparison 
  pixels.updateLEDs();
}

void loop() {
  pixels.demo();
  //pixels.demoChase(0, 255, 0, 4);
  delay(1000);
}
